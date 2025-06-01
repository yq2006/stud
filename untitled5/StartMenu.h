#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>

class StartMenu : public QWidget
{
    Q_OBJECT
public:
        // explicit
    StartMenu(QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event);
signals:
    void startPVP();
    void startPVE();
    void showHistory();

private slots:
    void clearHistory();

private:
    void loadHistory();
    QPushButton *clearBtn;

    QListWidget *historyList;


};

#endif
