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

    virtual void execute(unsigned int operand1, unsigned int operand2, unsigned int literal);

protected:
    CPU * cpu;
    Memory * memory;
};

#endif // INSTRUCTION_H
