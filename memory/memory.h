#ifndef MEMORY_H
#define MEMORY_H

class Memory
{
public:
  Memory();

  unsigned int read(int addr) const {
    if (addr < 64 && addr > -1)
      return mem[addr];
    else
      throw "Memory cell not found";
  }

  void write(int addr, unsigned int val) {
    if (addr < 64 && addr > -1)
      mem[addr] = val;
    else
      throw "Memory cell not found";
  }

private:
  unsigned int mem[64]; // 64 ячейки по 32 бита
};

#endif // MEMORY_H
