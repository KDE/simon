//
// C++ Interface: importtrainingdirectoryworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTTRAININGDIRECTORYWORKINGPAGE_H
#define IMPORTTRAININGDIRECTORYWORKINGPAGE_H

#include <QWizardPage>

class QProgressBar;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportTrainingDirectoryWorkingPage : public QWizardPage{
private:
	QProgressBar *pbMain;
	bool completed;
	int prog;
	
	bool error() { completed = false; return false; }
	
	QString extractSaid(QString source);
	QStringList* searchDir(QString dir);
	QStringList* processSounds(QStringList files, QString destDir);
	bool createPrompts(QStringList dataFiles, QString destDir);
	bool createScp(QStringList dataFiles, QString destDir);

public:
    ImportTrainingDirectoryWorkingPage(QWidget *parent=0);
bool importDir(QString dir);
	bool isComplete() { return completed; }

    ~ImportTrainingDirectoryWorkingPage();

};

#endif
