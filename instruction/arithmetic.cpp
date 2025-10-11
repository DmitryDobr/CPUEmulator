#include "arithmetic.h"

#include "../cpu/cpu.h"

#include <QDebug>

void AddInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  unsigned int source = getSourceValue(operand2, literal, modificator);
  unsigned int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin+source);
}

void SubInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  unsigned int source = getSourceValue(operand2, literal, modificator);
  unsigned int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin-source);
}

void MulInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  unsigned int source = getSourceValue(operand2, literal, modificator);
  unsigned int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin*source);
}

void DivInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  unsigned int source = getSourceValue(operand2, literal, modificator);
  unsigned int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin/source);
}

void IncInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  unsigned int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin+1);
}

void DecInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  unsigned int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin-1);
}

void CmpInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  unsigned int source = getSourceValue(operand2, literal, modificator);
  unsigned int destin = getDestinationValue(operand1, operand2, literal, modificator);

  qDebug() << "------------------------";
  qDebug() << destin << " - CMP with - " << source;
  qDebug() << "------------------------";

  if (destin == source) {
    qDebug() << "destin == source";
    cpu->setFlags(cpu->flags() | CPUNameSpace::ZeroFlag);
  }
  else if (destin > source) {
    qDebug() << "destin > source";
    cpu->setFlags(cpu->flags() & !CPUNameSpace::SignFlag);
  }
  else if (destin < source) {
    qDebug() << "destin < source";
    cpu->setFlags(cpu->flags() | CPUNameSpace::SignFlag);
  }
}
