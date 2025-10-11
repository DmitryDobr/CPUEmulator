#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "instruction.h"

class AndInstruction final : public Instruction {
// поразрядное "И" source и destination, запись в destination
public:
    AndInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class OrInstruction final : public Instruction {
//  поразрядное "ИЛИ" source и destination, запись в destination
public:
    OrInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class NotInstruction final : public Instruction {
//  поразрядное отрицание destination, запись в destination
public:
    NotInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class XorInstruction final : public Instruction {
// поразрядное "исключающее ИЛИ" source и destination, запись в destination
public:
    XorInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};


#endif // BOOLEAN_H
