/****************************************************************************
** Meta object code from reading C++ file 'runapplicationview.h'
**
** Created: Sun May 18 16:22:45 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "runapplicationview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runapplicationview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RunApplicationView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      33,   19,   19,   19, 0x08,
      49,   19,   19,   19, 0x08,
      68,   63,   19,   19, 0x08,
      96,   19,   19,   19, 0x08,
     124,  117,   19,   19, 0x08,
     149,   19,   19,   19, 0x28,
     167,   19,   19,   19, 0x0a,
     182,   19,   19,   19, 0x0a,
     203,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RunApplicationView[] = {
    "RunApplicationView\0\0addCommand()\0"
    "deleteCommand()\0editCommand()\0list\0"
    "insertCommands(CommandList)\0"
    "runSelectedCommand()\0filter\0"
    "filterByPattern(QString)\0filterByPattern()\0"
    "loadCommands()\0setSettingsVisible()\0"
    "setSettingsHidden()\0"
};

const QMetaObject RunApplicationView::staticMetaObject = {
    { &InlineWidget::staticMetaObject, qt_meta_stringdata_RunApplicationView,
      qt_meta_data_RunApplicationView, 0 }
};

const QMetaObject *RunApplicationView::metaObject() const
{
    return &staticMetaObject;
}

void *RunApplicationView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RunApplicationView))
	return static_cast<void*>(const_cast< RunApplicationView*>(this));
    return InlineWidget::qt_metacast(_clname);
}

int RunApplicationView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = InlineWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addCommand(); break;
        case 1: deleteCommand(); break;
        case 2: editCommand(); break;
        case 3: insertCommands((*reinterpret_cast< CommandList(*)>(_a[1]))); break;
        case 4: runSelectedCommand(); break;
        case 5: filterByPattern((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: filterByPattern(); break;
        case 7: loadCommands(); break;
        case 8: setSettingsVisible(); break;
        case 9: setSettingsHidden(); break;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
