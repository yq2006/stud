#include "StartMenu.h"
#include "qevent.h"
#include "qmessagebox.h"
#include <QFile>
#include <QTextStream>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *pvpBtn = new QPushButton("双人对战");
    QPushButton *pveBtn = new QPushButton("人机对战");
    QPushButton *historyBtn = new QPushButton("历史记录");
    clearBtn = new QPushButton("清除记录");
    historyList = new QListWidget();

    layout->addWidget(pvpBtn);
    layout->addWidget(pveBtn);
    layout->addWidget(historyBtn);
    layout->addWidget(historyList);
    layout->addWidget(clearBtn);

    connect(clearBtn, &QPushButton::clicked,this, &StartMenu::clearHistory);
    connect(pvpBtn, &QPushButton::clicked, this, &StartMenu::startPVP);
    connect(pveBtn, &QPushButton::clicked, this, &StartMenu::startPVE);
    connect(historyBtn, &QPushButton::clicked, this, &StartMenu::loadHistory);


    loadHistory();
}

void StartMenu::clearHistory()
{
    QFile::remove("history.log");  // 删除记录文件
    historyList->clear();
}

void StartMenu::closeEvent(QCloseEvent *event) {
    hide(); // 隐藏窗口
    event->ignore(); // 阻止默认关闭行为
}

void StartMenu::loadHistory() {
    historyList->clear();

    QFile file("history.log");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);



    file.close();
}
