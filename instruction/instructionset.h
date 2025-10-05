#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <QMap>

#include "instruction.h"

class HltInstruction final : public Instruction {
// завершение работы процессора (перенос счетчика команд в последнюю ячейку памяти
public:
    HltInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(int operand1, int operand2, int literal) override;
};


class InstructionSet
{
// список инструкций
public:
    InstructionSet(CPU * cpu, Memory * mem);
    ~InstructionSet();

    Instruction * getInstruction(unsigned int code);

private:
    QMap <unsigned int, Instruction *> instructions;

};

#endif // INSTRUCTIONSET_H
