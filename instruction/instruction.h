#ifndef INSTRUCTION_H
#define INSTRUCTION_H

class CPU;
class Memory;

class Instruction
{
    // базовый класс инструкции-команды процессора
public:
    Instruction(CPU * cp, Memory * mem);
    virtual ~Instruction();

    virtual void execute(int operand1, int operand2, int literal);

protected:
    CPU * cpu;
    Memory * memory;
};

#endif // INSTRUCTION_H
