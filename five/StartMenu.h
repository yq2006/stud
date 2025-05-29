#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QCloseEvent>
#include "ui_StartMenu.h"

class StartMenu : public QWidget
{
    Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);

signals:
    void closeEvent(QCloseEvent *event);

public slots:
    void startPVP();
    void startPVE();
    void showHistory();
    void clearHistory();

private slots:
    void loadHistory();

    void on_pvpBtn_clicked();

    void on_pveBtn_clicked();

    void on_historyBtn_clicked();

    void on_clearBtn_clicked();

private:
    Ui::StartMenu ui;
    QListWidget *historyList;
    QPushButton *clearBtn;
};

#endif // STARTMENU_H
