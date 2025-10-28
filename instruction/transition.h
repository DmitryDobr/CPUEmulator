#ifndef TRANSITION_H
#define TRANSITION_H

#include "instruction.h"

class JmpInstruction final : public Instruction {
// переход pc к метке памяти из destination
public:
    JmpInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class LoopInstruction final : public Instruction {
// переход pc к метке памяти из destination и вычитание 1 из регистра С-(cycle) (REG2)
public:
    LoopInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class JcInstruction final : public Instruction {
// переход pc к метке памяти из destination, если флаг переноса установлен
public:
    JcInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class JncInstruction final : public Instruction {
// переход pc к метке памяти из destination, если флаг переноса не установлен
public:
    JncInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class JsInstruction final : public Instruction {
// переход pc к метке памяти из destination, если флаг знака установлен
public:
    JsInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class JnsInstruction final : public Instruction {
// переход pc к метке памяти из destination, если флаг знака не установлен
public:
    JnsInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class JzInstruction final : public Instruction {
// переход pc к метке памяти из destination, если флаг нуля установлен
public:
    JzInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};

class JnzInstruction final : public Instruction {
// переход pc к метке памяти из destination, если флаг нуля не установлен
public:
    JnzInstruction(CPU * cp, Memory * mem) : Instruction(cp, mem) {}

    void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) override;
};


#endif // TRANSITION_H
