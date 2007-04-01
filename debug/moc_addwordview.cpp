/****************************************************************************
** Meta object code from reading C++ file 'addwordview.h'
**
** Created: Tue 20. Mar 12:25:57 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../addwordview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'addwordview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_AddWordView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      24,   12,   12,   12, 0x0a,
      35,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AddWordView[] = {
    "AddWordView\0\0nextStep()\0prevStep()\0"
    "finish()\0"
};

const QMetaObject AddWordView::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AddWordView,
      qt_meta_data_AddWordView, 0 }
};

const QMetaObject *AddWordView::metaObject() const
{
    return &staticMetaObject;
}

void *AddWordView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AddWordView))
	return static_cast<void*>(const_cast<AddWordView*>(this));
    return QDialog::qt_metacast(_clname);
}

int AddWordView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: nextStep(); break;
        case 1: prevStep(); break;
        case 2: finish(); break;
        }
        _id -= 3;
    }
    return _id;
}
