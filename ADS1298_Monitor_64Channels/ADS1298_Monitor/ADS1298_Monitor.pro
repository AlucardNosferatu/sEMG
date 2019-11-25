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


SOURCES += dialogs/fftdialog.cpp \
    tinyxml/tinystr.cpp \
    tinyxml/tinyxml.cpp \
    tinyxml/tinyxmlerror.cpp \
    tinyxml/tinyxmlparser.cpp \
    ads1298decoder.cpp \
    Filter.cpp \
    iirfilter.cpp \
    main.cpp \
    mainwindow.cpp \
    Matrix.cpp \
    qcustomplot.cpp \
    Tensor.cpp \
    utils.cpp \
    qt-cnn.cpp

HEADERS  += cnnmain.hpp \
    dialogs/fftdialog.h \
    fftw3/fftw3.h \
    qt-cnn.hpp \
    tinyxml/tinystr.h \
    tinyxml/tinyxml.h \
    ads1298decoder.h \
    Filter.hpp \
    iirfilter.h \
    mainwindow.h \
    Matrix.hpp \
    qcustomplot.h \
    sEMG.hpp \
    Tensor.hpp \
    utils.hpp \

FORMS    += \
    dialogs/fftdialog.ui \
    mainwindow.ui


#INCLUDEPATH += /opt/fftw/include
#DEPENDPATH += /opt/fftw/include
INCLUDEPATH += $$PWD/fftw3
DEPENDPATH += $$PWD/fftw3

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/fftw3/libfftw3-3.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/fftw3/liblibfftw3-3.a

win32: LIBS += -L$$PWD/fftw3/ -llibfftw3-3
#LIBS += -L"/opt/fftw/lib" -lfftw3

##libtorch
#unix:!macx: LIBS += -L"//home/mitom/dlcv/libtorch/lib" -ltorch
#INCLUDEPATH += /home/mitom/dlcv/libtorch/include
#DEPENDPATH += /home/mitom/dlcv/libtorch/include

DISTFILES += \
    bias1_weight.xml \
    bias2_weight.xml \
    bias3_weight.xml \
    bn1_bias.xml \
    bn1_running_mean.xml \
    bn1_running_var.xml \
    bn1_weight.xml \
    conv1_0_weight.xml \
    conv1_0_weight_1.xml \
    conv1_weight.xml \
    conv2_weight.xml \
    conv3_weight.xml \
    fullconn1_bias.xml \
    fullconn1_weight.xml \
    fullconn2_bias.xml \
    fullconn2_weight.xml \
    online20_0_input.txt \
    sEMG_weight_5_cpp.pt

RESOURCES += \
    led_icon.qrc


