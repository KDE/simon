/****************************************************************************
** Meta object code from reading C++ file 'wordlistview.h'
**
** Created: Tue 20. Mar 12:26:06 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../wordlistview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wordlistview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_WordListView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      37,   32,   13,   13, 0x0a,
      60,   13,   13,   13, 0x0a,
      78,   13,   13,   13, 0x0a,
      99,   13,   13,   13, 0x0a,
     116,  109,   13,   13, 0x0a,
     145,   13,   13,   13, 0x0a,
     163,   13,   13,   13, 0x0a,
     195,  189,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WordListView[] = {
    "WordListView\0\0suggestTraining()\0word\0"
    "markWordToTrain(Word*)\0copyWordToTrain()\0"
    "deleteTrainingWord()\0addWord()\0filter\0"
    "filterListbyPattern(QString)\0"
    "clearSearchText()\0switchToGenericTraining()\0"
    "vocab\0insertVocab(WordList*)\0"
};

const QMetaObject WordListView::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_WordListView,
      qt_meta_data_WordListView, 0 }
};

const QMetaObject *WordListView::metaObject() const
{
    return &staticMetaObject;
}

void *WordListView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WordListView))
	return static_cast<void*>(const_cast<WordListView*>(this));
    return QDialog::qt_metacast(_clname);
}

int WordListView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: suggestTraining(); break;
        case 1: markWordToTrain((*reinterpret_cast< Word*(*)>(_a[1]))); break;
        case 2: copyWordToTrain(); break;
        case 3: deleteTrainingWord(); break;
        case 4: addWord(); break;
        case 5: filterListbyPattern((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: clearSearchText(); break;
        case 7: switchToGenericTraining(); break;
        case 8: insertVocab((*reinterpret_cast< WordList*(*)>(_a[1]))); break;
        }
        _id -= 9;
    }
    return _id;
}
