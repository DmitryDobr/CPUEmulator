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
    QMap<QString, unsigned int> instructionCodes;

};

#endif // ASMTRANSLATOR_H
