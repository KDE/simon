/****************************************************************************
** Meta object code from reading C++ file 'trainingview.h'
**
** Created: Tue 20. Mar 12:26:11 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../trainingview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trainingview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_TrainingView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      33,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TrainingView[] = {
    "TrainingView\0\0switchToWordList()\0"
    "loadList()\0"
};

const QMetaObject TrainingView::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_TrainingView,
      qt_meta_data_TrainingView, 0 }
};

const QMetaObject *TrainingView::metaObject() const
{
    return &staticMetaObject;
}

void *TrainingView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TrainingView))
	return static_cast<void*>(const_cast<TrainingView*>(this));
    return QDialog::qt_metacast(_clname);
}

int TrainingView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: switchToWordList(); break;
        case 1: loadList(); break;
        }
        _id -= 2;
    }
    return _id;
}
