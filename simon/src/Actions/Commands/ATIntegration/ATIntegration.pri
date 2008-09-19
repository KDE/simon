
unix {
HEADERS +=	$$PWD/dbusbackend.h
SOURCES +=	$$PWD/dbusbackend.cpp 
}

win32 {
HEADERS +=	$$PWD/msaabackend.h 
SOURCES += 	$$PWD/msaabackend.cpp
}

HEADERS += 	$$PWD/atbackend.h \
		$$PWD/atobject.h \
		$$PWD/ato.h \
		$$PWD/atwatcher.h  

SOURCES += 	$$PWD/atobject.cpp \
		$$PWD/atwatcher.cpp 
