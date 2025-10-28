#include "asmtranslator.h"

#include <QRegularExpression>

AsmTranslator::AsmTranslator() {
    // запоминаем имя инструкции и ее номер
    instructionCodes = QMap<QString, unsigned int>{
        {"HLT", 0x1f},  // закончить работу
        {"MOV", 0xE},   // перенести
        {"ADD", 0x1},   // сложить
        {"SUB", 0x2},   // вычесть
        {"MUL", 0x3},   // умножить
        {"DIV", 0x4},   // разделить
        {"INC", 0x5},   // инкремент
        {"DEC", 0x6},   // декремент
        {"CMP", 0x7},   // сравнить
        {"AND", 0x8},   // логическое И
        {"OR", 0x9},    // логическое ИЛИ
        {"NOT", 0xA},   // логическое НЕ
        {"XOR", 0xB},   // логическое ИСКЛЮЧАЮЩЕЕ ИЛИ
        {"JC", 0x14},   // переход при поднятом флаге переноса
        {"JNC", 0x15},  // переход при опущенном флаге переноса
        {"JS", 0x16},   // переход при подняом флаге знака
        {"JNS", 0x17},  // переход при опущенном флаге знака
        {"JZ", 0x18},   // переход при поднятом флаге нуля
        {"JNZ", 0x19},  // переход при опущенном флаге нуля
        {"JMP", 0x1A},  // переход
        {"LOOP", 0x1B}, // переход с циклом
    };

    asmLoaded = false; // статус загрузки кода в память
}

bool AsmTranslator::translate(QString text, Memory * mem) {

    asmLoaded = false;
    dataLabelsBuffer.clear();
    sectionsEnd.clear();

    QStringList lst = text.split("\n");
    unsigned int cell = 0;
    int codestartline = 0;
    bool codeSection = false; // проверка в какой секции кода мы находимся
    // если в .data - записываем с начала памяти как данные
    // если в .code - записываем с середины памяти как команды

    if (lst[0] != ".data" && lst[0] != ".text")
        errorString = "Код не начинается с объявления секции";


    // первый проход - запоминаем адресы, на которые указывают
    // переменные в .data
    // и метки в .text
    // также очищаемся от комментариев
    for (int i = 0; i < lst.length(); i++) {
        lst[i] = lst[i].split(';')[0]; // чтобы удалить комментарии
        if (lst[i].isEmpty() || lst[i] == ".data")
            continue;

        if (codeSection) {
            if (lst[i].contains(":")) {
                QString NameData = lst[i]; // [0] - имя данных [1] - сами данные

                if (dataLabelsBuffer.contains(NameData.remove(':').trimmed())) // такой токен уже был в коде или неправильный asm-код
                    errorString = "Повторное объявление именованной секции: " + NameData.remove(':').trimmed();

                sectionsEnd.append(static_cast<int>(cell)); // конец секции

                dataLabelsBuffer.insert(NameData.trimmed(), cell); // запоминаем что токен указывает на текущую ячейку памяти,
                // в которую будет перемещаться pc процессора при выполнении кода
                cell -= 1;
            }

            if (!lst[i].isEmpty()) {
                cell += 1;
            }
        }
        else if (lst[i] == ".text") {
            // с этого момента начинается секция с кодами команд
            codeSection = true;
            cell = CPUNameSpace::MEMORY_SIZE / 2;
            sectionsEnd.append(static_cast<int>(cell)); // начало секции .text
            codestartline = i;
        }
        else {
            // записываем данные в память
            QStringList NameData = lst[i].split(':'); // [0] - имя данных [1] - сами данные

            // такой токен уже был в коде или неправильный asm-код
            if (dataLabelsBuffer.contains(NameData[0].trimmed()) || NameData.length() < 2)
                errorString = "Повторное объявление именованной секции или ошибка в синтаксисе: " + NameData[0].trimmed();

            dataLabelsBuffer.insert(NameData[0].trimmed(), cell); // запоминаем что токен указывает на текущую ячейку памяти,
            // в которой находятся числовые данные

            QStringList values = NameData[1].split(','); // если это последовательность чисел
            // записываем все что есть в память
            for (int j = 0; j < values.length(); j++) {
                int number = values[j].toInt();
                unsigned int val = static_cast<unsigned int>(number);
                mem->write(cell, val);
                cell += 1;
            }
            sectionsEnd.append(static_cast<int>(cell)); // конец секции
        }
    }

    cell = CPUNameSpace::MEMORY_SIZE / 2;
    // второй проход по тексту - пишем команды в память
    for (int i = codestartline+1; i < lst.length(); i++) {

        if (lst[i].isEmpty() || lst[i].contains(':'))
            continue;

        unsigned int instructionCode = 0, operand1 = 0, operand2 = 0, literal = 0, modificator = 0;
        QString comandString, destinString, sourceString; // для промежуточных результатов операции со строками

        QStringList CmdDestSource = lst[i].split(','); // [0] - код команды + destination ; [1] - source
        CmdDestSource.removeAll(""); // удалить пустые строки
        if (CmdDestSource.length() > 1)
            sourceString = CmdDestSource[1];

        QStringList CmdDest = CmdDestSource[0].split(' '); //  [0] - код команды ; [1] - destination
        CmdDest.removeAll(""); // удалить пустые строки
        comandString = CmdDest[0];
        if (CmdDest.length() > 1)
            destinString = CmdDest[1];

        comandString = comandString.trimmed(); // строка с именем команды
        destinString = destinString.trimmed(); // строка с destination (регистр/память)
        sourceString = sourceString.trimmed(); // строка с source (регистр/память/литерал)

        instructionCode = instructionCodes.value(comandString);

        QVector <unsigned int> destToken, sourToken; // разборка токенов dest source
        destToken = analyseToken(destinString);
        sourToken = analyseToken(sourceString);

        if (destToken[0] == asmTypes::readError || sourToken[0] == asmTypes::readError)
            break;

        modificator = getModificator(destToken[0], sourToken[0]); // определяем модификатор

        // сборка operand 1 operand 2 literal из destination source и модификатора
        if (destToken[0] != asmTypes::empty && sourToken[0] != asmTypes::empty) {
            if (modificator < 6) {
                operand1 = destToken[1]; // операнд 1 всегда

                if (modificator == 1)
                    literal = sourToken[1];
                else
                    operand2 = sourToken[1];

                if (modificator >= 4)
                    literal = sourToken[2];
            }

            if (modificator >= 8 && modificator <= 11) {
                operand2 = sourToken[1];

                operand1 = destToken[1];
                if (modificator >= 10)
                    literal = destToken[2];
            }

            if (modificator >= 12 && modificator <= 15) {
                literal = sourToken[1];

                operand1 = destToken[1];
                if (modificator >= 14)
                    operand2 = destToken[2];
            }
        }
        else if (destToken[0] != asmTypes::empty && sourToken[0] == asmTypes::empty) {
            if (modificator == 7)
                literal = destToken[1];
            else
                operand1 = destToken[1];
        }

        // финальная сборка команды
        unsigned int code = instructionCode;
        code = code << 6;
        code = code | operand1;
        code = code << 6;
        code = code | operand2;
        code = code << 11;
        code = code | literal;
        code = code << 4;
        code = code | modificator;

        if (code >= 0x8000000)
            mem->write(cell,code);
        else
            continue;

        cell += 1;
    }

    // финальное заполнение ячеек концов секций
    sectionsEnd.append(static_cast<int>(cell)); // конец секции .text
    sectionsEnd.append(CPUNameSpace::MEMORY_SIZE);

    asmLoaded = true;
    return asmLoaded;
}

// для анализа чем является destination и source. В начале вернет числовой код чем является,
// потом - непосредственные значения, которые вгоняются в память
QVector<unsigned int> AsmTranslator::analyseToken(QString token) {

    QVector<unsigned int> result;
    QRegularExpression memExpression("\\[.*\\]"); // выражение для поиска в строке символов памяти

    bool flag = true;
    if (token.isEmpty()) {
        // отсутствует
        result.append(asmTypes::empty); // пустой токен
    }
    else if (token.contains("REG") && !memExpression.match(token).hasMatch()) {
        // это регистр
        result.append(asmTypes::reg);
        result.append(token.remove("REG").toUInt(&flag));
    }
    else if (token.contains("REG") && memExpression.match(token).hasMatch()) {
        // это память из регистра (возможно со смещением)
        QStringList buf = token.remove("[").remove("]").split('+');
        unsigned int memreg = buf[0].remove("REG").toUInt(&flag); // номер регистра в котором адрес ячейки памяти

        // если есть смещение
        if (buf.length() > 1 && flag) {
            if (buf[1].contains("REG")) {
                // это память из регистра с смещением из регистра
                result.append(asmTypes::memRegOffsetReg);
                result.append(memreg);
                result.append(buf[1].remove("REG").toUInt(&flag)); // смещение записано в регистре
            }
            else {
                // это память из регистра с фиксированным смещением
                result.append(asmTypes::memRegOffsetNum);
                result.append(memreg);
                result.append(buf[1].toUInt(&flag)); // фиксированное смещение
            }
        }
        else {
            // это память из регистра без смещения";
            result.append(asmTypes::memReg); // значение адрес памяти в регистре
            result.append(memreg);
        }
    }
    else if (!token.contains("REG") && memExpression.match(token).hasMatch()) {
        // это ячейка памяти
        QString mem = token.remove("[").remove("]");
        result.append(asmTypes::memCell);

        if (dataLabelsBuffer.contains(mem)) {
            // это ячейка памяти взятая по токену
            result.append(dataLabelsBuffer.value(mem));
        }
        else {
            // это ячейка памяти, взятая напрямую
            result.append(mem.toUInt(&flag));
        }
    }
    else if (dataLabelsBuffer.contains(token)) {
        // это номер ячейки памяти, полученный по токену переменной или секции (считаем как число)
        result.append(asmTypes::number);
        result.append(dataLabelsBuffer.value(token));
    }
    else {
        // это числовое значение
        result.append(asmTypes::number);
        int val = token.toInt(&flag);
        // числовые значения с отрицанием кладутся только в literal (11 бит) для source
        // отрицательные значения в литерале имеют вид 1хххххххххх
        if (val < 0) {
            // -100
            unsigned int number = static_cast<unsigned int>(-val); // 100 -> 00001100100
            number = number & 0x3FF; // маска литерала, чтоб обрезать все что дальше 11 бит
            number = number | 0x400; // добавить левый бит знака 1124 -> 10001100100
            result.append(number);
        }
        else
            result.append(static_cast<unsigned int>(val));
    }

    // возникла ошибка при чтении токена
    if (!flag) {
        errorString = "Ошибка чтения токена: " + token;
        result.clear();
        result.append(asmTypes::readError);
    }

    return result;
}

unsigned int AsmTranslator::getModificator(unsigned int destType, unsigned int sourceType) {

    unsigned int result = 0;

    if (destType != 0 && sourceType != 0) {
        if (destType == asmTypes::reg) { // destination - регистр
            switch (sourceType) {
                case asmTypes::memCell:
                    result = 2;
                break;
                case asmTypes::reg:
                    result = 0;
                break;
                case asmTypes::number:
                    result = 1;
                break;
                case asmTypes::memReg:
                    result = 3;
                break;
                case asmTypes::memRegOffsetNum:
                    result = 4;
                break;
                case asmTypes::memRegOffsetReg:
                    result = 5;
                break;
            }
        }
        else { // destination - какая-то ячейка памяти
            if (sourceType == asmTypes::reg)
                result = 5;
            else
                result = 9;

            result += destType;
        }
    }
    // destination без source
    else if (destType != 0) {
        switch (destType) {
            case asmTypes::number:
                result = 7;
            break;
            case asmTypes::reg:
                result = 5;
            break;
            case asmTypes::memCell:
                result = 8;
            break;
            case asmTypes::memReg:
                result = 9;
            break;
            case asmTypes::memRegOffsetNum:
                result = 10;
            break;
            case asmTypes::memRegOffsetReg:
                result = 11;
            break;
        }
    }

    return result;
}
