#include "mainwindow.h"
#include <QApplication>
#include<QDebug>

#include "fftw3/fftw3.h"
#define N 5
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
