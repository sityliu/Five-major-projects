/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "myslider.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QListWidget *video_list;
    MySlider *Slider_progress;
    QLabel *label;
    QPushButton *Play;
    QPushButton *Last;
    QPushButton *Next;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(900, 450);
        MainWindow->setMinimumSize(QSize(900, 450));
        MainWindow->setMaximumSize(QSize(900, 450));
        MainWindow->setStyleSheet(QString::fromUtf8("MainWindow{background-image: url(:/\346\235\220\346\226\231/GROV.jpg);}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 0, 600, 360));
        widget->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        video_list = new QListWidget(centralwidget);
        video_list->setObjectName(QStringLiteral("video_list"));
        video_list->setGeometry(QRect(600, 0, 300, 360));
        video_list->setStyleSheet(QString::fromUtf8("background-image: url(:/\346\235\220\346\226\231/GROV.jpg);"));
        Slider_progress = new MySlider(centralwidget);
        Slider_progress->setObjectName(QStringLiteral("Slider_progress"));
        Slider_progress->setGeometry(QRect(0, 360, 500, 20));
        Slider_progress->setOrientation(Qt::Horizontal);
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(520, 360, 81, 17));
        Play = new QPushButton(centralwidget);
        Play->setObjectName(QStringLiteral("Play"));
        Play->setGeometry(QRect(50, 380, 50, 50));
        Play->setStyleSheet(QStringLiteral(""));
        Play->setFlat(false);
        Last = new QPushButton(centralwidget);
        Last->setObjectName(QStringLiteral("Last"));
        Last->setGeometry(QRect(0, 380, 50, 50));
        Next = new QPushButton(centralwidget);
        Next->setObjectName(QStringLiteral("Next"));
        Next->setGeometry(QRect(100, 380, 50, 50));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CarPlayer", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "00:00/00:00", Q_NULLPTR));
        Play->setText(QString());
        Last->setText(QString());
        Next->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
