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
    void on_updatedCPU(unsigned int pCounter);

    void on_registerUpdated(unsigned int reg, unsigned int val);

    void on_memoryCellUpdated(unsigned int addr, unsigned int val);

private:
  Ui::MainWindow *ui;

  CPU * cpu;
};

#endif // MAINWINDOW_H
