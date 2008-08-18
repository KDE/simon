FORMS += 	$$PWD/commandsettingsdlg.ui

HEADERS += 	$$PWD/command.h \
		$$PWD/commandmanager.h \
		$$PWD/commandmodel.h \
		$$PWD/commandsettings.h

SOURCES +=	$$PWD/commandmanager.cpp \
		$$PWD/command.cpp \
		$$PWD/commandmodel.cpp \
		$$PWD/commandsettings.cpp


include($$PWD/ATIntegration/ATIntegration.pri)
include($$PWD/DesktopGrid/DesktopGrid.pri)
include($$PWD/Executable/Executable.pri)
include($$PWD/Place/Place.pri)
include($$PWD/Shortcut/Shortcut.pri)
include($$PWD/TextMacro/TextMacro.pri)
