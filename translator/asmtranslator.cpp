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

void AsmTranslator::translate(QString text, Memory * mem) {
    (void)mem;

    QMap<QString, int> sectionAddrs; // по наименованию section: запоминаем адрес памяти в котором находится первая операция
    QMap<QString, int> dataAddrs;    // по наименованию переменной объявленной в data. запоминаем адрес памяти, с которого начинается

    QStringList lst = text.split("\n");
    for (int i = 0; i < lst.length(); i++) {
        unsigned int instructionCode = 0, destination = 0, source = 0, literal = 0, modificator = 0;

        QString comandString, destinString, sourceString;

        qDebug() << lst[i];
        QStringList CmdDestSource = lst[i].split(','); // [0] - код команды + destination ; [1] - source
        if (CmdDestSource.length() > 1)
            sourceString = CmdDestSource[1];

        QStringList CmdDest = CmdDestSource[0].split(' '); //  [0] - код команды ; [1] - destination
        comandString = CmdDest[0];
        if (CmdDest.length() > 1)
            destinString = CmdDest[1];

        instructionCode = instructionCodes.value(comandString);
        qDebug() << "разбор команды :";

        qDebug() << "destin string = " << destinString;
        qDebug() << "source string = " << sourceString;
        qDebug() << "-------------------------------------------------";

        if (!destinString.isEmpty()) {
            qDebug() << "анализ токена destination: ";
            QVector <unsigned int> destToken = analyseToken(destinString);
            qDebug() << destToken;
        }

        if (!sourceString.isEmpty()) {
            qDebug() << "анализ токена source: ";
            QVector <unsigned int> sourToken = analyseToken(sourceString);
            qDebug() << sourToken;
        }


//        qDebug() << "instruc.code = " << instructionCode;
//        qDebug() << "destination  = " << destination;
//        qDebug() << "source       = " << source;
//        qDebug() << "literal      = " << literal;
//        qDebug() << "modificator  = " << modificator;
        qDebug() << "=================================================";
        qDebug() << "=================================================";
    }
}

// для анализа чем является destination и source. В начале вернет числовой код чем является,
// потом - непосредственные значения, которые вгоняются в память
QVector<unsigned int> AsmTranslator::analyseToken(QString token) {

    QVector<unsigned int> result;
    QRegularExpression memExpression("\\[.*\\]"); // выражение для поиска в строке символов памяти

    if (token.contains("REG") && !memExpression.match(token).hasMatch()) {
        qDebug() << "--> это регистр";
        result.append(asmTypes::reg);
        result.append(token.remove("REG").toUInt());
    }
    else if (token.contains("REG") && memExpression.match(token).hasMatch()) {
        qDebug() << "--> это память из регистра (возможно со смещением)";

        QStringList buf = token.remove("[").remove("]").split('+');
        unsigned int memreg = buf[0].remove("REG").toUInt(); // номер регистра в котором адрес ячейки памяти

        // если есть смещение
        if (buf.length() > 1) {
            if (buf[1].contains("REG")) {
                qDebug() << "--> это память из регистра с смещением из регистра";
                result.append(asmTypes::memRegOffsetReg);
                result.append(memreg);
                result.append(buf[1].remove("REG").toUInt()); // смещение записано в регистре
            }
            else {
                qDebug() << "--> это память из регистра с фиксированным смещением";
                result.append(asmTypes::memRegOffsetNum);
                result.append(memreg);
                result.append(buf[1].toUInt()); // фиксированное смещение
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
        result.append(token.remove("[").remove("]").toUInt());
    }
    else {
        qDebug() << "--> это числовое значение";
        result.append(asmTypes::number);
        result.append(token.toUInt());
    }

    return result;
}
