#include "mainwindow.h"
#include <QApplication>
#include<QDebug>

#include "fftw3/fftw3.h"
#include "qt-cnn.hpp"

#define N 5
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    //cnnmain
//    cout << "=========================" << endl;
//    cout << "=========================" << endl;
//    cout << "=========================" << endl;
//    cout << "=========================" << endl;
//    //run();


    return a.exec();
}
