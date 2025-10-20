#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cpu/cpu.h"
#include "translator/asmtranslator.h"

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

    void on_pushButton_play_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_filePick_clicked();

    void on_pushButton_execAsm_clicked();

    void on_pushButton_editAsm_clicked();

private:
  Ui::MainWindow *ui;

  CPU * cpu;
  AsmTranslator translator;
};

#endif // MAINWINDOW_H
