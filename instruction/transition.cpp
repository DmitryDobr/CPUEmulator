#include "transition.h"

#include "../cpu/cpu.h"

void JmpInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  cpu->setProgramCounter(static_cast<unsigned int>(metka-1));
}

void LoopInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  if (cpu->getRegister(2) > 0) {
    cpu->setRegister(2,cpu->getRegister(2)-1);
    cpu->setProgramCounter(static_cast<unsigned int>(metka-1));
  }
}

void JcInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  if (cpu->flags() & CPUNameSpace::CarryFlag)
    cpu->setProgramCounter(static_cast<unsigned int>(metka-1));
}

void JncInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  if (!(cpu->flags() & CPUNameSpace::CarryFlag))
    cpu->setProgramCounter(static_cast<unsigned int>(metka-1));
}

void JsInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  if (cpu->flags() & CPUNameSpace::SignFlag)
    cpu->setProgramCounter(static_cast<unsigned int>(metka-1));
}

void JnsInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  if (!(cpu->flags() & CPUNameSpace::SignFlag))
    cpu->setProgramCounter(static_cast<unsigned int>(metka-1));
}

void JzInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  if (cpu->flags() & CPUNameSpace::ZeroFlag)
    cpu->setProgramCounter(static_cast<unsigned int>(metka-1));
}

void JnzInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  int metka = getDestinationValue(operand1,operand2,literal,modificator);

  if (!(cpu->flags() & CPUNameSpace::ZeroFlag))
    cpu->setProgramCounter(static_cast<unsigned int>(metka-1));
}
