#ifndef ASMTRANSLATOR_H
#define ASMTRANSLATOR_H

#include <QString>
#include <QMap>
#include <QStringList>

#include "../cpu/cpu.h"
#include "../memory/memory.h"

class AsmTranslator
{
public:
    AsmTranslator();

    bool translate(QString text, Memory * mem);

private:
    QVector <unsigned int> analyseToken(QString token);

    unsigned int getModificator(unsigned int destType, unsigned int sourceType);

    // для разбора чем является destination и source
    enum asmTypes {
        empty           = 0, // пустой
        number          = 1, // 1000
        reg             = 2, // REG0
        memCell         = 3, // [1000]
        memReg          = 4, // [REG0]
        memRegOffsetNum = 5, // [REG0+4]
        memRegOffsetReg = 6, // [REG0+REG1]
        readError       = 7  // ошибка чтения токена
    };

    QMap<QString, unsigned int> instructionCodes;
    // сохраняем номера инструкций чтоб быстро подставлять вместо названий

    QMap<QString, unsigned int> dataLabelsBuffer;
    // по наименованию section: запоминаем адрес памяти в котором находится первая операция
    // по наименованию переменной объявленной в data. запоминаем адрес памяти, с которого начинается

};

#endif // ASMTRANSLATOR_H
