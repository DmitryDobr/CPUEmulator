#ifndef MEMORY_H
#define MEMORY_H

class Memory
{
public:
  Memory();

  unsigned int read(int addr) const { return mem[addr]; }

  void write(int addr, unsigned int val) { mem[addr] = val; }

private:
  unsigned int mem[64]; // 64 ячейки по 32 бита
};

#endif // MEMORY_H
