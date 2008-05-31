HEADERS += 	Actions/Commands/Executable/executablecommand.h \
		Actions/Commands/Executable/executablecommandmanager.h \
		Actions/Commands/Executable/xmlexecutablecommand.h

SOURCES += 	Actions/Commands/Executable/executablecommand.cpp \
		Actions/Commands/Executable/executablecommandmanager.cpp \
		Actions/Commands/Executable/xmlexecutablecommand.cpp

include(Actions/Commands/Executable/ImportProgram/ImportProgram.pri)