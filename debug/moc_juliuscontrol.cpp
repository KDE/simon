/****************************************************************************
** Meta object code from reading C++ file 'juliuscontrol.h'
**
** Created: Tue 20. Mar 12:26:16 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../juliuscontrol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'juliuscontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_JuliusControl[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x05,
      44,   14,   14,   14, 0x05,
      56,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      71,   14,   14,   14, 0x0a,
      84,   14,   14,   14, 0x0a,
     100,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_JuliusControl[] = {
    "JuliusControl\0\0word\0wordRecognised(QString)\0"
    "connected()\0disconnected()\0recognised()\0"
    "getConnection()\0connectionLost()\0"
};

const QMetaObject JuliusControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_JuliusControl,
      qt_meta_data_JuliusControl, 0 }
};

const QMetaObject *JuliusControl::metaObject() const
{
    return &staticMetaObject;
}

void *JuliusControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JuliusControl))
	return static_cast<void*>(const_cast<JuliusControl*>(this));
    return QObject::qt_metacast(_clname);
}

int JuliusControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: wordRecognised((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: connected(); break;
        case 2: disconnected(); break;
        case 3: recognised(); break;
        case 4: getConnection(); break;
        case 5: connectionLost(); break;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void JuliusControl::wordRecognised(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void JuliusControl::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void JuliusControl::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
