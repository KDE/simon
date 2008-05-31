FORMS +=	Actions/modifycommands.ui \
		Actions/commandpreviewwidget.ui \
		Actions/rundialog.ui

HEADERS += 	Actions/newcommand.h \
		Actions/runcommandview.h \
		Actions/commandpreviewwidget.h \
		Actions/actionmanager.h

SOURCES += 	Actions/newcommand.cpp \
		Actions/runcommandview.cpp \
		Actions/commandpreviewwidget.cpp \
		Actions/actionmanager.cpp

include(Actions/Commands/Commands.pri)
