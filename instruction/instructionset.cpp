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

void HltInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal) {
  (void)operand1;
  (void)operand2;
  (void)literal;

  cpu->setProgramCounter(63);
}

void MovInstruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal) {
  // значение, сигнализирующее, что операнд 2 не используется, и в операнд 1 записывается литерал
  // 1001111

    qDebug() << "mov " << operand1 << operand2 << literal;

    unsigned int value = 0;
    if (operand2 == 79) {
        value = literal;
    }
    else {
        if (operand2 >> 7) { // сдвиг на 7 разрядов дал 1 => чтение значения с ячейки памяти
        }
        else { // сдвиг на 7 разрядов дал 0 => чтение значения с регистра
            value = cpu->getRegister(operand2);
        }
    }

    if (operand1 >> 7) { // сдвиг на 7 разрядов дал 1 => кладем значение в ячейку памяти
    }
    else { // сдвиг на 7 разрядов дал 0 => кладем значение в регистр
        cpu->setRegister(operand1,value);           // положили в регистр значение
        cpu->switchUpdated(false,operand1,value);   // для обновления интерфейса
    }
}
