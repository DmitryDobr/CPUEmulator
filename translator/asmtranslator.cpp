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

    QRegularExpression memExpression("\\[.*\\]"); // выражение для поиска в строке символов памяти

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

        if (destinString.contains("REG") && !memExpression.match(destinString).hasMatch()) {
            qDebug() << "- destination это регистр";
            destination = destinString.remove("REG").toUInt();
        }
        else if (destinString.contains("REG") && memExpression.match(destinString).hasMatch()) {
            qDebug() << "- destination это память из регистра (возможно со смещением)";


//            destination = destinString.remove("[").remove("]").toUInt();
        }
        else if (!destinString.contains("REG") && memExpression.match(destinString).hasMatch()) {
            qDebug() << "- destination это память";
            destination = destinString.remove("[").remove("]").toUInt();
        }
        else {
            qDebug() << "- destination отсутствует";
        }


        qDebug() << "instruc.code = " << instructionCode;
        qDebug() << "destination  = " << destination;
        qDebug() << "source       = " << source;
        qDebug() << "literal      = " << literal;
        qDebug() << "modificator  = " << modificator;
        qDebug() << "=================================================";


    }
}
