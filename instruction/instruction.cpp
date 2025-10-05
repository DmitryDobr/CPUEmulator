#include "instruction.h"

Instruction::Instruction(CPU *cp, Memory * mem): cpu(cp), memory(mem){}

Instruction::~Instruction() {
    cpu = nullptr;
    memory = nullptr;
}

void Instruction::execute(int operand1, int operand2, int literal) {
    (void)operand1;
    (void)operand2;
    (void)literal;
}
