unix {
HEADERS += 	$$PWD/kdeprogrammanager.h

SOURCES += 	$$PWD/kdeprogrammanager.cpp
}

win32 {
HEADERS += 	$$PWD/windowsprogrammanager.h 

SOURCES += 	$$PWD/windowsprogrammanager.cpp
}


HEADERS += 	$$PWD/categoryxmlreader.h \
		$$PWD/programcategory.h \
		$$PWD/program.h \
		$$PWD/programmanager.h
		

SOURCES += 	$$PWD/categoryxmlreader.cpp \
		$$PWD/programmanager.cpp
		
