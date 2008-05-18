include(ModelManagement/WordList/AddWord/AddWord.pri)
include(ModelManagement/WordList/ImportDict/ImportDict.pri)
include(ModelManagement/WordList/RemoveWord/RemoveWord.pri)

FORMS += 	ModelManagement/WordList/wordlist.ui

HEADERS += 	ModelManagement/WordList/dragtablewidget.h \
		ModelManagement/WordList/droplistwidget.h \
		ModelManagement/WordList/word.h \
		ModelManagement/WordList/wordlistmanager.h \
		ModelManagement/WordList/wordlistview.h

SOURCES += 	ModelManagement/WordList/dragtablewidget.cpp \
		ModelManagement/WordList/droplistwidget.cpp \
		ModelManagement/WordList/wordlistmanager.cpp \
		ModelManagement/WordList/wordlistview.cpp
