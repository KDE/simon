/****************************************************************************
** Meta object code from reading C++ file 'vumeter.h'
**
** Created: Tue 20. Mar 12:26:56 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../vumeter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vumeter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_VuMeter[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,    9,    8,    8, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_VuMeter[] = {
    "VuMeter\0\0level\0level(int)\0"
};

const QMetaObject VuMeter::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_VuMeter,
      qt_meta_data_VuMeter, 0 }
};

const QMetaObject *VuMeter::metaObject() const
{
    return &staticMetaObject;
}

void *VuMeter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VuMeter))
	return static_cast<void*>(const_cast<VuMeter*>(this));
    return QThread::qt_metacast(_clname);
}

int VuMeter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: level((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void VuMeter::level(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
