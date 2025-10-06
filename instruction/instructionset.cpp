#include "instructionset.h"

#include "../cpu/cpu.h"

#include <QDebug>

InstructionSet::InstructionSet(CPU *cpu, Memory *mem) {
  instructions.insert(0x1F, new HltInstruction(cpu, mem)); // 31
  instructions.insert(0xE, new MovInstruction(cpu, mem));  // 14
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
  // значение, сигнализирующее, что операнд 2 не используется, и в операнд 1 записывается литерал
  // 1001111

    unsigned int source = getSourceValue(operand2, literal, modificator);

    qDebug() << "source val = " << source;
    qDebug() << "dest num   = " << operand1;
    qDebug() << "modificator= " << modificator;

    setDestinationValue(operand1, operand2, literal, modificator, source);
}
