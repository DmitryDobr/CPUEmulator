#include "instruction.h"

Instruction::Instruction(CPU *cp, Memory * mem): cpu(cp), memory(mem){}

Instruction::~Instruction() {
  cpu = nullptr;
  memory = nullptr;
}

void Instruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal) {
  (void)operand1;
  (void)operand2;
  (void)literal;
}
