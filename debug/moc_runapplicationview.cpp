/****************************************************************************
** Meta object code from reading C++ file 'runapplicationview.h'
**
** Created: Tue 20. Mar 12:26:45 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../runapplicationview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runapplicationview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_RunApplicationView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      25,   20,   19,   19, 0x0a,
      61,   53,   19,   19, 0x0a,
      91,   19,   19,   19, 0x0a,
     112,   19,   19,   19, 0x0a,
     130,   19,   19,   19, 0x0a,
     164,  148,   19,   19, 0x0a,
     209,  202,   19,   19, 0x2a,
     267,  246,  234,   19, 0x0a,
     314,  302,  234,   19, 0x2a,
     336,  246,  234,   19, 0x0a,
     378,  302,  234,   19, 0x2a,
     411,  302,  407,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RunApplicationView[] = {
    "RunApplicationView\0\0list\0"
    "insertCommands(CommandList)\0command\0"
    "runCommand(QTableWidgetItem*)\0"
    "runSelectedCommand()\0clearSearchText()\0"
    "filterByPattern()\0filter,commlist\0"
    "filterByPattern(QString,CommandList*)\0"
    "filter\0filterByPattern(QString)\0"
    "CommandList\0commandtype,commlist\0"
    "filterByCategory(int,CommandList*)\0"
    "commandtype\0filterByCategory(int)\0"
    "filterByStrCategory(QString,CommandList*)\0"
    "filterByStrCategory(QString)\0int\0"
    "getCategory(QString)\0"
};

const QMetaObject RunApplicationView::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_RunApplicationView,
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
	return static_cast<void*>(const_cast<RunApplicationView*>(this));
    return QDialog::qt_metacast(_clname);
}

int RunApplicationView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: insertCommands((*reinterpret_cast< CommandList(*)>(_a[1]))); break;
        case 1: runCommand((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: runSelectedCommand(); break;
        case 3: clearSearchText(); break;
        case 4: filterByPattern(); break;
        case 5: filterByPattern((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< CommandList*(*)>(_a[2]))); break;
        case 6: filterByPattern((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: { CommandList _r = filterByCategory((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< CommandList*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< CommandList*>(_a[0]) = _r; }  break;
        case 8: { CommandList _r = filterByCategory((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< CommandList*>(_a[0]) = _r; }  break;
        case 9: { CommandList _r = filterByStrCategory((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< CommandList*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< CommandList*>(_a[0]) = _r; }  break;
        case 10: { CommandList _r = filterByStrCategory((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< CommandList*>(_a[0]) = _r; }  break;
        case 11: { int _r = getCategory((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        }
        _id -= 12;
    }
    return _id;
}
