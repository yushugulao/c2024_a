/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *buttonUndo;
    QPushButton *buttonReset;
    QPushButton *buttonAIFirst;
    QPushButton *buttonUserFirst;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(786, 562);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        buttonUndo = new QPushButton(centralwidget);
        buttonUndo->setObjectName("buttonUndo");
        buttonUndo->setGeometry(QRect(610, 280, 181, 111));
        buttonUndo->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 19, 102);"));
        buttonUndo->setAutoDefault(false);
        buttonReset = new QPushButton(centralwidget);
        buttonReset->setObjectName("buttonReset");
        buttonReset->setGeometry(QRect(610, 390, 181, 121));
        buttonAIFirst = new QPushButton(centralwidget);
        buttonAIFirst->setObjectName("buttonAIFirst");
        buttonAIFirst->setGeometry(QRect(610, 20, 171, 111));
        buttonUserFirst = new QPushButton(centralwidget);
        buttonUserFirst->setObjectName("buttonUserFirst");
        buttonUserFirst->setGeometry(QRect(610, 130, 171, 111));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 786, 25));
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
        buttonUndo->setText(QCoreApplication::translate("MainWindow", "\346\202\224\346\243\213", nullptr));
        buttonReset->setText(QCoreApplication::translate("MainWindow", "\351\207\215\346\226\260\345\274\200\345\247\213", nullptr));
        buttonAIFirst->setText(QCoreApplication::translate("MainWindow", "\347\224\265\350\204\221\345\205\210\346\211\213", nullptr));
        buttonUserFirst->setText(QCoreApplication::translate("MainWindow", "\347\216\251\345\256\266\345\205\210\346\211\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
