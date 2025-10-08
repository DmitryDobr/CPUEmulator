#include "memory.h"

Memory::Memory(QObject *parent) : QObject(parent) {
    for (int i = 0 ; i < 63; i++)
        mem[i] = 0;
                          // 00000 000000 000000 00000000000 0000
    mem[0]  = 1879048225; // 01110 000000 000000 00000000010 0001 - MOV REG0, 2
    mem[1]  = 1881146305; // 01110 000001 000000 00000111100 0001 - MOV REG1, 60
    mem[2]  = 1883242496; // 01110 000010 000000 00000000000 0000 - MOV REG2, REG0
    mem[3]  = 2004878924; // 01110 111100 000000 00001100100 1100 - MOV [60], 100
    mem[4]  = 2009073308; // 01110 111110 000000 00001101001 1100 - MOV [62], 105
    mem[5]  = 1885372419; // 01110 000011 000001 00000000000 0011 - MOV REG3, [REG1]
    mem[6]  = 1887469605; // 01110 000100 000001 00000000010 0101 - MOV REG4, [REG1+REG2]
    mem[7]  = 1881194126; // 01110 000001 000001 01111101000 1110 - MOV [REG1+1], 1000
    mem[8]  = 134217745;  // 00001 000000 000000 00000000001 0001 - ADD REG0, 1
    mem[9]  = 664797276;  // 00100 111101 000000 00000000101 1100 - DIV [61], 5
    mem[10] = 4160749568; // 11111 000000 000000 00000000000 0000 - HLT
}

unsigned int Memory::read(unsigned int addr) const {
  if (addr < 64)
    return mem[addr];
  else
      throw "Memory cell not found";
}

void Memory::write(unsigned int addr, unsigned int val) {
  if (addr < 64) {
    mem[addr] = val;
    emit cellUpdated(addr, val);
  }
  else
    throw "Memory cell not found";
}
