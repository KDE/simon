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
 * \class ImportTrainingDirectoryWorkingPage
 * \author Peter Grasch
 * \version 0.1
 * \brief Imports the given directory
 * 
 * Starts the import process per importDir():
 *   * Create Prompts
 *   * Resample WAVs to a new dest.dir
 *   * Normalize WAVs
 *   * Write codetrain.scp
 * 
 * Normalisation and resampling is done with external programs
 * (resample and normalize-audio)
 * 
 * Extends QWizardpage;
 * \todo Testing
 */
class ImportTrainingDirectoryWorkingPage : public QWizardPage{
Q_OBJECT
private:
	QProgressBar *pbMain;
	bool completed;
	int prog;
	
	bool error() { completed = false; return false; }
	
	QString extractSaid(QString source);
	QStringList* searchDir(QString dir);
	QStringList* processSounds(QStringList files, QString destDir);
	bool createPrompts(QStringList dataFiles, QString dest);

public:
    ImportTrainingDirectoryWorkingPage(QWidget *parent=0);
	bool importDir(QString dir);
	bool isComplete() { return completed; }

    ~ImportTrainingDirectoryWorkingPage() {}

};

#endif
