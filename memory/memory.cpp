#include "memory.h"

Memory::Memory(QObject *parent) : QObject(parent) {
    for (int i = 0 ; i < 64; i++)
      mem[i] = 0;
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

void Memory::clear() {
  for (int i = 0 ; i < 64; i++)
    mem[i] = 0;
}
