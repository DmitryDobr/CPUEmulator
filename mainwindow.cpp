#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    cpu = new CPU(this); // инициализация процессора

    // настройка интерфейса
    ui->tw_mem->setRowCount(64);
    QStringList l;
    for (int i = 0; i < 64; i++){
        l.append("0x"+QString("%1").arg(i, 4, 16, QChar('0')));
        QTableWidgetItem * ti = new QTableWidgetItem;
        ti->setText(QString("%1").arg(0, 8, 16, QChar('0')));
        ti->setFlags(ti->flags() & 0xfffffffd);
        ti->setTextAlignment(Qt::AlignCenter);
        ui->tw_mem->setItem(i,0,ti);
    }
    ui->tw_mem->setVerticalHeaderLabels(l);

    ui->tw_reg->setRowCount(16);
    for (int i = 0; i < 16; i++) {
        QTableWidgetItem * ti = new QTableWidgetItem;
        ti->setText(QString("%1").arg(0, 8, 16, QChar('0')));
        ti->setFlags(ti->flags() & 0xfffffffd);
        ti->setTextAlignment(Qt::AlignCenter);
        ui->tw_reg->setItem(i,0,ti);
    }

    for (int i = 0; i < ui->tw_flag->rowCount(); i++) {
        QCheckBox * chb = new QCheckBox(this);
        chb->setStyleSheet("padding-left: 50%;");
        chb->setChecked(true);
        ui->tw_flag->setCellWidget(i,1,chb);
    }



    connect(cpu, SIGNAL(updateCPU(unsigned int, int, unsigned int, unsigned int)), this, SLOT(updatedCPU(unsigned int, int, unsigned int, unsigned int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updatedCPU(unsigned int pCounter, int FlagUpdated, unsigned int addr, unsigned int val) {

    // обновили регистр
    if (FlagUpdated == 1) {
        ui->tw_reg->item(addr,0)->setText(QString::number(val));
    }
    // обновили ячейку памяти
    else if (FlagUpdated == 2) {
        ui->tw_mem->item(addr,0)->setText(QString::number(val));
    }

    ui->le_pc->setText(QString::number(pCounter));
    ui->tw_mem->setCurrentCell(pCounter,0);
}
