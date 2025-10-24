#include "cpu.h"

#include <QDebug>

CPU::CPU(QObject *parent) : QObject(parent) {
  cpuFlags = CPUNameSpace::NoFlag; // ставим нулевые флаги
  pc = 0; // ставим счетчик команд
  CPUMemory = new Memory(this);
  connect(CPUMemory, SIGNAL(cellUpdated(unsigned int, unsigned int)), this, SIGNAL(memoryCellUpdated(unsigned int, unsigned int)));


  for (int i = 0; i < 16; i++)
      registers[i] = 0;
  instructionsSet = new InstructionSet(this, CPUMemory); // инициализация инструкций

  // создаем таймер для работы процессора
  mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
  mTimer->start(1000); // частота работы процессора - обновление 1 раз в секунду
}

CPU::~CPU() {
  delete instructionsSet;
}

unsigned int CPU::getRegister(unsigned int reg) const {
  if (reg < 16)
    return registers[reg];
  else
    return 0;
}

void CPU::setRegister(unsigned int reg, unsigned int val) {
  if (reg < 16) {
    registers[reg] = val;
    emit registerUpdated(reg, val);
  }
}

void CPU::setPlaying(bool flag) {
    if (flag)
        mTimer->start();
    else
        mTimer->stop();
}

void CPU::update() {

  unsigned int operation = CPUMemory->read(pc);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  unsigned int command     = (operation >> CPUNameSpace::COMMAND_OFFSET);
  unsigned int operand1    = (operation >> CPUNameSpace::OPERAND1_OFFSET) & CPUNameSpace::OPERAND_MASK;
  unsigned int operand2    = (operation >> CPUNameSpace::OPERAND2_OFFSET) & CPUNameSpace::OPERAND_MASK;
  unsigned int literal     = (operation >> CPUNameSpace::LITERAL_OFFSET)  & CPUNameSpace::LITERAL_MASK;
  unsigned int modificator = operation & CPUNameSpace::MODIFICATOR_MASK;

  Instruction * inst = instructionsSet->getInstruction(command);
  if (inst)
    inst->execute(operand1,operand2,literal,modificator);


  if (pc+1 >= CPUNameSpace::MEMORY_SIZE)
    mTimer->stop();
  else
    pc++;

  emit updateCPU(pc);
}
