#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <QMap>

#include "instruction.h"
#include "arithmetic.h"

class HltInstruction final : public Instruction {
// завершение работы процессора (перенос счетчика команд в последнюю ячейку памяти
public:
    HltInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class MovInstruction final : public Instruction {
// запись данных из source в destination
public:
    MovInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};


class InstructionSet
{
// список инструкций
public:
    InstructionSet(CPU * cpu, Memory * mem);
    ~InstructionSet();

    Instruction * getInstruction(unsigned int code);

private:
    QMap <unsigned int, Instruction *> instructions; // список команд процессора, каждой из которой присваивается свой адрес

};

#endif // INSTRUCTIONSET_H
