#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    cpu = new CPU(this); // инициализация процессора
    unsigned int * val = (unsigned int *)cpu->memory()->dataPtr(); // при инициализации в память уже что-то внесено (пока так)

    // настройка интерфейса
    ui->tw_mem->setRowCount(64); // заголовки строк
    QStringList l;
    for (int i = 0; i < 64; i++){
        l.append("0x"+QString("%1").arg(i, 3, 16, QChar('0')));
        QTableWidgetItem * ti = new QTableWidgetItem;

        ti->setText(QString("%1").arg(*val, 12, 10, QChar('0')));
        ti->setFlags(ti->flags() & 0xfffffffd);
        ti->setTextAlignment(Qt::AlignCenter);
        ui->tw_mem->setItem(i,0,ti);

        val = val + 1;
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
        chb->setChecked(false);
        ui->tw_flag->setCellWidget(i,1,chb);
    }

    ui->tw_mem->setCurrentCell(0,0);

    connect(cpu, SIGNAL(updateCPU(unsigned int)), this, SLOT(on_updatedCPU(unsigned int)));
    connect(cpu, SIGNAL(registerUpdated(unsigned int, unsigned int)), this, SLOT(on_registerUpdated(unsigned int, unsigned int)));
    connect(cpu, SIGNAL(memoryCellUpdated(unsigned int, unsigned int)), this, SLOT(on_memoryCellUpdated(unsigned int, unsigned int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_updatedCPU(unsigned int pCounter) {
    ui->le_pc->setText(QString::number(pCounter));
    ui->tw_mem->setCurrentCell(pCounter,0);

    // обновляем флаги в интерфейсе
    QCheckBox * chb;
    chb = (QCheckBox *)(ui->tw_flag->cellWidget(0, 1));
    if (chb)
        chb->setChecked(cpu->flags() & CPUNameSpace::SignFlag);

    chb = (QCheckBox *)(ui->tw_flag->cellWidget(1, 1));
    if (chb)
        chb->setChecked(cpu->flags() & CPUNameSpace::ZeroFlag);

    chb = (QCheckBox *)(ui->tw_flag->cellWidget(2, 1));
    if (chb)
        chb->setChecked(cpu->flags() & CPUNameSpace::CarryFlag);
}

void MainWindow::on_registerUpdated(unsigned int reg, unsigned int val) {
    ui->tw_reg->item(reg,0)->setText(QString::number(val));
}

void MainWindow::on_memoryCellUpdated(unsigned int addr, unsigned int val) {
    ui->tw_mem->item(addr,0)->setText(QString::number(val));
}
