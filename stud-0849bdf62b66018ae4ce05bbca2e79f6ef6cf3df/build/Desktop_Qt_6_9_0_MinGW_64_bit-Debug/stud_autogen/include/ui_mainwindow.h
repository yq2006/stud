/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTableView *tableView;
    QGroupBox *groupBoxStudentInfo;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelName;
    QLineEdit *lineEditName;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelAge;
    QSpinBox *spinBoxAge;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelGender;
    QComboBox *comboBoxGender;
    QPushButton *pushButtonAddStudent;
    QWidget *widget2;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(100, 0, 351, 191));
        groupBoxStudentInfo = new QGroupBox(centralwidget);
        groupBoxStudentInfo->setObjectName("groupBoxStudentInfo");
        groupBoxStudentInfo->setGeometry(QRect(80, 220, 281, 141));
        horizontalLayoutWidget = new QWidget(groupBoxStudentInfo);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(30, 0, 171, 41));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        labelName = new QLabel(horizontalLayoutWidget);
        labelName->setObjectName("labelName");
        labelName->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(labelName);

        lineEditName = new QLineEdit(horizontalLayoutWidget);
        lineEditName->setObjectName("lineEditName");

        horizontalLayout_2->addWidget(lineEditName);

        widget = new QWidget(groupBoxStudentInfo);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(40, 40, 141, 41));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        labelAge = new QLabel(widget);
        labelAge->setObjectName("labelAge");
        labelAge->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_3->addWidget(labelAge);

        spinBoxAge = new QSpinBox(widget);
        spinBoxAge->setObjectName("spinBoxAge");

        horizontalLayout_3->addWidget(spinBoxAge);

        widget1 = new QWidget(groupBoxStudentInfo);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(50, 90, 106, 21));
        horizontalLayout_4 = new QHBoxLayout(widget1);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        labelGender = new QLabel(widget1);
        labelGender->setObjectName("labelGender");

        horizontalLayout_4->addWidget(labelGender);

        comboBoxGender = new QComboBox(widget1);
        comboBoxGender->addItem(QString());
        comboBoxGender->addItem(QString());
        comboBoxGender->setObjectName("comboBoxGender");

        horizontalLayout_4->addWidget(comboBoxGender);

        pushButtonAddStudent = new QPushButton(centralwidget);
        pushButtonAddStudent->setObjectName("pushButtonAddStudent");
        pushButtonAddStudent->setGeometry(QRect(410, 250, 56, 18));
        widget2 = new QWidget(centralwidget);
        widget2->setObjectName("widget2");
        widget2->setGeometry(QRect(0, 0, 2, 2));
        horizontalLayout = new QHBoxLayout(widget2);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBoxStudentInfo->setTitle(QCoreApplication::translate("MainWindow", "\345\255\246\347\224\237\344\277\241\346\201\257", nullptr));
        labelName->setText(QCoreApplication::translate("MainWindow", "\345\247\223\345\220\215", nullptr));
        labelAge->setText(QCoreApplication::translate("MainWindow", "\345\271\264\351\276\204", nullptr));
        labelGender->setText(QCoreApplication::translate("MainWindow", "\346\200\247\345\210\253", nullptr));
        comboBoxGender->setItemText(0, QCoreApplication::translate("MainWindow", "\347\224\267", nullptr));
        comboBoxGender->setItemText(1, QCoreApplication::translate("MainWindow", "\345\245\263", nullptr));

        pushButtonAddStudent->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\255\246\347\224\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
