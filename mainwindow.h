#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override; // 添加关闭事件处理函数

private slots:
    void on_pushButtonAddStudent_clicked();
    void on_pushButtonDeleteStudent_clicked();
    void on_pushButtonModifyStudent_clicked();
    void on_pushButtonQueryStudent_clicked();
    void on_pushButtonSaveToFile_clicked();
    void on_pushButtonLoadFromFile_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    void saveToFile(const QString &fileName);
    void loadFromFile(const QString &fileName);
    void saveDataOnClose(); // 新增函数，用于在关闭时保存数据
};

#endif // MAINWINDOW_H
