#include "StartMenu.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartMenu menu;
    MainWindow w;

    QObject::connect(&menu, &StartMenu::startPVP, [&](){
        w.initPVPGame();
        w.show();
    });

    QObject::connect(&menu, &StartMenu::startPVE, [&](){
        w.initPVEGame();
        w.show();
    });

    menu.show();
    return a.exec();
}
