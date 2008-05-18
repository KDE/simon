message ( "Processing GeneralConfiguration..." )

FORMS += 	GeneralConfiguration/externalprogramms.ui \
		GeneralConfiguration/generalsettingsdlg.ui \
		GeneralConfiguration/internetextensiondlg.ui \
		GeneralConfiguration/logmanagerdlg.ui \
		GeneralConfiguration/networksettingsdlg.ui \
		GeneralConfiguration/passwordsettingsdlg.ui \
		GeneralConfiguration/soundsettingsdlg.ui \
		GeneralConfiguration/systemviewdlg.ui

HEADERS += 	GeneralConfiguration/externalprogrammanager.h \
		GeneralConfiguration/generalsettings.h \
		GeneralConfiguration/internetextensionsettings.h \
		GeneralConfiguration/networksettings.h \
		GeneralConfiguration/passwordsettings.h \
		GeneralConfiguration/soundsettings.h \
		GeneralConfiguration/systemview.h  \
		GeneralConfiguration/logview.h

SOURCES += 	GeneralConfiguration/externalprogrammanager.cpp \
		GeneralConfiguration/generalsettings.cpp \
		GeneralConfiguration/internetextensionsettings.cpp \
		GeneralConfiguration/networksettings.cpp \
		GeneralConfiguration/soundsettings.cpp \
		GeneralConfiguration/passwordsettings.cpp \
		GeneralConfiguration/systemview.cpp  \
		GeneralConfiguration/logview.cpp

QT += network

include(GeneralConfiguration/FirstRun/FirstRun.pri)
