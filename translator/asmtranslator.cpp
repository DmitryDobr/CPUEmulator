#include "asmtranslator.h"

#include <QDebug>

AsmTranslator::AsmTranslator() {

}

void AsmTranslator::translate(QString text, Memory * mem) {
    (void)mem;

    QStringList lst = text.split("\n");
    for (int i = 0; i < lst.length(); i++) {
        qDebug() << lst[i];
    }
}
