include(ModelManagement/Grammar/ImportGrammar/ImportGrammar.pri)
include(ModelManagement/Grammar/MergeTerminals/MergeTerminals.pri)
include(ModelManagement/Grammar/RenameTerminal/RenameTerminal.pri)

FORMS += 	ModelManagement/Grammar/grammarsettingsdlg.ui

HEADERS += 	ModelManagement/Grammar/grammarmanager.h \
		ModelManagement/Grammar/grammarsettings.h

SOURCES += 	ModelManagement/Grammar/grammarmanager.cpp \
		ModelManagement/Grammar/grammarsettings.cpp
