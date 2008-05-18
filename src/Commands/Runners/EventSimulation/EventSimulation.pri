win32 {
HEADERS += 	Commands/Runners/EventSimulation/windowsevents.h
SOURCES += 	Commands/Runners/EventSimulation/windowsevents.cpp
}

unix {
HEADERS +=	Commands/Runners/EventSimulation/xevents.h
SOURCES +=	Commands/Runners/EventSimulation/xevents.cpp
}

HEADERS += 	Commands/Runners/EventSimulation/coreevents.h \
		Commands/Runners/EventSimulation/key.h \
		Commands/Runners/EventSimulation/eventhandler.h

SOURCES += 	Commands/Runners/EventSimulation/eventhandler.cpp 
