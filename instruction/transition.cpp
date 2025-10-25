#include "transition.h"

#include "../cpu/cpu.h"

#include <QDebug>

void JmpInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  qDebug() << "JMP to: " << metka;
  cpu->setProgramCounter((unsigned int)metka-1);
}

void LoopInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  qDebug() << "LOOP to: " << metka;

  if (cpu->getRegister(2) > 0) {
    cpu->setRegister(2,cpu->getRegister(2)-1);
    cpu->setProgramCounter((unsigned int)metka-1);
  }
}

void JcInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  qDebug() << "JC to: " << metka;

  if (cpu->flags() & CPUNameSpace::CarryFlag)
    cpu->setProgramCounter((unsigned int)metka-1);
}

void JncInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  qDebug() << "JNC to: " << metka;
  if (!(cpu->flags() & CPUNameSpace::CarryFlag))
    cpu->setProgramCounter((unsigned int)metka-1);
}

void JsInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  qDebug() << "JS to: " << metka;
  if (cpu->flags() & CPUNameSpace::SignFlag)
    cpu->setProgramCounter((unsigned int)metka-1);
}

void JnsInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  qDebug() << "JNS to: " << metka;
  if (!(cpu->flags() & CPUNameSpace::SignFlag))
    cpu->setProgramCounter((unsigned int)metka-1);
}

void JzInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  qDebug() << "JZ to: " << metka;
  if (cpu->flags() & CPUNameSpace::ZeroFlag)
    cpu->setProgramCounter((unsigned int)metka-1);
}

void JnzInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  qDebug() << "JNZ to: " << metka;
  if (!(cpu->flags() & CPUNameSpace::ZeroFlag))
    cpu->setProgramCounter((unsigned int)metka-1);
}
