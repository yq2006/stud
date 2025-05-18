// #include "mainwindow.h"
// #include "./ui_mainwindow.h"
// #include <QFile>
// #include <QTextStream>
// #include <QMessageBox>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
//     , ui(new Ui::MainWindow)
// {
//     ui->setupUi(this);
//     model = new QStandardItemModel(this);
//     model->setHorizontalHeaderLabels(QStringList() << "姓名"<<"年龄"<<"性别"<<"学号"<<"专业"<<"年级"<<"成绩");
//     ui->tableView->setModel(model);
//     // 从文件读取数据并显示在表格中
//     QFile file("students.txt");
//     if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         QTextStream in(&file);
//         while (!in.atEnd()) {
//             QString line = in.readLine();
//             QStringList fields = line.split(",");

//             if (fields.size() == 7) {
                QList<QStandardItem*> rowItems = {
                    new QStandardItem(fields[0]),
                    new QStandardItem(fields[1]),
                    new QStandardItem(fields[2]),
                    new QStandardItem(fields[3]),
                    new QStandardItem(fields[4]),
                    new QStandardItem(fields[5]),
                    new QStandardItem(QString::number(fields[6].toDouble()))  // 确保成绩是数字类型
                };
                model->appendRow(rowItems);
            }
        }
        file.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonAddStudent_clicked()
{

    QString studentID = ui->lineEditStudentID->text();
    QString name = ui->lineEditName->text();
    QString gender = ui->comboBoxGender->currentText();
    int age = ui->spinBoxAge->value();
    QString major = ui->lineEditMajor->text();  // 使用正确的对象名
    QString grade = ui->lineEditGrade->text();  // 使用正确的对象名
    double score = ui->doubleSpinBoxScore->value();


    // 将数据保存到文件
    QFile file("students.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << name << "," << age << "," << gender << "\n";
        file.close();
    }

    QList<QStandardItem*> rowItems = {
        new QStandardItem(studentID),
        new QStandardItem(name),
        new QStandardItem(gender),
        new QStandardItem(QString::number(age)),
        new QStandardItem(major),
        new QStandardItem(grade),
        new QStandardItem(QString::number(score))
    };

    model->appendRow(rowItems);

    // // 清空所有输入控件
    ui->lineEditStudentID->clear();
    ui->lineEditName->clear();
    ui->comboBoxGender->setCurrentIndex(0);
    ui->spinBoxAge->setValue(0);
    ui->lineEditMajor->clear();
    ui->lineEditGrade->clear();
    ui->doubleSpinBoxScore->setValue(0.0);
}


// void MainWindow::on_pushButtonDeleteStudent_clicked() {
//     QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
//     if (selectedRows.isEmpty()) {
//         QMessageBox::warning(this, "警告", "请先选择要删除的学生信息！");
//         return;
//     }

//     int row = selectedRows.first().row();
//     model->removeRow(row);
//     updateStudentsFile();
// }

void MainWindow::updateStudentsFile() {
    QFile file("students.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList lines;
        while (!in.atEnd()) {
            QString line = in.readLine();
            lines.append(line);
        }
        file.close();

        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (int i = 0; i < lines.size(); ++i) {
                if (i != model->rowCount() - 1) {  // 跳过被删除的行
                    out << lines[i] << "\n";
                }
            }
            file.close();
        }
    }
}

