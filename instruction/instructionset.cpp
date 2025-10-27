#include "instructionset.h"

#include "../cpu/cpu.h"
#include <QDebug>

InstructionSet::InstructionSet(CPU *cpu, Memory *mem) {
  instructions.insert(0x1F, new HltInstruction(cpu, mem)); // 31
  instructions.insert(0xE, new MovInstruction(cpu, mem));  // 14

  instructions.insert(0x1, new AddInstruction(cpu, mem));   // 1
  instructions.insert(0x2, new SubInstruction(cpu, mem));   // 2
  instructions.insert(0x3, new MulInstruction(cpu, mem));   // 3
  instructions.insert(0x4, new DivInstruction(cpu, mem));   // 4
  instructions.insert(0x5, new IncInstruction(cpu, mem));   // 5
  instructions.insert(0x6, new DecInstruction(cpu, mem));   // 6
  instructions.insert(0x7, new CmpInstruction(cpu, mem));   // 7

  instructions.insert(0x8, new AndInstruction(cpu, mem));   // 8
  instructions.insert(0x9, new OrInstruction(cpu, mem));    // 9
  instructions.insert(0xA, new NotInstruction(cpu, mem));   // 10
  instructions.insert(0xB, new XorInstruction(cpu, mem));   // 11


  instructions.insert(0x14, new JcInstruction(cpu, mem));   // 20
  instructions.insert(0x15, new JncInstruction(cpu, mem));  // 21
  instructions.insert(0x16, new JsInstruction(cpu, mem));   // 22
  instructions.insert(0x17, new JnsInstruction(cpu, mem));  // 23
  instructions.insert(0x18, new JzInstruction(cpu, mem));   // 24
  instructions.insert(0x19, new JnzInstruction(cpu, mem));  // 25
  instructions.insert(0x1A, new JmpInstruction(cpu, mem));  // 26
  instructions.insert(0x1B, new LoopInstruction(cpu, mem)); // 27
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
  int source = getSourceValue(operand2, literal, modificator);

  setDestinationValue(operand1, operand2, literal, modificator, source);
}
