/****************************************************************************
** Meta object code from reading C++ file 'ads1298decoder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ADS1298_Monitor/ads1298decoder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ads1298decoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Ads1298Decoder_t {
    QByteArrayData data[12];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Ads1298Decoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Ads1298Decoder_t qt_meta_stringdata_Ads1298Decoder = {
    {
QT_MOC_LITERAL(0, 0, 14), // "Ads1298Decoder"
QT_MOC_LITERAL(1, 15, 16), // "hasNewDataPacket"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 5), // "index"
QT_MOC_LITERAL(4, 39, 7), // "double*"
QT_MOC_LITERAL(5, 47, 5), // "newDP"
QT_MOC_LITERAL(6, 53, 14), // "hasNewCmdReply"
QT_MOC_LITERAL(7, 68, 4), // "cmdR"
QT_MOC_LITERAL(8, 73, 20), // "hasNewWifiConnection"
QT_MOC_LITERAL(9, 94, 23), // "handleWifiNewConnection"
QT_MOC_LITERAL(10, 118, 24), // "handleWifiReceiveNewData"
QT_MOC_LITERAL(11, 143, 23) // "handleComReceiveNewData"

    },
    "Ads1298Decoder\0hasNewDataPacket\0\0index\0"
    "double*\0newDP\0hasNewCmdReply\0cmdR\0"
    "hasNewWifiConnection\0handleWifiNewConnection\0"
    "handleWifiReceiveNewData\0"
    "handleComReceiveNewData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Ads1298Decoder[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       6,    1,   49,    2, 0x06 /* Public */,
       8,    1,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   55,    2, 0x08 /* Private */,
      10,    0,   56,    2, 0x08 /* Private */,
      11,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Char,    7,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Ads1298Decoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Ads1298Decoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->hasNewDataPacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double*(*)>(_a[2]))); break;
        case 1: _t->hasNewCmdReply((*reinterpret_cast< char(*)>(_a[1]))); break;
        case 2: _t->hasNewWifiConnection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->handleWifiNewConnection(); break;
        case 4: _t->handleWifiReceiveNewData(); break;
        case 5: _t->handleComReceiveNewData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Ads1298Decoder::*)(int , double * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ads1298Decoder::hasNewDataPacket)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Ads1298Decoder::*)(char );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ads1298Decoder::hasNewCmdReply)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Ads1298Decoder::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Ads1298Decoder::hasNewWifiConnection)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Ads1298Decoder::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Ads1298Decoder.data,
    qt_meta_data_Ads1298Decoder,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Ads1298Decoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Ads1298Decoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Ads1298Decoder.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Ads1298Decoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Ads1298Decoder::hasNewDataPacket(int _t1, double * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Ads1298Decoder::hasNewCmdReply(char _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Ads1298Decoder::hasNewWifiConnection(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
