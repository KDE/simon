/****************************************************************************
** Meta object code from reading C++ file 'simonview.h'
**
** Created: Tue 20. Mar 12:25:50 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../simonview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simonview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_SimonView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   11,   10,   10, 0x0a,
      31,   10,   10,   10, 0x0a,
      50,   10,   10,   10, 0x0a,
      62,   10,   10,   10, 0x0a,
      74,   10,   10,   10, 0x0a,
      87,   10,   10,   10, 0x0a,
     106,   10,   10,   10, 0x0a,
     126,   10,   10,   10, 0x0a,
     142,   10,   10,   10, 0x0a,
     160,   10,   10,   10, 0x0a,
     181,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SimonView[] = {
    "SimonView\0\0level\0setLevel(int)\0"
    "toggleVisibility()\0hideSimon()\0"
    "showSimon()\0closeSimon()\0toggleActivation()\0"
    "showAddWordDialog()\0showRunDialog()\0"
    "showTrainDialog()\0showWordListDialog()\0"
    "showSettingsDialog()\0"
};

const QMetaObject SimonView::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SimonView,
      qt_meta_data_SimonView, 0 }
};

const QMetaObject *SimonView::metaObject() const
{
    return &staticMetaObject;
}

void *SimonView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimonView))
	return static_cast<void*>(const_cast<SimonView*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SimonView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: toggleVisibility(); break;
        case 2: hideSimon(); break;
        case 3: showSimon(); break;
        case 4: closeSimon(); break;
        case 5: toggleActivation(); break;
        case 6: showAddWordDialog(); break;
        case 7: showRunDialog(); break;
        case 8: showTrainDialog(); break;
        case 9: showWordListDialog(); break;
        case 10: showSettingsDialog(); break;
        }
        _id -= 11;
    }
    return _id;
}
