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
private slots:
	void idChanged(int id);
public slots:
	void show();
	QWizardPage* createIntroPage();
	ImportDictSelectSourcePage* 
		createSelectSourcePage();
	ImportBOMPPage* createImportBOMPPage();
	ImportDictWiktionaryPage*
		createImportWiktionaryPage();
	ImportDictWorkingPage* createImportDictWorkingPage();
	QWizardPage* createFinishedPage();
	



public:
    ImportDictView(QWidget *parent=0);
// 	WordList* getList() {return list;}
    ~ImportDictView();

};

#endif
