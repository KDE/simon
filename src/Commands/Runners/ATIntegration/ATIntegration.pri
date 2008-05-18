FORMS += 	Commands/Runners/ATIntegration/atsettings.ui

unix {
HEADERS +=	Commands/Runners/ATIntegration/dbusbackend.h
SOURCES +=	Commands/Runners/ATIntegration/dbusbackend.cpp 
}

win32 {
HEADERS +=	Commands/Runners/ATIntegration/msaabackend.h 
SOURCES += 	Commands/Runners/ATIntegration/msaabackend.cpp
}

HEADERS += 	Commands/Runners/ATIntegration/atbackend.h \
		Commands/Runners/ATIntegration/atsettings.h \
		Commands/Runners/ATIntegration/atobject.h \
		Commands/Runners/ATIntegration/ato.h \
		Commands/Runners/ATIntegration/atwatcher.h  

SOURCES += 	Commands/Runners/ATIntegration/atobject.cpp \
		Commands/Runners/ATIntegration/atsettings.cpp \
		Commands/Runners/ATIntegration/atwatcher.cpp 
