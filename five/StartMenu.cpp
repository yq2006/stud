#include "StartMenu.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
}

void StartMenu::on_pvpBtn_clicked()
{
    startPVP(); // 调用 startPVP 函数
}

void StartMenu::on_pveBtn_clicked()
{
    startPVE(); // 调用 startPVE 函数
}

void StartMenu::on_historyBtn_clicked()
{
    showHistory(); // 调用 showHistory 函数
}

void StartMenu::on_clearBtn_clicked()
{
    clearHistory(); // 调用 clearHistory 函数
}

void StartMenu::startPVP()
{

}

void StartMenu::startPVE()
{

}

void StartMenu::showHistory()
{
    loadHistory();
}

void StartMenu::clearHistory()
{
    ui.historyList->clear();
    QFile historyFile("history.log");
    if (historyFile.exists())
    {
        historyFile.remove();
    }

}

void StartMenu::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void StartMenu::loadHistory()
{
    QFile file("history.log");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    ui.historyList->clear();

    while (!in.atEnd())
    {
        QString line = in.readLine();
        ui.historyList->addItem(line);
    }
    file.close();
}


