
unix {
HEADERS +=	Actions/Commands/ATIntegration/dbusbackend.h
SOURCES +=	Actions/Commands/ATIntegration/dbusbackend.cpp 
}

win32 {
HEADERS +=	Actions/Commands/ATIntegration/msaabackend.h 
SOURCES += 	Actions/Commands/ATIntegration/msaabackend.cpp
}

HEADERS += 	Actions/Commands/ATIntegration/atbackend.h \
		Actions/Commands/ATIntegration/atobject.h \
		Actions/Commands/ATIntegration/ato.h \
		Actions/Commands/ATIntegration/atwatcher.h  

SOURCES += 	Actions/Commands/ATIntegration/atobject.cpp \
		Actions/Commands/ATIntegration/atwatcher.cpp 
