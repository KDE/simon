//
// C++ Interface: importgrammarworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTGRAMMARWORKINGPAGE_H
#define IMPORTGRAMMARWORKINGPAGE_H

#include <QWizardPage>
#include "ui_importgrammarworkingpage.h"
#include "../../WordList/word.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportGrammar;
class ImportGrammarWorkingPage : public QWizardPage
{
Q_OBJECT
signals:
	void grammarCreated(QStringList);
private:
	bool completed;
	Ui::ImportGrammarWorkingPage ui;
	ImportGrammar *grammarImporter;

public slots:
	void cancel();

private slots:
	void initializePage();
	void printStatus(QString);
	void displayFileProgress(int progress, int max);
	void displayWholeProgress(int progress, int max);
	void processCompletion();

public:
    ImportGrammarWorkingPage(QWidget* parent);

	bool isComplete() const { return completed; }
    ~ImportGrammarWorkingPage();

};

#endif
