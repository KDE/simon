/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef IMPORTDICTVIEW_H
#define IMPORTDICTVIEW_H


#include <QWizard>
#include "../word.h"

/**
 * \class ImportDictView
 * \brief Provides a dialog for importing dictonaries
 * \author Peter Grasch
 * \version 0.1
*/
class QWizardPage;
class ImportDictSelectSourcePage;
class ImportDictBOMPPage;
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
	ImportDictBOMPPage* createImportBOMPPage();
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
