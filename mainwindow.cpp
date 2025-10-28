#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QCheckBox>

#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    cpu = new CPU(this); // инициализация процессора

    // таблица ячеек памяти
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

        ti->setText(QString("0x%1").arg(0, 10, 16, QChar('0')));
        ti->setFlags(ti->flags() & 0xfffffffd);
        ti->setTextAlignment(Qt::AlignCenter);

        ui->tw_mem->setItem(row,column,ti);

        if (row == 15) {
            QString columnLab = QString("0x%1").arg(i, 2, 16, QChar('0'));
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
    ui->tw_mem->setCurrentCell(0,0);

    // таблица регистров
    rowLabels.clear();
    ui->tw_reg->setRowCount(16);
    for (int i = 0; i < 16; i++) {
        rowLabels.append(QString("REG%1(%2)").arg(i).arg(char(65+i)));

        QTableWidgetItem * ti = new QTableWidgetItem;
        ti->setText("0");
        ti->setFlags(ti->flags() & 0xfffffffd);
        ti->setTextAlignment(Qt::AlignCenter);
        ui->tw_reg->setItem(i,0,ti);
    }
    ui->tw_reg->setVerticalHeaderLabels(rowLabels);

    // таблица флагов
    for (int i = 0; i < ui->tw_flag->rowCount(); i++) {
        QCheckBox * chb = new QCheckBox(this);
        chb->setStyleSheet("padding-left: 50%;");
        chb->setChecked(false);
        ui->tw_flag->setCellWidget(i,1,chb);
    }

    // добавление иконок на кнопки
    QIcon ButtonIcon;
    ButtonIcon = QApplication::style()->standardIcon(QStyle::SP_MediaPlay);
    ui->pushButton_play->setIcon(ButtonIcon);
    ui->pushButton_play->setIconSize(QSize(40,40));

    ButtonIcon = QApplication::style()->standardIcon(QStyle::SP_MediaPause);
    ui->pushButton_pause->setIcon(ButtonIcon);
    ui->pushButton_pause->setIconSize(QSize(40,40));

    // соединения сигналов процессора с обработчиками
    connect(cpu, SIGNAL(updateCPU(unsigned int)), this, SLOT(on_updatedCPU(unsigned int)));
    connect(cpu, SIGNAL(registerUpdated(unsigned int, unsigned int)), this, SLOT(on_registerUpdated(unsigned int, unsigned int)));
    connect(cpu, SIGNAL(memoryCellUpdated(unsigned int, unsigned int)), this, SLOT(on_memoryCellUpdated(unsigned int, unsigned int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

// обновление состояние процессора
void MainWindow::on_updatedCPU(unsigned int pCounter) {
    ui->le_pc->setText(QString::number(pCounter));

    int column = pCounter / 16;
    int row = pCounter % 16;
    ui->tw_mem->setCurrentCell(row,column);

    // если находимся в конце памяти - процессор закончил своб работу
    if (pCounter == 63) {
        ui->pushButton_pause->setEnabled(false);
        ui->pushButton_play->setEnabled(false);
    }

    // обновляем флаги в интерфейсе
    QCheckBox * chb;
    chb = dynamic_cast<QCheckBox *>(ui->tw_flag->cellWidget(0, 1));
    if (chb)
        chb->setChecked(cpu->flags() & CPUNameSpace::SignFlag);

    chb = dynamic_cast<QCheckBox *>(ui->tw_flag->cellWidget(1, 1));
    if (chb)
        chb->setChecked(cpu->flags() & CPUNameSpace::ZeroFlag);

    chb = dynamic_cast<QCheckBox *>(ui->tw_flag->cellWidget(2, 1));
    if (chb)
        chb->setChecked(cpu->flags() & CPUNameSpace::CarryFlag);
}

// если в процессе был обновлен регистр
void MainWindow::on_registerUpdated(unsigned int reg, unsigned int val) {
    ui->tw_reg->item(static_cast<int>(reg),0)->setText(QString::number(static_cast<int>(val)));
    ui->tw_reg->setCurrentCell(static_cast<int>(reg),0);
}

// если в процессе была обновлена ячейка памяти
void MainWindow::on_memoryCellUpdated(unsigned int addr, unsigned int val) {
    int column = addr / 16;
    int row = addr % 16;
    ui->tw_mem->item(row,column)->setText(QString::number(static_cast<int>(val)));
}

// --------------------------------------------------------------------------
// возобновить выполнение процессора
void MainWindow::on_pushButton_play_clicked() {
    cpu->setPlaying(true);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_play->setEnabled(false);
}

// пауза выполнения процессора
void MainWindow::on_pushButton_pause_clicked() {
    cpu->setPlaying(false);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_play->setEnabled(true);
}

// --------------------------------------------------------------------------
// выбор файла
void MainWindow::on_pushButton_filePick_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл");
    ui->lineEdit_asmFile->setText(filePath);
}

// транслировать ассемблер код в память процессора
void MainWindow::on_pushButton_execAsm_clicked() {

    on_pushButton_pause_clicked(); // останавливаем текущее выполнение

    QString filePath = ui->lineEdit_asmFile->text();
    QFile file(filePath);
    QString content;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        content = stream.readAll();
        file.close();
    }

    cpu->resetCPU();
    if (translator.translate(content, cpu->memory())) {

        unsigned int * val = cpu->memory()->dataPtr(); // при инициализации в память уже что-то внесено (пока так)
        // обновляем таблицу памяти в ui
        int column = 0;
        int row = 0;
        for (int i = 0; i < CPUNameSpace::MEMORY_SIZE; i++) {
            // первая половина памяти - данные, вторая половина памяти - коды команд
            if (i >= CPUNameSpace::MEMORY_SIZE / 2) {

                ui->tw_mem->item(row,column)->setText(QString("0x%1").arg(*val, 10, 16, QChar('0')));
                // заполняем текстовую подсказку
                unsigned int literal = (*val >> CPUNameSpace::LITERAL_OFFSET) & CPUNameSpace::LITERAL_MASK;
                int litVal = 0;
                if (literal & 1024)
                    litVal = -(literal & 1023);
                else
                    litVal = literal & 1023;

                ui->tw_mem->item(row,column)->setToolTip(
                               "код команды  : " + QString::number((*val >> CPUNameSpace::COMMAND_OFFSET)) + "\n" +
                               "операнд 1       : " + QString::number(((*val >> CPUNameSpace::OPERAND1_OFFSET) & CPUNameSpace::OPERAND_MASK)) + "\n" +
                               "операнд 2       : " + QString::number(((*val >> CPUNameSpace::OPERAND2_OFFSET) & CPUNameSpace::OPERAND_MASK)) + "\n" +
                               "литерал           : " + QString::number(litVal) + "\n" +
                               "мод~oр           : " + QString::number((*val & CPUNameSpace::MODIFICATOR_MASK)) );
            }
            else {
                 ui->tw_mem->item(row,column)->setText(QString::number(static_cast<int>(*val)));
            }

            val = val + 1; // переходим к след.значению в памяти
            if (row == 15) {
                row = 0;
                column++;
            }
            else
                row++;
        }
    }
    else {
        QMessageBox::warning(this,"Ошибка чтения", translator.lastError());
    }

    // обнулить таблицу регистров
    for (int i = 0; i < 16; i++)
        ui->tw_reg->item(i,0)->setText("0");

    // подсвечивать ячейки памяти
    on_checkBox_clicked(ui->checkBox->isChecked());
}

// вызвать системный редактор
void MainWindow::on_pushButton_editAsm_clicked() {
    QString filePath = ui->lineEdit_asmFile->text();
    if (!filePath.isEmpty())
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

// --------------------------------------------------------------------------
// генерации палитры
QVector<QColor> getColors(int n) {
    QVector<QColor> colors;
    colors.reserve(n);

    for (int i = 0; i < n; ++i) {
        // Равномерное распределение оттенков
        double hue = static_cast<double>(i) / n;

        // Фиксированные параметры для нежных цветов
        double saturation = 0.4;  // Умеренная насыщенность
        double lightness = 0.75;   // Высокая светлота для нежности

        QColor color = QColor::fromHslF(hue, saturation, lightness);
        colors.append(color);
    }

    return colors;
}

// покрасить ячейки памяти чтоб было видно секции и данные
void MainWindow::highlightMemoryCells() {

    QVector<int> sections = translator.getSectionsEnd();
    QVector<QColor> map = getColors(sections.length());

    int column = 0;
    int row = 0;
    int c = 0;
    for (int i = 0; i < CPUNameSpace::MEMORY_SIZE; i++) {
        if (i >= sections[c]) // переход к след.секции
            c += 1;

        ui->tw_mem->item(row,column)->setBackgroundColor(map[c]);

        if (row == 15) {
            row = 0;
            column++;
        }
        else
            row++;
    }
}

// вернуть ячейкам в таблице памяти белый цвет
void MainWindow::disHighlightMemoryCells() {
    for (int i = 0; i < ui->tw_mem->rowCount(); i++) {
        for (int j = 0; j < ui->tw_mem->columnCount(); j++)
            ui->tw_mem->item(i,j)->setBackgroundColor(Qt::white);
    }
}

// реагируем на чекбокс изменения подсвечивания ячеек памяти
void MainWindow::on_checkBox_clicked(bool checked) {
    if (checked && translator.isLoad())
        highlightMemoryCells();
    else
        disHighlightMemoryCells();
}
