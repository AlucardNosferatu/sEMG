/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ADS1298_Monitor/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[27];
    char stringdata0[635];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 31), // "on_pushButton_workBench_clicked"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 28), // "on_pushButton_record_clicked"
QT_MOC_LITERAL(4, 73, 23), // "handleRecordNameChanged"
QT_MOC_LITERAL(5, 97, 22), // "handleHasNewDataPacket"
QT_MOC_LITERAL(6, 120, 5), // "index"
QT_MOC_LITERAL(7, 126, 7), // "double*"
QT_MOC_LITERAL(8, 134, 5), // "newDP"
QT_MOC_LITERAL(9, 140, 20), // "handleHasNewCmdReply"
QT_MOC_LITERAL(10, 161, 4), // "cmdR"
QT_MOC_LITERAL(11, 166, 26), // "handleHasNewWifiConnection"
QT_MOC_LITERAL(12, 193, 26), // "on_pushButton_open_clicked"
QT_MOC_LITERAL(13, 220, 27), // "on_pushButton_fresh_clicked"
QT_MOC_LITERAL(14, 248, 15), // "handleFreshPort"
QT_MOC_LITERAL(15, 264, 27), // "on_pushButton_close_clicked"
QT_MOC_LITERAL(16, 292, 33), // "on_pushButton_connectWifi_cli..."
QT_MOC_LITERAL(17, 326, 31), // "on_pushButton_ClearPlot_clicked"
QT_MOC_LITERAL(18, 358, 30), // "on_pushButton_clearLog_clicked"
QT_MOC_LITERAL(19, 389, 24), // "handleReplotTimerTimeout"
QT_MOC_LITERAL(20, 414, 25), // "on_pushButton_FFT_clicked"
QT_MOC_LITERAL(21, 440, 36), // "on_pushButton_squareWaveTest_..."
QT_MOC_LITERAL(22, 477, 31), // "on_pushButton_noiseTest_clicked"
QT_MOC_LITERAL(23, 509, 27), // "on_pushButton_reset_clicked"
QT_MOC_LITERAL(24, 537, 32), // "on_pushButton_beginReadC_clicked"
QT_MOC_LITERAL(25, 570, 31), // "on_pushButton_stopReadC_clicked"
QT_MOC_LITERAL(26, 602, 32) // "on_pushButton_normalTest_clicked"

    },
    "MainWindow\0on_pushButton_workBench_clicked\0"
    "\0on_pushButton_record_clicked\0"
    "handleRecordNameChanged\0handleHasNewDataPacket\0"
    "index\0double*\0newDP\0handleHasNewCmdReply\0"
    "cmdR\0handleHasNewWifiConnection\0"
    "on_pushButton_open_clicked\0"
    "on_pushButton_fresh_clicked\0handleFreshPort\0"
    "on_pushButton_close_clicked\0"
    "on_pushButton_connectWifi_clicked\0"
    "on_pushButton_ClearPlot_clicked\0"
    "on_pushButton_clearLog_clicked\0"
    "handleReplotTimerTimeout\0"
    "on_pushButton_FFT_clicked\0"
    "on_pushButton_squareWaveTest_clicked\0"
    "on_pushButton_noiseTest_clicked\0"
    "on_pushButton_reset_clicked\0"
    "on_pushButton_beginReadC_clicked\0"
    "on_pushButton_stopReadC_clicked\0"
    "on_pushButton_normalTest_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  119,    2, 0x08 /* Private */,
       3,    0,  120,    2, 0x08 /* Private */,
       4,    0,  121,    2, 0x08 /* Private */,
       5,    2,  122,    2, 0x08 /* Private */,
       9,    1,  127,    2, 0x08 /* Private */,
      11,    1,  130,    2, 0x08 /* Private */,
      12,    0,  133,    2, 0x08 /* Private */,
      13,    0,  134,    2, 0x08 /* Private */,
      14,    0,  135,    2, 0x08 /* Private */,
      15,    0,  136,    2, 0x08 /* Private */,
      16,    0,  137,    2, 0x08 /* Private */,
      17,    0,  138,    2, 0x08 /* Private */,
      18,    0,  139,    2, 0x08 /* Private */,
      19,    0,  140,    2, 0x08 /* Private */,
      20,    0,  141,    2, 0x08 /* Private */,
      21,    0,  142,    2, 0x08 /* Private */,
      22,    0,  143,    2, 0x08 /* Private */,
      23,    0,  144,    2, 0x08 /* Private */,
      24,    0,  145,    2, 0x08 /* Private */,
      25,    0,  146,    2, 0x08 /* Private */,
      26,    0,  147,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,    6,    8,
    QMetaType::Void, QMetaType::Char,   10,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_workBench_clicked(); break;
        case 1: _t->on_pushButton_record_clicked(); break;
        case 2: _t->handleRecordNameChanged(); break;
        case 3: _t->handleHasNewDataPacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double*(*)>(_a[2]))); break;
        case 4: _t->handleHasNewCmdReply((*reinterpret_cast< char(*)>(_a[1]))); break;
        case 5: _t->handleHasNewWifiConnection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_pushButton_open_clicked(); break;
        case 7: _t->on_pushButton_fresh_clicked(); break;
        case 8: _t->handleFreshPort(); break;
        case 9: _t->on_pushButton_close_clicked(); break;
        case 10: _t->on_pushButton_connectWifi_clicked(); break;
        case 11: _t->on_pushButton_ClearPlot_clicked(); break;
        case 12: _t->on_pushButton_clearLog_clicked(); break;
        case 13: _t->handleReplotTimerTimeout(); break;
        case 14: _t->on_pushButton_FFT_clicked(); break;
        case 15: _t->on_pushButton_squareWaveTest_clicked(); break;
        case 16: _t->on_pushButton_noiseTest_clicked(); break;
        case 17: _t->on_pushButton_reset_clicked(); break;
        case 18: _t->on_pushButton_beginReadC_clicked(); break;
        case 19: _t->on_pushButton_stopReadC_clicked(); break;
        case 20: _t->on_pushButton_normalTest_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
