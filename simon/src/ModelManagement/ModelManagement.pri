message ( "Processing ModelManagement..." )

include($$PWD/Grammar/Grammar.pri)
include($$PWD/Training/Training.pri)
include($$PWD/WordList/WordList.pri)

FORMS += 	$$PWD/modelsettingsdlg.ui

HEADERS += 	$$PWD/modelmanager.h \
		$$PWD/modelsettings.h

SOURCES += 	$$PWD/modelmanager.cpp \
		$$PWD/modelsettings.cpp
