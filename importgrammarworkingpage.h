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
#include "word.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class WordListManager;
class ImportGrammar;
class ImportGrammarWorkingPage : public QWizardPage
{
Q_OBJECT
signals:
	void grammarCreated(QStringList);
private:
	bool completed;
	Ui::ImportGrammarWorkingPage ui;
	WordListManager *wordListManager;
	ImportGrammar *grammarImporter;

private slots:
	void initializePage();
	void printStatus(QString);
	void displayFileProgress(int progress, int max);
	void displayWholeProgress(int progress, int max);
public:
    ImportGrammarWorkingPage(WordListManager *wordListManager, QWidget* parent);

	void setWordListManager(WordListManager*);
	bool isComplete() { return completed; }
    ~ImportGrammarWorkingPage();

};

#endif
