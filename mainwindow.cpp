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

    for (int i = 0; i < ui->tw_flag->rowCount(); i++) {
        QCheckBox * chb = new QCheckBox(this);
        chb->setStyleSheet("padding-left: 50%;");
        chb->setChecked(true);
        ui->tw_flag->setCellWidget(i,1,chb);
    }

    connect(cpu, SIGNAL(updateCPU(int, bool, int, unsigned int)), this, SLOT(updatedCPU(int, bool, int, unsigned int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updatedCPU(int pCounter, int regUpdated, int num, unsigned int val) {

    ui->le_pc->setText(QString::number(pCounter));
    ui->tw_mem->setCurrentCell(pCounter,0);
}
