FORMS +=	$$PWD/modifycommands.ui \
		$$PWD/commandpreviewwidget.ui \
		$$PWD/rundialog.ui

HEADERS += 	$$PWD/newcommand.h \
		$$PWD/runcommandview.h \
		$$PWD/commandpreviewwidget.h \
		$$PWD/actionmanager.h

SOURCES += 	$$PWD/newcommand.cpp \
		$$PWD/runcommandview.cpp \
		$$PWD/commandpreviewwidget.cpp \
		$$PWD/actionmanager.cpp

include($$PWD/Commands/Commands.pri)
