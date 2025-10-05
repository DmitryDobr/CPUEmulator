#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cpu/cpu.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
    void updatedCPU(int pCounter, QMap<int,int> updatedRegisters, QMap<int,int> updatedMemoryCells);

private:
  Ui::MainWindow *ui;

  CPU * cpu;
};

#endif // MAINWINDOW_H
