#include "transition.h"

#include "../cpu/cpu.h"

#include <QDebug>

void JmpInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)modificator;
  cpu->setProgramCounter(literal-1);
}

void LoopInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)modificator;

  if (cpu->getRegister(2) > 0) {
    cpu->setRegister(2,cpu->getRegister(2)-1);
    cpu->setProgramCounter(literal-1);
  }
}

void JcInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)modificator;
  if (cpu->flags() & CPUNameSpace::CarryFlag)
    cpu->setProgramCounter(literal-1);
}

void JncInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)modificator;

  if (!(cpu->flags() & CPUNameSpace::CarryFlag))
    cpu->setProgramCounter(literal-1);
}

void JsInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)modificator;

  if (cpu->flags() & CPUNameSpace::SignFlag)
    cpu->setProgramCounter(literal-1);
}

void JnsInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)modificator;

  if (!(cpu->flags() & CPUNameSpace::SignFlag))
    cpu->setProgramCounter(literal-1);
}

void JzInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)modificator;

  if (cpu->flags() & CPUNameSpace::ZeroFlag)
    cpu->setProgramCounter(literal-1);
}

void JnzInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)modificator;

  if (!(cpu->flags() & CPUNameSpace::ZeroFlag))
    cpu->setProgramCounter(literal-1);
}
