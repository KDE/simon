win32{
TARGET=../../simon
}
!win32{
TARGET=../simon
}


OBJECTS_DIR = buildtmp
MOC_DIR = buildtmp
UI_DIR = buildtmp

QT += network xml

TEMPLATE = app

CONFIG += qt \
	  warn_on \
	  debug \
	  thread

RESOURCES += banners.qrc \
	      others.qrc \
	      action_icons.qrc \
	      application_icons.qrc  \
	      category_icons.qrc  \
	      other_icons.qrc


unix {
    CONFIG += x11 qdbus
    INCLUDEPATH += /usr/include/alsa \
	      /usr/X11R6/include
    LIBS += \
      /usr/lib/libportaudio.a \
      -L/usr/X11R6/lib \
      -L/usr/lib/jack \
      -L/usr/lib/alsa \
      -lX11 \
      -lXtst \
      -lasound \
      -ljack \
      -lpthread \
      -lrt
}


win32 {
    LIBS += -ldsound -lwinmm -lole32 -atls portaudio_x86.dll
}


INCLUDEPATH += 	SimonLib/SpeechGUI

FORMS +=	main.ui

SOURCES += 	main.cpp \
		simonview.cpp \
		simoncontrol.cpp
HEADERS += 	simonview.h \
		simoncontrol.h

include(Actions/Actions.pri)
include(Configuration/Configuration.pri)
include(ModelManagement/ModelManagement.pri)
include(RecognitionControl/RecognitionControl.pri)
include(SimonLib/SimonLib.pri)

TRANSLATIONS = simon_en.ts
message ("Working... (This may take a couple of seconds)")
