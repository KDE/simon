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

RESOURCES += $$PWD/banners.qrc \
	      $$PWD/others.qrc \
	      $$PWD/action_icons.qrc \
	      $$PWD/application_icons.qrc  \
	      $$PWD/category_icons.qrc  \
	      $$PWD/other_icons.qrc


unix {
    CONFIG += x11 qdbus
    INCLUDEPATH += /usr/include/alsa \
	      /usr/X11R6/include
    LIBS += \
      /usr/lib64/libportaudio.a \
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


INCLUDEPATH += 	$$PWD/SimonLib/SpeechGUI

FORMS +=	$$PWD/main.ui

SOURCES += 	$$PWD/main.cpp \
		$$PWD/simonview.cpp \
		$$PWD/simoncontrol.cpp
HEADERS += 	$$PWD/simonview.h \
		$$PWD/simoncontrol.h

include($$PWD/Actions/Actions.pri)
include($$PWD/Configuration/Configuration.pri)
include($$PWD/ModelManagement/ModelManagement.pri)
include($$PWD/RecognitionControl/RecognitionControl.pri)
include($$PWD/SimonLib/SimonLib.pri)

TRANSLATIONS = $$PWD/simon_en.ts
message ("Working... (This may take a couple of seconds)")
