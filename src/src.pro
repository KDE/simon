TARGET=../simon
TRANSLATIONS += simon_en.ts simon_tob.ts

OBJECTS_DIR = buildtmp
MOC_DIR = buildtmp
UI_DIR = buildtmp

QT += network xml

TEMPLATE = app

CONFIG += qt \
	  debug \
	  warn_on \
	  thread

RESOURCES += banners.qrc \
	      others.qrc \
	      action_icons.qrc \
	      application_icons.qrc  \
	      category_icons.qrc  \
	      other_icons.qrc


unix {
    CONFIG += x11 qdbus
    DEFINES += __LINUX_ALSA__
    INCLUDEPATH += /usr/include/alsa \
	      /usr/X11R6/include
    LIBS += \
      -L/usr/X11R6/lib \
      -L/usr/lib/alsa-lib \
      -lX11 \
      -lXtst \
      -lasound
}


win32 {
    DEFINES += __WINDOWS_DS__
    LIBS += -ldsound -lwinmm -lole32 -oleacc -atls
}

INCLUDEPATH += 	SimonLib/SpeechGUI

FORMS +=	main.ui

SOURCES += 	main.cpp \
		simonview.cpp \
		simoncontrol.cpp
HEADERS += 	simonview.h \
		simoncontrol.h

SUBDIRS += 	Commands \
		GeneralConfiguration \
		ModelManagement \
		RecognitionControl \
		SimonLib
include(Commands/Commands.pri)
include(GeneralConfiguration/GeneralConfiguration.pri)
include(ModelManagement/ModelManagement.pri)
include(RecognitionControl/RecognitionControl.pri)
include(SimonLib/SimonLib.pri)

message ("Working... (This may take a couple of seconds)")
