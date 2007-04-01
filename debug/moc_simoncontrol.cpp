/****************************************************************************
** Meta object code from reading C++ file 'simoncontrol.h'
**
** Created: Tue 20. Mar 12:26:01 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../simoncontrol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simoncontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_SimonControl[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      34,   13,   13,   13, 0x0a,
      64,   59,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SimonControl[] = {
    "SimonControl\0\0connectedToJulius()\0"
    "disconnectedFromJulius()\0word\0"
    "wordRecognised(QString)\0"
};

const QMetaObject SimonControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SimonControl,
      qt_meta_data_SimonControl, 0 }
};

const QMetaObject *SimonControl::metaObject() const
{
    return &staticMetaObject;
}

void *SimonControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimonControl))
	return static_cast<void*>(const_cast<SimonControl*>(this));
    return QObject::qt_metacast(_clname);
}

int SimonControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectedToJulius(); break;
        case 1: disconnectedFromJulius(); break;
        case 2: wordRecognised((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}
