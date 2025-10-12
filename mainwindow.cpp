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
    ui->tw_mem->setRowCount(16);
    ui->tw_mem->setColumnCount(CPUNameSpace::MEMORY_SIZE/16);

    QStringList rowLabels; // заголовки строк
    for (int i = 0; i < 16; i++)
        rowLabels.append(QString("%1").arg(i, 1, 16));

    QStringList columnLabels; // заголовки столбов
    int column = 0;
    int row = 0;
    for (int i = 0; i < CPUNameSpace::MEMORY_SIZE; i++) {
        QTableWidgetItem * ti = new QTableWidgetItem;

        ti->setText(QString("%1").arg(*val, 10, 16, QChar('0')));
        ti->setFlags(ti->flags() & 0xfffffffd);
        ti->setTextAlignment(Qt::AlignCenter);


        unsigned int command     = (*val >> CPUNameSpace::COMMAND_OFFSET);
        unsigned int operand1    = (*val >> CPUNameSpace::OPERAND1_OFFSET) & CPUNameSpace::OPERAND_MASK;
        unsigned int operand2    = (*val >> CPUNameSpace::OPERAND2_OFFSET) & CPUNameSpace::OPERAND_MASK;
        unsigned int literal     = (*val >> CPUNameSpace::LITERAL_OFFSET)  & CPUNameSpace::LITERAL_MASK;
        unsigned int modificator = *val & CPUNameSpace::MODIFICATOR_MASK;

        if (*val > 0x8000000)
            ti->setToolTip("код команды : " + QString::number(command) + "\n" +
                           "операнд 1   : " + QString::number(operand1) + "\n" +
                           "операнд 2   : " + QString::number(operand2) + "\n" +
                           "литерал     : " + QString::number(literal) + "\n" +
                           "модификатор : " + QString::number(modificator)
                           );

        ui->tw_mem->setItem(row,column,ti);

        val = val + 1;

        if (row == 15) {
            QString columnLab = QString("0x%1").arg(i, 3, 16, QChar('0'));
            columnLab.chop(1);
            columnLabels.append(columnLab);

            row = 0;
            column++;
        }
        else
            row++;
    }
    ui->tw_mem->setHorizontalHeaderLabels(columnLabels);
    ui->tw_mem->setVerticalHeaderLabels(rowLabels);



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

    int column = pCounter / 16;
    int row = pCounter % 16;
    ui->tw_mem->setCurrentCell(row,column);

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
    int column = addr / 16;
    int row = addr % 16;
    ui->tw_mem->item(row,column)->setText(QString::number(val));
}
