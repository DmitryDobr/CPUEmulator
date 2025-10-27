#include "arithmetic.h"

#include "../cpu/cpu.h"

#include <QDebug>

void AddInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int source = getSourceValue(operand2, literal, modificator);
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin+source);
}

void SubInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int source = getSourceValue(operand2, literal, modificator);
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin-source);
}

void MulInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int source = getSourceValue(operand2, literal, modificator);
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin*source);
}

void DivInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int source = getSourceValue(operand2, literal, modificator);
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, destin/source);
}

void IncInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  qDebug() << "INC";

  setDestinationValue(operand1, operand2, literal, modificator, destin+1);
}

void DecInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  qDebug() << "DEC ";

  setDestinationValue(operand1, operand2, literal, modificator, destin-1);
}

void CmpInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int source = getSourceValue(operand2, literal, modificator);
  int destin = getDestinationValue(operand1, operand2, literal, modificator);

  qDebug() << "CMP" << destin << " with: " << source;

  if (destin == source)
    cpu->setFlags(cpu->flags() | CPUNameSpace::ZeroFlag); // поднимаем флаг нуля
  else
    cpu->setFlags(cpu->flags() & 0xD); // опускаем флаг нуля

  if (destin >= source)
    cpu->setFlags(cpu->flags() & 0xE); // опускаем флаг знака
  else
    cpu->setFlags(cpu->flags() | CPUNameSpace::SignFlag); // поднимаем флаг знака
}
