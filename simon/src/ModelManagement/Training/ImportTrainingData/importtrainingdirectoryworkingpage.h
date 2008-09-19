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
#include "ui_importtrainingdataworkingdlg.h"

class ImportTrainingData;


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
 * Normalization and resampling is done with external programs
 * (resample and normalize-audio)
 * 
 * Extends QWizardpage;
 */
class ImportTrainingDirectoryWorkingPage : public QWizardPage{
    Q_OBJECT

signals:
	void done();
private:
	Ui::ImportTrainingDataWorkingDlg ui;
	
	ImportTrainingData *importer;
	bool completed;
	
	void error() { completed = false; }

private slots:
	void setComplete();
	void displayProgress(int now, int max);
	void displayStatus(QString status);
	void displayError(QString error);
public:
	ImportTrainingDirectoryWorkingPage(QWidget *parent=0);
	~ImportTrainingDirectoryWorkingPage() {}
// 	bool importDir(QString dir);
	bool isComplete() const { return completed; }
	void initializePage();

};

#endif
