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
	@author Peter Grasch <bedahr@gmx.net>
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

    ~ImportTrainingDirectory();

};

#endif
