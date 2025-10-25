#include "asmtranslator.h"

#include <QDebug>
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
}

bool AsmTranslator::translate(QString text, Memory * mem) {

    QMap<QString, int> sectionAddrs; // по наименованию section: запоминаем адрес памяти в котором находится первая операция
    QMap<QString, int> dataAddrs;    // по наименованию переменной объявленной в data. запоминаем адрес памяти, с которого начинается

    QStringList lst = text.split("\n");
    unsigned int cell = CPUNameSpace::MEMORY_SIZE / 2;
    for (int i = 0; i < lst.length(); i++) {
        unsigned int instructionCode = 0, operand1 = 0, operand2 = 0, literal = 0, modificator = 0;

        QString comandString, destinString, sourceString;

        qDebug() << lst[i];
        QStringList CmdDestSource = lst[i].split(','); // [0] - код команды + destination ; [1] - source
        CmdDestSource.removeAll(""); // удалить пустые строки
        if (CmdDestSource.length() > 1)
            sourceString = CmdDestSource[1];

        QStringList CmdDest = CmdDestSource[0].split(' '); //  [0] - код команды ; [1] - destination
        CmdDest.removeAll(""); // удалить пустые строки
        comandString = CmdDest[0];
        if (CmdDest.length() > 1)
            destinString = CmdDest[1];

        instructionCode = instructionCodes.value(comandString);
        qDebug() << "разбор команды :";

        qDebug() << "destin string = " << destinString;
        qDebug() << "source string = " << sourceString;
        qDebug() << "-------------------------------------------------";

        QVector <unsigned int> destToken, sourToken;

        qDebug() << "анализ токена destination: ";
        destToken = analyseToken(destinString);
        qDebug() << destToken;

        qDebug() << "анализ токена source: ";
        sourToken = analyseToken(sourceString);
        qDebug() << sourToken;

        if (destToken[0] == asmTypes::readError || sourToken[0] == asmTypes::readError)
            return false;

        modificator = getModificator(destToken[0], sourToken[0]);

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

        if (modificator == 7)
            literal = destToken[1];

        qDebug() << "=================================================";
        qDebug() << QString("%1").arg(instructionCode, 5, 2, QChar('0'))
                 << QString("%1").arg(operand1, 6, 2, QChar('0'))
                 << QString("%1").arg(operand2, 6, 2, QChar('0'))
                 << QString("%1").arg(literal, 11, 2, QChar('0'))
                 << QString("%1").arg(modificator, 4, 2, QChar('0'));
        qDebug() << "=================================================";

        unsigned int code = instructionCode;
        code = code << 6;
        code = code | operand1;
        code = code << 6;
        code = code | operand2;
        code = code << 11;
        code = code | literal;
        code = code << 4;
        code = code | modificator;

        qDebug() << QString("%1").arg(code,8,16,QChar('0'));
        qDebug() << "=================================================";

        mem->write(cell,code);
        cell += 1;
    }

    return true;
}

// для анализа чем является destination и source. В начале вернет числовой код чем является,
// потом - непосредственные значения, которые вгоняются в память
QVector<unsigned int> AsmTranslator::analyseToken(QString token) {

    QVector<unsigned int> result;
    QRegularExpression memExpression("\\[.*\\]"); // выражение для поиска в строке символов памяти

    bool flag = true;
    if (token.isEmpty()) {
        qDebug() << "--> отсутствует";
        result.append(asmTypes::empty); // пустой токен
    }
    else if (token.contains("REG") && !memExpression.match(token).hasMatch()) {
        qDebug() << "--> это регистр";
        result.append(asmTypes::reg);
        result.append(token.remove("REG").toUInt(&flag));
    }
    else if (token.contains("REG") && memExpression.match(token).hasMatch()) {
        qDebug() << "--> это память из регистра (возможно со смещением)";

        QStringList buf = token.remove("[").remove("]").split('+');
        unsigned int memreg = buf[0].remove("REG").toUInt(&flag); // номер регистра в котором адрес ячейки памяти

        // если есть смещение
        if (buf.length() > 1 && flag) {
            if (buf[1].contains("REG")) {
                qDebug() << "--> это память из регистра с смещением из регистра";
                result.append(asmTypes::memRegOffsetReg);
                result.append(memreg);
                result.append(buf[1].remove("REG").toUInt(&flag)); // смещение записано в регистре
            }
            else {
                qDebug() << "--> это память из регистра с фиксированным смещением";
                result.append(asmTypes::memRegOffsetNum);
                result.append(memreg);
                result.append(buf[1].toUInt(&flag)); // фиксированное смещение
            }
        }
        else {
            qDebug() << "--> это память из регистра без смещения";
            result.append(asmTypes::memReg); // значение адрес памяти в регистре
            result.append(memreg);
        }
    }
    else if (!token.contains("REG") && memExpression.match(token).hasMatch()) {
        qDebug() << "--> это ячейка памяти";
        result.append(asmTypes::memCell);
        result.append(token.remove("[").remove("]").toUInt(&flag));
    }
    else {
        qDebug() << "--> это числовое значение";
        result.append(asmTypes::number);
        int val = token.toInt(&flag);
        // числовые значения с отрицанием кладутся только в literal (11 бит) для source
        // отрицательные значения в литерале имеют вид 1хххххххххх
        if (val < 0) {
            // -100
            unsigned int number = (unsigned int)(-val); // 100 -> 00001100100
            number = number & 0x3FF; // маска литерала, чтоб обрезать все что дальше 11 бит
            number = number | 0x400; // добавить левый бит знака 1124 -> 10001100100
            result.append(number);
        }
        else
            result.append((unsigned int)val);
    }

    // возникла ошибка при чтении токена
    if (!flag) {
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
    // destination без source (для инструкций перехода)
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
