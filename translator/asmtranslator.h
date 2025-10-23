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

    void translate(QString text, Memory * mem);

private:
    QVector <unsigned int> analyseToken(QString token);

    // для разбора чем является destination и source
    enum asmTypes {
        number          = 1, // 1000
        reg             = 2, // REG0
        memCell         = 3, // [1000]
        memReg          = 4, // [REG0]
        memRegOffsetNum = 5, // [REG0+4]
        memRegOffsetReg = 6  // [REG0+REG1]
    };

    QMap<QString, unsigned int> instructionCodes;

};

#endif // ASMTRANSLATOR_H
