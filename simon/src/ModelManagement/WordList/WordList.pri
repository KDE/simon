include($$PWD/AddWord/AddWord.pri)
include($$PWD/ImportDict/ImportDict.pri)
include($$PWD/RemoveWord/RemoveWord.pri)

FORMS += 	$$PWD/wordlist.ui

HEADERS += 	$$PWD/dragtablewidget.h \
		$$PWD/droplistwidget.h \
		$$PWD/word.h \
		$$PWD/wordlistmanager.h \
		$$PWD/wordlistview.h

SOURCES += 	$$PWD/dragtablewidget.cpp \
		$$PWD/droplistwidget.cpp \
		$$PWD/wordlistmanager.cpp \
		$$PWD/wordlistview.cpp
