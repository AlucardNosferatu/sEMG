/********************************************************************************
** Form generated from reading UI file 'fftdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FFTDIALOG_H
#define UI_FFTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_FFTDialog
{
public:
    QCustomPlot *customPlot;
    QPushButton *pushButton;

    void setupUi(QDialog *FFTDialog)
    {
        if (FFTDialog->objectName().isEmpty())
            FFTDialog->setObjectName(QString::fromUtf8("FFTDialog"));
        FFTDialog->resize(683, 451);
        customPlot = new QCustomPlot(FFTDialog);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        customPlot->setGeometry(QRect(30, 50, 611, 301));
        pushButton = new QPushButton(FFTDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(40, 390, 75, 23));

        retranslateUi(FFTDialog);

        QMetaObject::connectSlotsByName(FFTDialog);
    } // setupUi

    void retranslateUi(QDialog *FFTDialog)
    {
        FFTDialog->setWindowTitle(QCoreApplication::translate("FFTDialog", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("FFTDialog", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FFTDialog: public Ui_FFTDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FFTDIALOG_H
