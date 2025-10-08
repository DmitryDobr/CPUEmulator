#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <QMap>

#include "instruction.h"

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

class AddInstruction final : public Instruction {
// сумма source и destination, запись в destination
public:
    AddInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class SubInstruction final : public Instruction {
// вычитание из destination значение source, запись в destination
public:
    SubInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class MulInstruction final : public Instruction {
// умножение source на destination, запись в destination
public:
    MulInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class DivInstruction final : public Instruction {
// умножение source на destination, запись в destination
public:
    DivInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class IncInstruction final : public Instruction {
// умножение source на destination, запись в destination
public:
    IncInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class DecInstruction final : public Instruction {
// умножение source на destination, запись в destination
public:
    DecInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class CmpInstruction final : public Instruction {
// умножение source на destination, запись в destination
public:
    CmpInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

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
