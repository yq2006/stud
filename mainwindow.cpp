#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent> // 添加关闭事件头文件

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << "姓名" << "性别" << "年龄" << "学号" << "专业" << "年级" << "成绩");
    ui->tableView->setModel(model);

    // 加载数据
    loadFromFile("students.csv");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 添加关闭事件处理函数
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveDataOnClose();
    event->accept(); // 接受关闭事件
}

// 新增函数，用于在关闭时保存数据
void MainWindow::saveDataOnClose()
{
    saveToFile("students.csv");
}

void MainWindow::on_pushButtonAddStudent_clicked()
{
    QString name = ui->lineEditName->text();
    QString gender = ui->comboBoxGender->currentText();
    int age = ui->spinBoxAge->value();
    QString studentID = ui->lineEditStudentID->text();
    QString major = ui->lineEditMajor->text();
    QString grade = ui->lineEditGrade->text();
    double score = ui->doubleSpinBoxScore->value();

    QList<QStandardItem*> rowItems = {
        new QStandardItem(name),
        new QStandardItem(gender),
        new QStandardItem(QString::number(age)),
        new QStandardItem(studentID),
        new QStandardItem(major),
        new QStandardItem(grade),
        new QStandardItem(QString::number(score))
    };

    model->appendRow(rowItems);

    ui->lineEditName->clear();
    ui->comboBoxGender->setCurrentIndex(0);
    ui->spinBoxAge->setValue(0);
    ui->lineEditStudentID->clear();
    ui->lineEditMajor->clear();
    ui->lineEditGrade->clear();
    ui->doubleSpinBoxScore->setValue(0.0);
}

void MainWindow::on_pushButtonDeleteStudent_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的学生信息！");
        return;
    }

    foreach (QModelIndex index, selectedRows) {
        int row = index.row();
        model->removeRow(row);
    }
}

void MainWindow::on_pushButtonModifyStudent_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要修改的学生信息！");
        return;
    }

    int row = selectedRows.first().row();
    QString name = model->data(model->index(row, 0)).toString();
    QString gender = model->data(model->index(row, 1)).toString();
    int age = model->data(model->index(row, 2)).toInt();
    QString studentID = model->data(model->index(row, 3)).toString();
    QString major = model->data(model->index(row, 4)).toString();
    QString grade = model->data(model->index(row, 5)).toString();
    double score = model->data(model->index(row, 6)).toDouble();

    ui->lineEditName->setText(name);
    ui->comboBoxGender->setCurrentText(gender);
    ui->spinBoxAge->setValue(age);
    ui->lineEditStudentID->setText(studentID);
    ui->lineEditMajor->setText(major);
    ui->lineEditGrade->setText(grade);
    ui->doubleSpinBoxScore->setValue(score);
}

void MainWindow::on_pushButtonQueryStudent_clicked()
{
    QString name = ui->lineEditName->text();
    QString gender = ui->comboBoxGender->currentText();
    int age = ui->spinBoxAge->value();
    QString studentID = ui->lineEditStudentID->text();
    QString major = ui->lineEditMajor->text();
    QString grade = ui->lineEditGrade->text();

    QString filter;
    if (!name.isEmpty()) {
        filter += QString("name LIKE '%%1%'").arg(name);
    }
    if (!gender.isEmpty()) {
        if (!filter.isEmpty()) filter += " AND ";
        filter += QString("gender LIKE '%%1%'").arg(gender);
    }
    if (age > 0) {
        if (!filter.isEmpty()) filter += " AND ";
        filter += QString("age == %1").arg(age);
    }
    if (!studentID.isEmpty()) {
        if (!filter.isEmpty()) filter += " AND ";
        filter += QString("studentID LIKE '%%1%'").arg(studentID);
    }
    if (!major.isEmpty()) {
        if (!filter.isEmpty()) filter += " AND ";
        filter += QString("major LIKE '%%1%'").arg(major);
    }
    if (!grade.isEmpty()) {
        if (!filter.isEmpty()) filter += " AND ";
        filter += QString("grade LIKE '%%1%'").arg(grade);
    }

    // if (!filter.isEmpty()) {
    //     proxyModel->setFilterRegExp(QRegExp(filter, Qt::CaseInsensitive, QRegExp::Wildcard));
    // } else {
    //     proxyModel->setFilterRegExp(QRegExp());
    // }
}
void MainWindow::on_pushButtonSaveToFile_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "", "CSV Files (*.csv)");
    if (!fileName.isEmpty()) {
        saveToFile(fileName);
        QMessageBox::information(this, "成功", "数据已保存到文件！");
    }
}

void MainWindow::on_pushButtonLoadFromFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "CSV Files (*.csv)");
    if (!fileName.isEmpty()) {
        loadFromFile(fileName);
        QMessageBox::information(this, "成功", "数据已从文件加载！");
    }
}


void MainWindow::saveToFile(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // 写入表头
        out << "姓名,性别,年龄,学号,专业,年级,成绩\n";
        // 写入数据
        for (int row = 0; row < model->rowCount(); ++row) {
            QStringList fields;
            for (int col = 0; col < model->columnCount(); ++col) {
                fields.append(model->data(model->index(row, col)).toString());
            }
            out << fields.join(",") << "\n";
        }
        file.close();
    }
}

void MainWindow::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // 读取表头
        in.readLine();
        // 读取数据
        model->clear();
        model->setHorizontalHeaderLabels(QStringList() << "姓名" << "性别" << "年龄" << "学号" << "专业" << "年级" << "成绩");
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields.size() == 7) {
                QList<QStandardItem*> rowItems = {
                    new QStandardItem(fields[0]),
                    new QStandardItem(fields[1]),
                    new QStandardItem(fields[2]),
                    new QStandardItem(fields[3]),
                    new QStandardItem(fields[4]),
                    new QStandardItem(fields[5]),
                    new QStandardItem(fields[6])
                };
                model->appendRow(rowItems);
            }
        }
        file.close();
    }
}
