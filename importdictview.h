//
// C++ Interface: importdictview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTDICTVIEW_H
#define IMPORTDICTVIEW_H


#include <QWizard>
#include "word.h"

/**
 * \class ImportDictView
 * \brief Provides a dialog for importing dictonaries
 * \author Peter Grasch
 * \version 0.1
*/
class QWizardPage;
class ImportDictSelectSourcePage;
class ImportBOMPPage;
class ImportDictWiktionaryPage;
class ImportDictWorkingPage;

class ImportDictView : public QWizard {
	Q_OBJECT
private:
	int prevId;
	
signals:
	void dictGenerated(WordList*);
public slots:
	void show();
	QWizardPage* createIntroPage();
	ImportDictSelectSourcePage* 
		createSelectSourcePage();
	ImportBOMPPage* createImportBOMPPage();
	ImportDictWiktionaryPage*
		createImportWiktionaryPage();

	QWizardPage* createImportLexiconPage();
	ImportDictWorkingPage* createImportDictWorkingPage();
	QWizardPage* createFinishedPage();


public:
	enum ImportDictWizardPages {
		IntroPage=0,
		SelectTypePage=1,
		BompPage=2,
		WiktionaryPage=3,
		LexiconPage=4,
		WorkingPage=5,
		FinishedPage=6
	};
    ImportDictView(QWidget *parent=0);
    ~ImportDictView();

};

#endif
