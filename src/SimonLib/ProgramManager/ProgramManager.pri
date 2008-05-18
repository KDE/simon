unix {
HEADERS += 	SimonLib/ProgramManager/kdeprogrammanager.h

SOURCES += 	SimonLib/ProgramManager/kdeprogrammanager.cpp
}

win32 {
HEADERS += 	SimonLib/ProgramManager/windowsprogrammanager.h 

SOURCES += 	SimonLib/ProgramManager/windowsprogrammanager.cpp
}


HEADERS += 	SimonLib/ProgramManager/categoryxmlreader.h \
		SimonLib/ProgramManager/programcategory.h \
		SimonLib/ProgramManager/program.h \
		SimonLib/ProgramManager/programmanager.h
		

SOURCES += 	SimonLib/ProgramManager/categoryxmlreader.cpp \
		SimonLib/ProgramManager/programmanager.cpp
		
