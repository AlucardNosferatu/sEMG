/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QCustomPlot *customPlot;
    QLineEdit *lineEdit_channel;
    QCustomPlot *customPlot0;
    QLineEdit *lineEdit_channel1;
    QScrollBar *replayBar;
    QPushButton *pushButton_openFile;
    QPushButton *pushButton_play;
    QLabel *label_Icon;
    QCustomPlot *customPlot1;
    QCustomPlot *customPlot2;
    QCustomPlot *customPlot3;
    QCustomPlot *customPlot4;
    QCustomPlot *customPlot5;
    QCustomPlot *customPlot6;
    QCustomPlot *customPlot7;
    QCustomPlot *customPlot8;
    QCustomPlot *customPlot9;
    QCustomPlot *customPlot10;
    QCustomPlot *customPlot11;
    QCustomPlot *customPlot12;
    QCustomPlot *customPlot13;
    QCustomPlot *customPlot14;
    QCustomPlot *customPlot15;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_13;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton_fresh;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label;
    QComboBox *portBox;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QComboBox *comboBox_baudRate;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_3;
    QLineEdit *lineEdit_port;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_open;
    QPushButton *pushButton_close;
    QPushButton *pushButton_connectWifi;
    QVBoxLayout *verticalLayout_5;
    QTextBrowser *textBrowser_log;
    QPushButton *pushButton_clearLog;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_squareWaveTest;
    QPushButton *pushButton_noiseTest;
    QPushButton *pushButton_normalTest;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_reset;
    QPushButton *pushButton_beginReadC;
    QPushButton *pushButton_stopReadC;
    QPushButton *pushButton_ClearPlot;
    QHBoxLayout *horizontalLayout_12;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_workBench;
    QLineEdit *lineEdit_Dir;
    QHBoxLayout *horizontalLayout_11;
    QLineEdit *lineEdit_fileName;
    QSpinBox *spinBox_fileName;
    QVBoxLayout *verticalLayout_8;
    QPushButton *pushButton_record;
    QCheckBox *checkBox_overwrite;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_14;
    QComboBox *comboBox_channel;
    QPushButton *pushButton_FFT;
    QPushButton *pushButton_SetMask;
    QPushButton *pushButton_DoSep;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(2361, 1328);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setAnimated(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        customPlot = new QCustomPlot(centralWidget);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        customPlot->setGeometry(QRect(1920, 160, 431, 241));
        lineEdit_channel = new QLineEdit(customPlot);
        lineEdit_channel->setObjectName(QString::fromUtf8("lineEdit_channel"));
        lineEdit_channel->setGeometry(QRect(0, 340, 81, 31));
        lineEdit_channel->setReadOnly(true);
        customPlot0 = new QCustomPlot(centralWidget);
        customPlot0->setObjectName(QString::fromUtf8("customPlot0"));
        customPlot0->setGeometry(QRect(10, 160, 461, 241));
        lineEdit_channel1 = new QLineEdit(customPlot0);
        lineEdit_channel1->setObjectName(QString::fromUtf8("lineEdit_channel1"));
        lineEdit_channel1->setGeometry(QRect(0, 240, 61, 21));
        lineEdit_channel1->setReadOnly(true);
        replayBar = new QScrollBar(centralWidget);
        replayBar->setObjectName(QString::fromUtf8("replayBar"));
        replayBar->setGeometry(QRect(10, 1230, 811, 20));
        replayBar->setOrientation(Qt::Horizontal);
        pushButton_openFile = new QPushButton(centralWidget);
        pushButton_openFile->setObjectName(QString::fromUtf8("pushButton_openFile"));
        pushButton_openFile->setGeometry(QRect(920, 1230, 75, 23));
        pushButton_play = new QPushButton(centralWidget);
        pushButton_play->setObjectName(QString::fromUtf8("pushButton_play"));
        pushButton_play->setGeometry(QRect(820, 1230, 75, 23));
        label_Icon = new QLabel(centralWidget);
        label_Icon->setObjectName(QString::fromUtf8("label_Icon"));
        label_Icon->setGeometry(QRect(1860, 20, 21, 21));
        customPlot1 = new QCustomPlot(centralWidget);
        customPlot1->setObjectName(QString::fromUtf8("customPlot1"));
        customPlot1->setGeometry(QRect(490, 160, 481, 241));
        customPlot2 = new QCustomPlot(centralWidget);
        customPlot2->setObjectName(QString::fromUtf8("customPlot2"));
        customPlot2->setGeometry(QRect(990, 160, 471, 241));
        customPlot3 = new QCustomPlot(centralWidget);
        customPlot3->setObjectName(QString::fromUtf8("customPlot3"));
        customPlot3->setGeometry(QRect(1480, 160, 431, 241));
        customPlot4 = new QCustomPlot(centralWidget);
        customPlot4->setObjectName(QString::fromUtf8("customPlot4"));
        customPlot4->setGeometry(QRect(10, 420, 461, 241));
        customPlot5 = new QCustomPlot(centralWidget);
        customPlot5->setObjectName(QString::fromUtf8("customPlot5"));
        customPlot5->setGeometry(QRect(490, 420, 481, 241));
        customPlot6 = new QCustomPlot(centralWidget);
        customPlot6->setObjectName(QString::fromUtf8("customPlot6"));
        customPlot6->setGeometry(QRect(990, 420, 471, 241));
        customPlot7 = new QCustomPlot(centralWidget);
        customPlot7->setObjectName(QString::fromUtf8("customPlot7"));
        customPlot7->setGeometry(QRect(1480, 420, 431, 241));
        customPlot8 = new QCustomPlot(centralWidget);
        customPlot8->setObjectName(QString::fromUtf8("customPlot8"));
        customPlot8->setGeometry(QRect(10, 680, 461, 241));
        customPlot9 = new QCustomPlot(centralWidget);
        customPlot9->setObjectName(QString::fromUtf8("customPlot9"));
        customPlot9->setGeometry(QRect(490, 680, 481, 241));
        customPlot10 = new QCustomPlot(centralWidget);
        customPlot10->setObjectName(QString::fromUtf8("customPlot10"));
        customPlot10->setGeometry(QRect(990, 680, 471, 241));
        customPlot11 = new QCustomPlot(centralWidget);
        customPlot11->setObjectName(QString::fromUtf8("customPlot11"));
        customPlot11->setGeometry(QRect(1480, 680, 431, 241));
        customPlot12 = new QCustomPlot(centralWidget);
        customPlot12->setObjectName(QString::fromUtf8("customPlot12"));
        customPlot12->setGeometry(QRect(10, 930, 461, 221));
        customPlot13 = new QCustomPlot(centralWidget);
        customPlot13->setObjectName(QString::fromUtf8("customPlot13"));
        customPlot13->setGeometry(QRect(490, 930, 481, 221));
        customPlot14 = new QCustomPlot(centralWidget);
        customPlot14->setObjectName(QString::fromUtf8("customPlot14"));
        customPlot14->setGeometry(QRect(990, 930, 471, 221));
        customPlot15 = new QCustomPlot(centralWidget);
        customPlot15->setObjectName(QString::fromUtf8("customPlot15"));
        customPlot15->setGeometry(QRect(1480, 930, 431, 221));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 0, 961, 151));
        horizontalLayout_13 = new QHBoxLayout(layoutWidget);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        pushButton_fresh = new QPushButton(layoutWidget);
        pushButton_fresh->setObjectName(QString::fromUtf8("pushButton_fresh"));

        horizontalLayout_8->addWidget(pushButton_fresh);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_6->addWidget(label);

        portBox = new QComboBox(layoutWidget);
        portBox->setObjectName(QString::fromUtf8("portBox"));

        horizontalLayout_6->addWidget(portBox);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_5->addWidget(label_2);

        comboBox_baudRate = new QComboBox(layoutWidget);
        comboBox_baudRate->addItem(QString());
        comboBox_baudRate->addItem(QString());
        comboBox_baudRate->addItem(QString());
        comboBox_baudRate->addItem(QString());
        comboBox_baudRate->addItem(QString());
        comboBox_baudRate->setObjectName(QString::fromUtf8("comboBox_baudRate"));

        horizontalLayout_5->addWidget(comboBox_baudRate);


        verticalLayout_3->addLayout(horizontalLayout_5);


        horizontalLayout_8->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_7->addWidget(label_3);

        lineEdit_port = new QLineEdit(layoutWidget);
        lineEdit_port->setObjectName(QString::fromUtf8("lineEdit_port"));

        horizontalLayout_7->addWidget(lineEdit_port);


        verticalLayout_4->addLayout(horizontalLayout_7);


        horizontalLayout_10->addLayout(verticalLayout_4);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton_open = new QPushButton(layoutWidget);
        pushButton_open->setObjectName(QString::fromUtf8("pushButton_open"));

        verticalLayout_2->addWidget(pushButton_open);

        pushButton_close = new QPushButton(layoutWidget);
        pushButton_close->setObjectName(QString::fromUtf8("pushButton_close"));

        verticalLayout_2->addWidget(pushButton_close);

        pushButton_connectWifi = new QPushButton(layoutWidget);
        pushButton_connectWifi->setObjectName(QString::fromUtf8("pushButton_connectWifi"));

        verticalLayout_2->addWidget(pushButton_connectWifi);


        horizontalLayout_9->addLayout(verticalLayout_2);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        textBrowser_log = new QTextBrowser(layoutWidget);
        textBrowser_log->setObjectName(QString::fromUtf8("textBrowser_log"));

        verticalLayout_5->addWidget(textBrowser_log);

        pushButton_clearLog = new QPushButton(layoutWidget);
        pushButton_clearLog->setObjectName(QString::fromUtf8("pushButton_clearLog"));

        verticalLayout_5->addWidget(pushButton_clearLog);


        horizontalLayout_9->addLayout(verticalLayout_5);


        verticalLayout_6->addLayout(horizontalLayout_9);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_squareWaveTest = new QPushButton(layoutWidget);
        pushButton_squareWaveTest->setObjectName(QString::fromUtf8("pushButton_squareWaveTest"));

        horizontalLayout_2->addWidget(pushButton_squareWaveTest);

        pushButton_noiseTest = new QPushButton(layoutWidget);
        pushButton_noiseTest->setObjectName(QString::fromUtf8("pushButton_noiseTest"));

        horizontalLayout_2->addWidget(pushButton_noiseTest);

        pushButton_normalTest = new QPushButton(layoutWidget);
        pushButton_normalTest->setObjectName(QString::fromUtf8("pushButton_normalTest"));

        horizontalLayout_2->addWidget(pushButton_normalTest);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton_reset = new QPushButton(layoutWidget);
        pushButton_reset->setObjectName(QString::fromUtf8("pushButton_reset"));

        horizontalLayout_3->addWidget(pushButton_reset);

        pushButton_beginReadC = new QPushButton(layoutWidget);
        pushButton_beginReadC->setObjectName(QString::fromUtf8("pushButton_beginReadC"));

        horizontalLayout_3->addWidget(pushButton_beginReadC);

        pushButton_stopReadC = new QPushButton(layoutWidget);
        pushButton_stopReadC->setObjectName(QString::fromUtf8("pushButton_stopReadC"));

        horizontalLayout_3->addWidget(pushButton_stopReadC);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_4->addLayout(verticalLayout);

        pushButton_ClearPlot = new QPushButton(layoutWidget);
        pushButton_ClearPlot->setObjectName(QString::fromUtf8("pushButton_ClearPlot"));

        horizontalLayout_4->addWidget(pushButton_ClearPlot);


        verticalLayout_6->addLayout(horizontalLayout_4);


        horizontalLayout_10->addLayout(verticalLayout_6);


        horizontalLayout_13->addLayout(horizontalLayout_10);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_workBench = new QPushButton(layoutWidget);
        pushButton_workBench->setObjectName(QString::fromUtf8("pushButton_workBench"));

        horizontalLayout->addWidget(pushButton_workBench);

        lineEdit_Dir = new QLineEdit(layoutWidget);
        lineEdit_Dir->setObjectName(QString::fromUtf8("lineEdit_Dir"));
        lineEdit_Dir->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit_Dir);


        verticalLayout_7->addLayout(horizontalLayout);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        lineEdit_fileName = new QLineEdit(layoutWidget);
        lineEdit_fileName->setObjectName(QString::fromUtf8("lineEdit_fileName"));

        horizontalLayout_11->addWidget(lineEdit_fileName);

        spinBox_fileName = new QSpinBox(layoutWidget);
        spinBox_fileName->setObjectName(QString::fromUtf8("spinBox_fileName"));

        horizontalLayout_11->addWidget(spinBox_fileName);


        verticalLayout_7->addLayout(horizontalLayout_11);


        horizontalLayout_12->addLayout(verticalLayout_7);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        pushButton_record = new QPushButton(layoutWidget);
        pushButton_record->setObjectName(QString::fromUtf8("pushButton_record"));

        verticalLayout_8->addWidget(pushButton_record);

        checkBox_overwrite = new QCheckBox(layoutWidget);
        checkBox_overwrite->setObjectName(QString::fromUtf8("checkBox_overwrite"));

        verticalLayout_8->addWidget(checkBox_overwrite);


        horizontalLayout_12->addLayout(verticalLayout_8);


        horizontalLayout_13->addLayout(horizontalLayout_12);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(1920, 80, 431, 71));
        horizontalLayout_14 = new QHBoxLayout(widget);
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        comboBox_channel = new QComboBox(widget);
        comboBox_channel->setObjectName(QString::fromUtf8("comboBox_channel"));

        horizontalLayout_14->addWidget(comboBox_channel);

        pushButton_FFT = new QPushButton(widget);
        pushButton_FFT->setObjectName(QString::fromUtf8("pushButton_FFT"));

        horizontalLayout_14->addWidget(pushButton_FFT);

        pushButton_SetMask = new QPushButton(widget);
        pushButton_SetMask->setObjectName(QString::fromUtf8("pushButton_SetMask"));

        horizontalLayout_14->addWidget(pushButton_SetMask);

        pushButton_DoSep = new QPushButton(widget);
        pushButton_DoSep->setObjectName(QString::fromUtf8("pushButton_DoSep"));

        horizontalLayout_14->addWidget(pushButton_DoSep);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 2361, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_openFile->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        pushButton_play->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        label_Icon->setText(QString());
        pushButton_fresh->setText(QCoreApplication::translate("MainWindow", "\345\210\267\346\226\260", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\267\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        comboBox_baudRate->setItemText(0, QCoreApplication::translate("MainWindow", "921600", nullptr));
        comboBox_baudRate->setItemText(1, QCoreApplication::translate("MainWindow", "115200", nullptr));
        comboBox_baudRate->setItemText(2, QCoreApplication::translate("MainWindow", "9600", nullptr));
        comboBox_baudRate->setItemText(3, QCoreApplication::translate("MainWindow", "19200", nullptr));
        comboBox_baudRate->setItemText(4, QCoreApplication::translate("MainWindow", "38400", nullptr));

        label_3->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243\345\217\267\357\274\232", nullptr));
        lineEdit_port->setText(QCoreApplication::translate("MainWindow", "8081;8082;8083;8084", nullptr));
        pushButton_open->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
        pushButton_close->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
        pushButton_connectWifi->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
        pushButton_clearLog->setText(QString());
        pushButton_squareWaveTest->setText(QCoreApplication::translate("MainWindow", "Square Wave", nullptr));
        pushButton_noiseTest->setText(QCoreApplication::translate("MainWindow", "Noise Test", nullptr));
        pushButton_normalTest->setText(QCoreApplication::translate("MainWindow", "Normal Test", nullptr));
        pushButton_reset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        pushButton_beginReadC->setText(QCoreApplication::translate("MainWindow", "Begin ReadC", nullptr));
        pushButton_stopReadC->setText(QCoreApplication::translate("MainWindow", "Stop ReaadC", nullptr));
        pushButton_ClearPlot->setText(QCoreApplication::translate("MainWindow", "Clear Plot", nullptr));
        pushButton_workBench->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210", nullptr));
        pushButton_record->setText(QCoreApplication::translate("MainWindow", "Record", nullptr));
        checkBox_overwrite->setText(QCoreApplication::translate("MainWindow", "overwrite", nullptr));
        pushButton_FFT->setText(QCoreApplication::translate("MainWindow", "FFT", nullptr));
        pushButton_SetMask->setText(QCoreApplication::translate("MainWindow", "Set As Mask", nullptr));
        pushButton_DoSep->setText(QCoreApplication::translate("MainWindow", "Do Separation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
