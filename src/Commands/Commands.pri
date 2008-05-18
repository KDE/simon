message ( "Processing Commands..." )

unix{
HEADERS += 	Commands/runlinuxbackend.h 
SOURCES += 	Commands/runlinuxbackend.cpp 
}

win32{
HEADERS += 	Commands/runwindowsbackend.h 
SOURCES += 	Commands/runwindowsbackend.cpp 
}

FORMS += 	Commands/modifycommands.ui \
		Commands/rundialog.ui

HEADERS += 	Commands/command.h \
		Commands/newcommand.h \
		Commands/runapplicationview.h \
		Commands/runbackend.h \
		Commands/runcommand.h \
		Commands/xmlcommand.h

SOURCES += 	Commands/newcommand.cpp \
		Commands/runapplicationview.cpp \
		Commands/runcommand.cpp \
		Commands/xmlcommand.cpp


include(Commands/ImportPlace/ImportPlace.pri)
include(Commands/ImportProgram/ImportProgram.pri)
include(Commands/Runners/Runners.pri)


QT += 		xml

