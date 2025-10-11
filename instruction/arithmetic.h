#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "instruction.h"

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


#endif // ARITHMETIC_H
