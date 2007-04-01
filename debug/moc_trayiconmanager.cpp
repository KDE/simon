/****************************************************************************
** Meta object code from reading C++ file 'trayiconmanager.h'
**
** Created: Tue 20. Mar 12:26:20 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../trayiconmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trayiconmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_TrayIconManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      27,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   43,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TrayIconManager[] = {
    "TrayIconManager\0\0clicked()\0middleClicked()\0"
    "reason\0triggered(QSystemTrayIcon::ActivationReason)\0"
};

const QMetaObject TrayIconManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TrayIconManager,
      qt_meta_data_TrayIconManager, 0 }
};

const QMetaObject *TrayIconManager::metaObject() const
{
    return &staticMetaObject;
}

void *TrayIconManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TrayIconManager))
	return static_cast<void*>(const_cast<TrayIconManager*>(this));
    return QObject::qt_metacast(_clname);
}

int TrayIconManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        case 1: middleClicked(); break;
        case 2: triggered((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TrayIconManager::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void TrayIconManager::middleClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
