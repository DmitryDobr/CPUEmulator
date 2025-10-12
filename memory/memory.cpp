#include "memory.h"

Memory::Memory(QObject *parent) : QObject(parent) {
    for (int i = 0 ; i < 64; i++)
        mem[i] = 0;
                          // 00000 000000 000000 00000000000 0000
    mem[0]  = 0x70000021; // 01110 000000 000000 00000000010 0001 - MOV REG0, 2
    mem[1]  = 0x702003C1; // 01110 000001 000000 00000111100 0001 - MOV REG1, 60
    mem[2]  = 0x70400000; // 01110 000010 000000 00000000000 0000 - MOV REG2, REG0
    mem[3]  = 0x7780064C; // 01110 111100 000000 00001100100 1100 - MOV [60], 100
    mem[4]  = 0x77C0069C; // 01110 111110 000000 00001101001 1100 - MOV [62], 105
    mem[5]  = 0x70608003; // 01110 000011 000001 00000000000 0011 - MOV REG3, [REG1]
    mem[6]  = 0x70808025; // 01110 000100 000001 00000000010 0101 - MOV REG4, [REG1+REG2]
    mem[7]  = 0x7020BE8E; // 01110 000001 000001 01111101000 1110 - MOV [REG1+1], 1000
    mem[8]  = 0x8000011;  // 00001 000000 000000 00000000001 0001 - ADD REG0, 1 // REG0=3
    mem[9]  = 0x27A0005C; // 00100 111101 000000 00000000101 1100 - DIV [61], 5
    mem[10] = 0x38000041; // 00111 000000 000000 00000000100 0001 - CMP REG0, 4
    mem[11] = 0xB0000080; // 10110 000000 000000 00000001000 0000 - JS [8]
    mem[12] = 0xF8000000; // 11111 000000 000000 00000000000 0000 - HLT
}

unsigned int Memory::read(unsigned int addr) const {
  if (addr < 64)
    return mem[addr];
  else
      return 0;
}

void Memory::write(unsigned int addr, unsigned int val) {
  if (addr < 64) {
    mem[addr] = val;
    emit cellUpdated(addr, val);
  }
}
