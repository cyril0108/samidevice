/****************************************************************************
** Meta object code from reading C++ file 'servercore.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../servercore.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'servercore.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ServerCore_t {
    QByteArrayData data[10];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ServerCore_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ServerCore_t qt_meta_stringdata_ServerCore = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ServerCore"
QT_MOC_LITERAL(1, 11, 11), // "deviceAdded"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 3), // "uid"
QT_MOC_LITERAL(4, 28, 19), // "displayInfoRecieved"
QT_MOC_LITERAL(5, 48, 6), // "retMap"
QT_MOC_LITERAL(6, 55, 15), // "commandReturned"
QT_MOC_LITERAL(7, 71, 19), // "dataWrittenToDevice"
QT_MOC_LITERAL(8, 91, 8), // "remoteIP"
QT_MOC_LITERAL(9, 100, 3) // "msg"

    },
    "ServerCore\0deviceAdded\0\0uid\0"
    "displayInfoRecieved\0retMap\0commandReturned\0"
    "dataWrittenToDevice\0remoteIP\0msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ServerCore[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    2,   37,    2, 0x06 /* Public */,
       6,    2,   42,    2, 0x06 /* Public */,
       7,    2,   47,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantMap,    3,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantMap,    3,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    8,    9,

       0        // eod
};

void ServerCore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ServerCore *_t = static_cast<ServerCore *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deviceAdded((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->displayInfoRecieved((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariantMap(*)>(_a[2]))); break;
        case 2: _t->commandReturned((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariantMap(*)>(_a[2]))); break;
        case 3: _t->dataWrittenToDevice((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ServerCore::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ServerCore::deviceAdded)) {
                *result = 0;
            }
        }
        {
            typedef void (ServerCore::*_t)(QString , QVariantMap );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ServerCore::displayInfoRecieved)) {
                *result = 1;
            }
        }
        {
            typedef void (ServerCore::*_t)(QString , QVariantMap );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ServerCore::commandReturned)) {
                *result = 2;
            }
        }
        {
            typedef void (ServerCore::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ServerCore::dataWrittenToDevice)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject ServerCore::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ServerCore.data,
      qt_meta_data_ServerCore,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ServerCore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServerCore::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ServerCore.stringdata0))
        return static_cast<void*>(const_cast< ServerCore*>(this));
    return QObject::qt_metacast(_clname);
}

int ServerCore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ServerCore::deviceAdded(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ServerCore::displayInfoRecieved(QString _t1, QVariantMap _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ServerCore::commandReturned(QString _t1, QVariantMap _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ServerCore::dataWrittenToDevice(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
