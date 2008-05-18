/****************************************************************************
** Meta object code from reading C++ file 'shortcutsettings.h'
**
** Created: Sun May 18 16:08:45 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "shortcutsettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'shortcutsettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ShortcutSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      52,   18,   17,   17, 0x08,
      81,   17,   17,   17, 0x08,
     101,   92,   17,   17, 0x0a,
     143,  132,  127,   17, 0x0a,
     166,  162,  127,   17, 0x2a,
     181,   17,  127,   17, 0x2a,
     193,   17,   17,   17, 0x0a,
     207,   17,   17,   17, 0x0a,
     224,   17,  127,   17, 0x0a,
     232,   17,  127,   17, 0x0a,
     239,   17,  127,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ShortcutSettings[] = {
    "ShortcutSettings\0\0newrow,newcolumn,oldrow,oldcolumn\0"
    "cellChanged(int,int,int,int)\0reselect()\0"
    "shortcut\0insertShortcut(Shortcut*)\0"
    "bool\0row,column\0applyName(int,int)\0"
    "row\0applyName(int)\0applyName()\0"
    "addShortcut()\0deleteShortcut()\0apply()\0"
    "init()\0reset()\0"
};

const QMetaObject ShortcutSettings::staticMetaObject = {
    { &SystemWidget::staticMetaObject, qt_meta_stringdata_ShortcutSettings,
      qt_meta_data_ShortcutSettings, 0 }
};

const QMetaObject *ShortcutSettings::metaObject() const
{
    return &staticMetaObject;
}

void *ShortcutSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ShortcutSettings))
	return static_cast<void*>(const_cast< ShortcutSettings*>(this));
    return SystemWidget::qt_metacast(_clname);
}

int ShortcutSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SystemWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: reselect(); break;
        case 2: insertShortcut((*reinterpret_cast< Shortcut*(*)>(_a[1]))); break;
        case 3: { bool _r = applyName((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = applyName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = applyName();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: addShortcut(); break;
        case 7: deleteShortcut(); break;
        case 8: { bool _r = apply();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = init();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = reset();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
