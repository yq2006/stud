#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << "姓名"<<"年龄"<<"性别");
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonAddStudent_clicked()
{

        QString name = ui->lineEditName->text();
    int age = ui->spinBoxAge->value();
    QString gender = ui->comboBoxGender->currentText();

    QList<QStandardItem*> rowItems = {
        new QStandardItem(name),
        new QStandardItem(QString::number(age)),
        new QStandardItem(gender)
    };

    model->appendRow(rowItems);

    // 清空所有输入控件
    ui->lineEditName->clear();       // 清空姓名输入框
    ui->spinBoxAge->setValue(0);     // 重置年龄为0
    ui->comboBoxGender->setCurrentIndex(0); // 重置性别为第一个选项
}

