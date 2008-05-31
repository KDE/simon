win32 {
HEADERS += 	SimonLib/EventSimulation/windowsevents.h
SOURCES += 	SimonLib/EventSimulation/windowsevents.cpp
}

unix {
HEADERS +=	SimonLib/EventSimulation/xevents.h
SOURCES +=	SimonLib/EventSimulation/xevents.cpp
}

HEADERS += 	SimonLib/EventSimulation/coreevents.h \
		SimonLib/EventSimulation/key.h \
		SimonLib/EventSimulation/shortcut.h \
		SimonLib/EventSimulation/eventhandler.h

SOURCES += 	SimonLib/EventSimulation/eventhandler.cpp \
		SimonLib/EventSimulation/shortcut.cpp
