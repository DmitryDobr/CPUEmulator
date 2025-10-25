#ifndef MEMORY_H
#define MEMORY_H

#include <QObject>

class Memory : public QObject
{
  Q_OBJECT

public:
  explicit Memory(QObject *parent = nullptr);

  unsigned int read(unsigned int addr) const;

  void write(unsigned int addr, unsigned int val);

  void clear();

  void* dataPtr() const { return (void*)mem;}

signals:
  void cellUpdated(unsigned int addr, unsigned int val);

private:
  unsigned int mem[64]; // 64 ячейки по 32 бита
};

#endif // MEMORY_H
