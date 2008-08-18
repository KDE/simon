include($$PWD/ImportTrainingData/ImportTrainingData.pri)
include($$PWD/ImportTrainingTexts/ImportTrainingTexts.pri)

FORMS += 	$$PWD/trainmain.ui

HEADERS += 	$$PWD/trainingmanager.h \
		$$PWD/trainingtext.h \
		$$PWD/trainingview.h \
		$$PWD/xmltrainingtext.h

SOURCES += 	$$PWD/trainingmanager.cpp \
		$$PWD/trainingtext.cpp \
		$$PWD/trainingview.cpp \
		$$PWD/xmltrainingtext.cpp
