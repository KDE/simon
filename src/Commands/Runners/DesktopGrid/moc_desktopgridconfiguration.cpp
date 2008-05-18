/****************************************************************************
** Meta object code from reading C++ file 'desktopgridconfiguration.h'
**
** Created: Sun May 18 16:41:31 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "desktopgridconfiguration.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'desktopgridconfiguration.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DesktopGridConfiguration[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      31,   25,   26,   25, 0x0a,
      39,   25,   26,   25, 0x0a,
      47,   25,   26,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DesktopGridConfiguration[] = {
    "DesktopGridConfiguration\0\0bool\0apply()\0"
    "reset()\0init()\0"
};

const QMetaObject DesktopGridConfiguration::staticMetaObject = {
    { &SystemWidget::staticMetaObject, qt_meta_stringdata_DesktopGridConfiguration,
      qt_meta_data_DesktopGridConfiguration, 0 }
};

const QMetaObject *DesktopGridConfiguration::metaObject() const
{
    return &staticMetaObject;
}

void *DesktopGridConfiguration::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DesktopGridConfiguration))
	return static_cast<void*>(const_cast< DesktopGridConfiguration*>(this));
    return SystemWidget::qt_metacast(_clname);
}

int DesktopGridConfiguration::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SystemWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = apply();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = reset();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = init();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
