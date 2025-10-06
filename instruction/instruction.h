#ifndef INSTRUCTION_H
#define INSTRUCTION_H

class CPU;
class Memory;

class Instruction
{
    // базовый класс инструкции-команды процессора
    // двоичное представление команды
    // 5  бит - код инструкции
    // 6  бит - операнд 1
    // 6  бит - операнд 2
    // 11 бит - литерал
    // 4  бит - модификатор доступа
    // итого 32 бит
    // фактически есть номер операции, destination и source, операция выполняется с помощью d и s и записывается в d
public:
    Instruction(CPU * cp, Memory * mem);
    virtual ~Instruction();

    virtual void execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator);

protected:

    unsigned int getSourceValue(unsigned int operand2, unsigned int literal, unsigned int modificator); // по значению модификатора получает значение из источника

    unsigned int getDestinationValue(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator); // по значению модификатора получает значение из цели

    void setDestinationValue(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator, unsigned int value); // по значению модификатора определяет куда нужно записать и делает запись значения

    CPU * cpu;
    Memory * memory;
};

#endif // INSTRUCTION_H
