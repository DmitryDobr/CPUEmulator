#include "instructionset.h"

#include "../cpu/cpu.h"

#include <QDebug>

InstructionSet::InstructionSet(CPU *cpu, Memory *mem) {
    instructions.insert(0x1F, new HltInstruction(cpu, mem)); // 31
}

InstructionSet::~InstructionSet() {
    for (int i = 0; i < instructions.values().size(); i++) {
        delete instructions.values()[i];
    }
    instructions.clear();
}

Instruction *InstructionSet::getInstruction(unsigned int code) {
    Instruction * inst = nullptr;
    if (instructions.contains(code))
        inst = instructions.value(code);

    return inst;
}

void HltInstruction::execute(int operand1, int operand2, int literal) {
    (void)operand1;
    (void)operand2;
    (void)literal;

    cpu->setProgramCounter(63);
}
