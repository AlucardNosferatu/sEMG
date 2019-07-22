#-------------------------------------------------
#
# Project created by QtCreator 2017-11-07T19:50:38
#
#-------------------------------------------------

QT       += core gui
QT       += widgets serialport
QT       += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ADS1298_Monitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    iirfilter.cpp \
    dialogs/fftdialog.cpp \
    ads1298decoder.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    iirfilter.h \
    fftw3/fftw3.h \
    dialogs/fftdialog.h \
    ads1298decoder.h

FORMS    += mainwindow.ui \
    dialogs/fftdialog.ui

INCLUDEPATH += $$PWD/fftw3
DEPENDPATH += $$PWD/fftw3

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/fftw3/libfftw3-3.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/fftw3/liblibfftw3-3.a

win32: LIBS += -L$$PWD/fftw3/ -llibfftw3-3

DISTFILES +=

RESOURCES += \
    led_icon.qrc
