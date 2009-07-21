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
#include <speechmodelbase/word.h>

#include <speechmodelmanagement/wordlisttype.h>

/**
 * \class ImportDictView
 * \brief Provides a dialog for importing dictonaries
 * \author Peter Grasch
 * \version 0.1
*/
class QWizardPage;
class ImportDictSelectSourcePage;
class ImportDictBOMPPage;
class ImportDictWorkingPage;

class ImportDictView : public QWizard {
	Q_OBJECT
private:
	ImportDictWorkingPage *workingPage;
	
signals:
	void dictGenerated(WordList*, WordListTarget::WordListType type);

private slots:
	void dictReady(WordList * list);

public slots:
	QWizardPage* createIntroPage();
	ImportDictSelectSourcePage* 
		createSelectSourcePage();
	ImportDictBOMPPage* createImportBOMPPage();

	QWizardPage* createImportLexiconPage();
	QWizardPage* createImportPLSPage();
	QWizardPage* createImportSPHINXPage();
	ImportDictWorkingPage* createImportDictWorkingPage();
	QWizardPage* createFinishedPage();


public:
	enum ImportDictWizardPages {
		IntroPage=0,
		SelectTypePage=1,
		BompPage=2,
		LexiconPage=3,
		PLSPage=4,
		SPHINXPage=5,
		WorkingPage=6,
		FinishedPage=7
	};
    ImportDictView(QWidget *parent=0);
    ~ImportDictView();

};

#endif
