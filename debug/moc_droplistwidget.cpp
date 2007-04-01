/****************************************************************************
** Meta object code from reading C++ file 'droplistwidget.h'
**
** Created: Tue 20. Mar 12:26:37 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../droplistwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'droplistwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_DropListWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      21,   16,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DropListWidget[] = {
    "DropListWidget\0\0text\0droppedText(QString)\0"
};

const QMetaObject DropListWidget::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_DropListWidget,
      qt_meta_data_DropListWidget, 0 }
};

const QMetaObject *DropListWidget::metaObject() const
{
    return &staticMetaObject;
}

void *DropListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DropListWidget))
	return static_cast<void*>(const_cast<DropListWidget*>(this));
    return QListWidget::qt_metacast(_clname);
}

int DropListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: droppedText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DropListWidget::droppedText(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
