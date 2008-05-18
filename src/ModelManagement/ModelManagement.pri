message ( "Processing ModelManagement..." )

include(ModelManagement/Grammar/Grammar.pri)
include(ModelManagement/Training/Training.pri)
include(ModelManagement/WordList/WordList.pri)

FORMS += 	ModelManagement/modelsettingsdlg.ui

HEADERS += 	ModelManagement/modelmanager.h \
		ModelManagement/modelsettings.h

SOURCES += 	ModelManagement/modelmanager.cpp \
		ModelManagement/modelsettings.cpp
