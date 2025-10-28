#include "instruction.h"

#include "../cpu/cpu.h"

Instruction::Instruction(CPU *cp, Memory * mem): cpu(cp), memory(mem){}

Instruction::~Instruction() {
  cpu = nullptr;
  memory = nullptr;
}

void Instruction::execute(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {
  (void)operand1;
  (void)operand2;
  (void)literal;
  (void)modificator;
}

int Instruction::getSourceValue(unsigned int operand2, unsigned int literal, unsigned int modificator) {

    int val = 0;
    if (modificator == 0 || (modificator >= 8 && modificator <= 11)) {
        // модификатор доступа говорит, что 2 операнд ссылается на значение из регистра
        val = (int)cpu->getRegister(operand2);
    }
    else {
        if (modificator == 1 || modificator >= 12) {
            // модификатор доступа говорит, что источник - значение в литерале
            // литерал - 11 бит, первый бит отвечает за знак
            if (literal & 1024)
                val = -(literal & 1023);
            else
                val = literal & 1023;
        }
        else if (modificator == 2) {
            // модификатор доступа говорит, что источник - значение из ячейки памяти с номером из операнда 2
            val = (int)memory->read(operand2);
        }
        else if (modificator == 3) {
            // модификатор доступа говорит, что источник - значение из ячейки памяти с номером из регистра, номер которого в операнде 2
            val = (int)memory->read(cpu->getRegister(operand2));
        }
        else if (modificator == 4) {
            // модификатор доступа говорит, что источник - значение из ячейки памяти с номером из регистра, номер которого в операнде 2 с смещением из литерала
            val = (int)memory->read(cpu->getRegister(operand2+literal));
        }
        else if (modificator == 5) {
            // модификатор доступа говорит, что источник - значение из ячейки памяти с номером из регистра, номер которого в операнде 2
            // с смещением, размер которого в другом регистре, номер которого записан в литерал
            val = (int)memory->read(cpu->getRegister(operand2)+cpu->getRegister(literal));
        }
    }

    return val;
}

int Instruction::getDestinationValue(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator) {

    unsigned int val = 0;
    if (modificator < 6) {
        // модификатор доступа говорит, что источник - значение в регистре, номер которого в операнде 1
        val = cpu->getRegister(operand1);
    }
    if (modificator == 7) {
        val = literal;
    }
    else {
        if (modificator == 8 || modificator == 12) {
            // модификатор доступа говорит, что источник - значение в ячейке памяти, номер которой в операнде 1
            val = memory->read(operand1);
        }
        else if (modificator == 9 || modificator == 13) {
            // модификатор доступа говорит, что источник - значение в ячейке памяти, номер которой записан в регистре, номер которого в операнде 1
            val = memory->read(cpu->getRegister(operand1));
        }
        else if (modificator == 10) {
            // модификатор доступа говорит, что источник - значение в ячейке памяти, номер которой записан в регистре
            // номер которого в операнде 1 со смещением, номер которого записан в литерале
            val = memory->read(cpu->getRegister(operand1) + literal);
        }
        else if (modificator == 11) {
            // модификатор доступа говорит, что источник - значение в ячейке памяти, номер которой записан в регистре
            // номер которого в операнде 1 со смещением, номер которого записан в регистре, номер которого - в литерале
            val = memory->read(cpu->getRegister(operand1) + cpu->getRegister(literal));
        }
        else if (modificator == 14) {
            // модификатор доступа говорит, что источник - значение в ячейке памяти, номер которой записан в регистре
            // номер которого в операнде 1 со смещением, размер которого в операнде 2
            val = memory->read(cpu->getRegister(operand1) + operand2);
        }
        else if (modificator == 15) {
            // модификатор доступа говорит, что источник - значение в ячейке памяти, номер которой записан в регистре
            // номер которого в операнде 1 со смещением, размер которого находится в регистре, номер которого - в операнде 2
            val = memory->read(cpu->getRegister(operand1) + cpu->getRegister(operand2));
        }
    }

    return (int)val;
}

void Instruction::setDestinationValue(unsigned int operand1, unsigned int operand2, unsigned int literal, unsigned int modificator, int value) {

    unsigned int val = (unsigned int) value;

    if (modificator < 6) {
        // модификатор доступа говорит, что назначение - значение в регистре, которое надо поменять
        cpu->setRegister(operand1, val);
    }
    else {
        if (modificator == 8 || modificator == 12) {
            // модификатор доступа говорит, что назначение - значение в ячейке памяти, номер которой в операнде 1, которое надо поменять
            memory->write(operand1, val);
        }
        else if (modificator == 9 || modificator == 13) {
            // модификатор доступа говорит, что назначение - значение в ячейке памяти, номер которой записан в регистре, номер которого в операнде 1
            memory->write(cpu->getRegister(operand1), val);
        }
        else if (modificator == 10) {
            // модификатор доступа говорит, что назначение - значение в ячейке памяти (которую меняем), номер которой записан в регистре
            // номер которого в операнде 1 со смещением, номер которого записан в литерале
            memory->write(cpu->getRegister(operand1) + literal, val);
        }
        else if (modificator == 11) {
            // модификатор доступа говорит, что назначение - значение в ячейке памяти (которую меняем), номер которой записан в регистре
            // номер которого в операнде 1 со смещением, номер которого записан в регистре, номер которого - в литерале
            memory->write(cpu->getRegister(operand1) + cpu->getRegister(literal), val);
        }
        else if (modificator == 14) {
            // модификатор доступа говорит, что назначение - значение в ячейке памяти (которую меняем), номер которой записан в регистре
            // номер которого в операнде 1 со смещением, размер которого в операнде 2
             memory->write(cpu->getRegister(operand1) + operand2, val);
        }
        else if (modificator == 15) {
            // модификатор доступа говорит, что назначение - значение в ячейке памяти (которую меняем), номер которой записан в регистре
            // номер которого в операнде 1 со смещением, размер которого находится в регистре, номер которого - в операнде 2
            memory->write(cpu->getRegister(operand1) + cpu->getRegister(operand2), val);
        }
    }
}
