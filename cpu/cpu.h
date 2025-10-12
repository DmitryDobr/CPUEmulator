#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QTimer>

#include "../memory/memory.h"
#include "../instruction/instructionset.h"

namespace CPUNameSpace {
    enum CPUFlag {
        NoFlag      = 0, // нет флагов
        SignFlag    = 1, // флаг 0 - положительный результат 1 - отрицаительный результат операции
        ZeroFlag    = 2, // результат операции равен 0ю
        CarryFlag   = 4  // флаг переноса разряда в беззнаковых вычислениях
    };
    Q_DECLARE_FLAGS(CPUFlags, CPUFlag)
    Q_DECLARE_OPERATORS_FOR_FLAGS(CPUFlags)

    const int MEMORY_SIZE      = 64;    // количество ячеек в памяти процессора

    const int COMMAND_OFFSET   = 27;    // смещение бит в коде операции, чтоб получить номер команды

    const int OPERAND1_OFFSET  = 21;    // смещение бит в коде операции, чтоб получить значние 1 операнда
    const int OPERAND2_OFFSET  = 15;    // смещение бит в коде операции, чтоб получить значние 2 операнда
    const int OPERAND_MASK     = 0x3F;  // битовая маска, при домножении на которую получается значение операнда

    const int LITERAL_OFFSET   = 4;     // смещение бит в коде операции, чтоб получить значние литерала
    const int LITERAL_MASK     = 0x7FF; // битовая маска, при домножении на которую получается значение литерала

    const int MODIFICATOR_MASK = 0xF;   // битовая маска, при домножении на которую получается значение модификатора
}


class CPU : public QObject
{
  Q_OBJECT

public:
  explicit CPU(QObject *parent = nullptr);
  ~CPU();

  CPUNameSpace::CPUFlags flags() const { return cpuFlags; }

  void setFlags(CPUNameSpace::CPUFlags flags) { cpuFlags = flags; }

  unsigned int programCounter() const { return pc; }

  void setProgramCounter(unsigned int c) { pc = c; }

  unsigned int getRegister(unsigned int reg) const;

  void setRegister(unsigned int reg, unsigned int val);

  Memory * memory() const {return CPUMemory;}

signals:
  void updateCPU(unsigned int pCounter);

  void registerUpdated(unsigned int reg, unsigned int val);

  void memoryCellUpdated(unsigned int addr, unsigned int val);

private slots:
  void update();

private:
  /// элементы процессора
  unsigned int registers[16]; // регистры общего назначения (РОН) 16 регистров, размерность 32 бит
  unsigned int pc; // счетчик команд
  Memory * CPUMemory; // память данных и команд
  // архитектура фон-Неймана, поэтому данные и команды "физически" в одном месте
  CPUNameSpace::CPUFlags cpuFlags; // флаги состояния процессора

  InstructionSet * instructionsSet; // список команд процессора

  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  QTimer * mTimer; // таймер, по срабатыванию сигнала timeout() процессор будет
  // считывать команды из памяти и выполнять их
  // команды считываются на основе счетчика команд и выполняются в соответствии с набором инструкций
  int cycleCounter; // счетчик количества исполненных "тактов"
};

#endif // CPU_H
