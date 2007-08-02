//
// C++ Interface: importtrainingdirectory
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTTRAININGDIRECTORY_H
#define IMPORTTRAININGDIRECTORY_H

#include <QWizard>

class QWidget;
class QWizardPage;
class ImportTrainingDirectoryIntroPage;
class ImportTrainingDirectoryWorkingPage;


/**
  \class ImportTrainingDirectory
  \author Peter Grasch
  \brief The wizard to import a training-directory

  The wizard is there to migrated tagged-audio-data into the language
  model.
  It does that using the filename to determine what the user said;
  e.g.: this_is_a_test.wav
  would be the line: *\/this_is_a_test.wav THIS IS A TEST
  in the prompts file and so on.

  The wizard walks the directory recursive;


  Extends QWizard;

  \todo The source/dest-dir should be configurable
  
*/
class ImportTrainingDirectory : public QWizard {
	Q_OBJECT
private slots:
	void idChanged(int id);
private:
	int prevId;
	ImportTrainingDirectoryIntroPage* createIntroPage();
	ImportTrainingDirectoryWorkingPage* createWorkingPage();
	QWizardPage* createFinishedPage();
public:
    ImportTrainingDirectory(QWidget *parent=0);

    ~ImportTrainingDirectory()
	{
	}

};

#endif
