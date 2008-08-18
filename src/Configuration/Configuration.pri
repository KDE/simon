message ( "Processing Configuration..." )

FORMS += 	$$PWD/externalprogramms.ui \
		$$PWD/generalsettingsdlg.ui \
		$$PWD/internetextensiondlg.ui \
		$$PWD/logmanagerdlg.ui \
		$$PWD/networksettingsdlg.ui \
		$$PWD/passwordsettingsdlg.ui \
		$$PWD/systemviewdlg.ui

HEADERS += 	$$PWD/externalprogrammanager.h \
		$$PWD/generalsettings.h \
		$$PWD/internetextensionsettings.h \
		$$PWD/networksettings.h \
		$$PWD/passwordsettings.h \
		$$PWD/systemview.h  \
		$$PWD/logview.h

SOURCES += 	$$PWD/externalprogrammanager.cpp \
		$$PWD/generalsettings.cpp \
		$$PWD/internetextensionsettings.cpp \
		$$PWD/networksettings.cpp \
		$$PWD/passwordsettings.cpp \
		$$PWD/systemview.cpp  \
		$$PWD/logview.cpp 

include($$PWD/FirstRun/FirstRun.pri)
