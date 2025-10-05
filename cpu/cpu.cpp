#include "cpu.h"

#include <QDebug>

CPU::CPU(QObject *parent) : QObject(parent) {
    cpuFlags = CPUNameSpace::NoFlag; // ставим нулевые флаги
    pc = 0; // ставим счетчик команд

    cycleCounter = 0;

    instructionsSet = new InstructionSet(this, &CPUMemory); // инициализация инструкций

    // создаем таймер для работы процессора
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
    mTimer->start(1000); // частота работы процессора - обновление 1 раз в секунду
}

CPU::~CPU() {
    delete instructionsSet;
}

unsigned int CPU::getRegister(int reg) const {
    if (reg < 16 && reg > 0)
        return registers[reg];
    else
        throw "register not found";
}

void CPU::setRegister(int reg, unsigned int val) {
    if (reg < 16 && reg > 0)
      registers[reg] = val;
    else
      throw "register not found";
}

void CPU::update() {

    qDebug() << cycleCounter << " update";

    unsigned int operation = CPUMemory.read(pc);
    qDebug() << "Memory cell no. " << pc << " value = " << operation;

    QString binaryString = QString("%1").arg(operation, 32, 2, QChar('0'));
    binaryString.insert(5,'.');
    binaryString.insert(13,'.');
    binaryString.insert(21,'.');
    qDebug() << binaryString;

    qDebug() << "HEX code : " << QString::number(operation, 16);

    unsigned int command = (operation >> CPUNameSpace::COMMAND_OFFSET); // смещение на 27 бит влево, чтобы получить 5 разрядов, определяющих номер команды в целочисленном виде
    qDebug() << "command no." << command;


    Instruction * inst = instructionsSet->getInstruction(command);
    if (inst)
      inst->execute(0,0,0);



    if (pc+1 >= CPUNameSpace::MEMORY_SIZE) {
      qDebug() << "STOP";
      mTimer->stop();
      emit CPUHalt();
    }
    else
      pc++;


    cycleCounter++;

}
