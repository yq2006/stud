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
#include <QtWidgets/QDoubleSpinBox>
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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTableView *tableView;
    QGroupBox *groupBoxStudentInfo;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelName;
    QLineEdit *lineEditName;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelAge;
    QSpinBox *spinBoxAge;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelGender;
    QComboBox *comboBoxGender;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QLineEdit *lineEditStudentID;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QLineEdit *lineEditMajor;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_3;
    QLineEdit *lineEditGrade;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBoxScore;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButtonAddStudent;
    QPushButton *pushButtonDeleteStudent;
    QPushButton *pushButtonModifyStudent;
    QPushButton *pushButtonQueryStudent;
    QPushButton *pushButtonSaveToFile;
    QPushButton *pushButtonLoadFromFile;
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
        tableView->setGeometry(QRect(0, 20, 731, 251));
        tableView->setMinimumSize(QSize(730, 0));
        groupBoxStudentInfo = new QGroupBox(centralwidget);
        groupBoxStudentInfo->setObjectName("groupBoxStudentInfo");
        groupBoxStudentInfo->setGeometry(QRect(60, 280, 171, 221));
        layoutWidget = new QWidget(groupBoxStudentInfo);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 20, 140, 173));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        labelName = new QLabel(layoutWidget);
        labelName->setObjectName("labelName");
        labelName->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(labelName);

        lineEditName = new QLineEdit(layoutWidget);
        lineEditName->setObjectName("lineEditName");

        horizontalLayout_2->addWidget(lineEditName);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        labelAge = new QLabel(layoutWidget);
        labelAge->setObjectName("labelAge");
        labelAge->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(labelAge);

        spinBoxAge = new QSpinBox(layoutWidget);
        spinBoxAge->setObjectName("spinBoxAge");

        horizontalLayout_3->addWidget(spinBoxAge);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        labelGender = new QLabel(layoutWidget);
        labelGender->setObjectName("labelGender");

        horizontalLayout_4->addWidget(labelGender);

        comboBoxGender = new QComboBox(layoutWidget);
        comboBoxGender->addItem(QString());
        comboBoxGender->addItem(QString());
        comboBoxGender->setObjectName("comboBoxGender");

        horizontalLayout_4->addWidget(comboBoxGender);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label = new QLabel(layoutWidget);
        label->setObjectName("label");

        horizontalLayout_5->addWidget(label);

        lineEditStudentID = new QLineEdit(layoutWidget);
        lineEditStudentID->setObjectName("lineEditStudentID");

        horizontalLayout_5->addWidget(lineEditStudentID);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");

        horizontalLayout_6->addWidget(label_2);

        lineEditMajor = new QLineEdit(layoutWidget);
        lineEditMajor->setObjectName("lineEditMajor");

        horizontalLayout_6->addWidget(lineEditMajor);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName("label_3");

        horizontalLayout_7->addWidget(label_3);

        lineEditGrade = new QLineEdit(layoutWidget);
        lineEditGrade->setObjectName("lineEditGrade");

        horizontalLayout_7->addWidget(lineEditGrade);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName("label_4");

        horizontalLayout_8->addWidget(label_4);

        doubleSpinBoxScore = new QDoubleSpinBox(layoutWidget);
        doubleSpinBoxScore->setObjectName("doubleSpinBoxScore");

        horizontalLayout_8->addWidget(doubleSpinBoxScore);


        verticalLayout->addLayout(horizontalLayout_8);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(0, 0, 2, 2));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        layoutWidget2 = new QWidget(centralwidget);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(300, 320, 71, 151));
        verticalLayout_2 = new QVBoxLayout(layoutWidget2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButtonAddStudent = new QPushButton(layoutWidget2);
        pushButtonAddStudent->setObjectName("pushButtonAddStudent");

        verticalLayout_2->addWidget(pushButtonAddStudent);

        pushButtonDeleteStudent = new QPushButton(layoutWidget2);
        pushButtonDeleteStudent->setObjectName("pushButtonDeleteStudent");

        verticalLayout_2->addWidget(pushButtonDeleteStudent);

        pushButtonModifyStudent = new QPushButton(layoutWidget2);
        pushButtonModifyStudent->setObjectName("pushButtonModifyStudent");

        verticalLayout_2->addWidget(pushButtonModifyStudent);

        pushButtonQueryStudent = new QPushButton(layoutWidget2);
        pushButtonQueryStudent->setObjectName("pushButtonQueryStudent");

        verticalLayout_2->addWidget(pushButtonQueryStudent);

        pushButtonSaveToFile = new QPushButton(layoutWidget2);
        pushButtonSaveToFile->setObjectName("pushButtonSaveToFile");

        verticalLayout_2->addWidget(pushButtonSaveToFile);

        pushButtonLoadFromFile = new QPushButton(layoutWidget2);
        pushButtonLoadFromFile->setObjectName("pushButtonLoadFromFile");

        verticalLayout_2->addWidget(pushButtonLoadFromFile);

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

        label->setText(QCoreApplication::translate("MainWindow", "\345\255\246\345\217\267", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\344\270\223\344\270\232", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\345\271\264\347\272\247", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\346\210\220\347\273\251", nullptr));
        pushButtonAddStudent->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\255\246\347\224\237", nullptr));
        pushButtonDeleteStudent->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\345\255\246\347\224\237", nullptr));
        pushButtonModifyStudent->setText(QCoreApplication::translate("MainWindow", "\344\277\256\346\224\271\345\255\246\347\224\237", nullptr));
        pushButtonQueryStudent->setText(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242\345\255\246\347\224\237", nullptr));
        pushButtonSaveToFile->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\345\210\260\346\226\207\344\273\266", nullptr));
        pushButtonLoadFromFile->setText(QCoreApplication::translate("MainWindow", "\344\273\216\346\226\207\344\273\266\345\212\240\350\275\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
