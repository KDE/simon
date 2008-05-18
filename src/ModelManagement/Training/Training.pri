include(ModelManagement/Training/ImportTrainingData/ImportTrainingData.pri)
include(ModelManagement/Training/ImportTrainingTexts/ImportTrainingTexts.pri)

FORMS += 	ModelManagement/Training/trainmain.ui

HEADERS += 	ModelManagement/Training/trainingmanager.h \
		ModelManagement/Training/trainingtext.h \
		ModelManagement/Training/trainingview.h \
		ModelManagement/Training/xmltrainingtext.h

SOURCES += 	ModelManagement/Training/trainingmanager.cpp \
		ModelManagement/Training/trainingtext.cpp \
		ModelManagement/Training/trainingview.cpp \
		ModelManagement/Training/xmltrainingtext.cpp
