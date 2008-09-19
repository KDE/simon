win32 {
HEADERS += 	$$PWD/windowsevents.h
SOURCES += 	$$PWD/windowsevents.cpp
}

unix {
HEADERS +=	$$PWD/xevents.h
SOURCES +=	$$PWD/xevents.cpp
}

HEADERS += 	$$PWD/coreevents.h \
		$$PWD/key.h \
		$$PWD/shortcut.h \
		$$PWD/eventhandler.h

SOURCES += 	$$PWD/eventhandler.cpp \
		$$PWD/shortcut.cpp
