FORMS += 	Actions/Commands/commandsettingsdlg.ui

HEADERS += 	Actions/Commands/command.h \
		Actions/Commands/commandmanager.h \
		Actions/Commands/commandmodel.h \
		Actions/Commands/commandsettings.h

SOURCES +=	Actions/Commands/commandmanager.cpp \
		Actions/Commands/command.cpp \
		Actions/Commands/commandmodel.cpp \
		Actions/Commands/commandsettings.cpp


include(Actions/Commands/ATIntegration/ATIntegration.pri)
include(Actions/Commands/DesktopGrid/DesktopGrid.pri)
include(Actions/Commands/Executable/Executable.pri)
include(Actions/Commands/Place/Place.pri)
include(Actions/Commands/Shortcut/Shortcut.pri)
include(Actions/Commands/TextMacro/TextMacro.pri)
