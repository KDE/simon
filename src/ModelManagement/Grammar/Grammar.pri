include($$PWD/ImportGrammar/ImportGrammar.pri)
include($$PWD/MergeTerminals/MergeTerminals.pri)
include($$PWD/RenameTerminal/RenameTerminal.pri)

FORMS += 	$$PWD/grammarsettingsdlg.ui

HEADERS += 	$$PWD/grammarmanager.h \
		$$PWD/grammarsettings.h

SOURCES += 	$$PWD/grammarmanager.cpp \
		$$PWD/grammarsettings.cpp
