#include "instructionset.h"

#include "../cpu/cpu.h"

InstructionSet::InstructionSet(CPU *cpu, Memory *mem) {
  instructions.insert(0x1F, new HltInstruction(cpu, mem)); // 31
  instructions.insert(0xE, new MovInstruction(cpu, mem));  // 14

  instructions.insert(0x1, new AddInstruction(cpu, mem));  // 1
  instructions.insert(0x2, new SubInstruction(cpu, mem));  // 2
  instructions.insert(0x3, new MulInstruction(cpu, mem));  // 3
  instructions.insert(0x4, new DivInstruction(cpu, mem));  // 4
  instructions.insert(0x5, new IncInstruction(cpu, mem));  // 5
  instructions.insert(0x6, new DecInstruction(cpu, mem));  // 6
  instructions.insert(0x7, new CmpInstruction(cpu, mem));  // 7

  instructions.insert(0x8, new AndInstruction(cpu, mem));  // 8
  instructions.insert(0x9, new OrInstruction(cpu, mem));   // 9
  instructions.insert(0xA, new NotInstruction(cpu, mem));  // 10
  instructions.insert(0xB, new XorInstruction(cpu, mem));  // 11

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

void HltInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)literal;
  (void)modificator;

  cpu->setProgramCounter(63);
}

void MovInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  unsigned int source = getSourceValue(operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, source);
}
