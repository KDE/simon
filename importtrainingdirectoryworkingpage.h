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
class PostProcessing;

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
	PostProcessing *pp;
	bool completed;
	int prog;
	
	void error() { completed = false; }
	
	QString extractSaid(QString source);
	QStringList* searchDir(QString dir);
	QStringList* processSounds(QStringList files, QString destDir);
	bool createPrompts(QStringList dataFiles);

public:
    ImportTrainingDirectoryWorkingPage(QWidget *parent=0);
	bool importDir(QString dir);
	bool isComplete() { return completed; }
	void initializePage();

    ~ImportTrainingDirectoryWorkingPage();

};

#endif
