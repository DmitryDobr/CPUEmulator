#include "cpu.h"

#include <QDebug>

CPU::CPU(QObject *parent) : QObject(parent) {
  cpuFlags = CPUNameSpace::NoFlag; // ставим нулевые флаги
  pc = 0; // ставим счетчик команд

  cycleCounter = 0;
  RegMemUpdatedFlag = 0;
  lastUpdated = -1;
  lastValue = 0;

  instructionsSet = new InstructionSet(this, &CPUMemory); // инициализация инструкций

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
    throw "register not found";
}

void CPU::setRegister(unsigned int reg, unsigned int val) {
  if (reg < 16)
    registers[reg] = val;
  else
    throw "register not found";
}

void CPU::switchUpdated(bool isMem, unsigned int addr, unsigned int val) {
  RegMemUpdatedFlag = int(isMem) + 1;
  lastUpdated       = addr;
  lastValue         = val;
}

void CPU::update() {

  qDebug() << cycleCounter << " update";

  unsigned int operation = CPUMemory.read(pc);
  qDebug() << "Memory cell no. " << pc << " value = " << operation;

  QString binaryString = QString("%1").arg(operation, 32, 2, QChar('0'));
  binaryString.insert(5,'.');
  binaryString.insert(12,'.');
  binaryString.insert(20,'.');
  binaryString.insert(31,'.');
  qDebug() << binaryString;

  qDebug() << "HEX code : " << QString::number(operation, 16);

  unsigned int command  = (operation >> CPUNameSpace::COMMAND_OFFSET); // смещение на 27 бит влево, чтобы получить 5 разрядов, определяющих номер команды в целочисленном виде
  unsigned int operand1 = (operation >> CPUNameSpace::OPERAND1_OFFSET) & 0x7F;
  unsigned int operand2 = (operation >> CPUNameSpace::OPERAND2_OFFSET) & 0x7F;
  unsigned int literal  = operation & CPUNameSpace::LITERAL_MASK;
  qDebug() << "command no." << command;


  Instruction * inst = instructionsSet->getInstruction(command);
  if (inst)
    inst->execute(operand1,operand2,literal);



  if (pc+1 >= CPUNameSpace::MEMORY_SIZE) {
    qDebug() << "STOP";
    mTimer->stop();
  }
  else
    pc++;

  emit updateCPU(pc, RegMemUpdatedFlag, lastUpdated, lastValue);

  RegMemUpdatedFlag = 0;

  cycleCounter++;
}
