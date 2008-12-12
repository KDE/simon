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


#include "importdictview.h"

#include "importdictworkingpage.h"
#include "importdictselectsourcepage.h"
#include "importdictbomppage.h"
#include "importlexiconpage.h"
#include "importdictplspage.h"
#include <QLabel>
#include <QVBoxLayout>
#include <KStandardDirs>

/**
 * \brief Constructor
 * \author Peter Grasch
 * Generates a new ImportDictView object and sets up the signal/slots
 * \param parent
 * The parent of the window.
 */
ImportDictView::ImportDictView(QWidget *parent) : QWizard(parent)
{
	prevId=0;
	addPage(createIntroPage());

	addPage(createSelectSourcePage());
	addPage(createImportBOMPPage());
	addPage(createImportLexiconPage());
	addPage(createImportPLSPage());
	ImportDictWorkingPage *workingPage = createImportDictWorkingPage();
	connect(workingPage, SIGNAL(wordListImported(WordList*)), this, SIGNAL(dictGenerated(WordList*)));
	connect(workingPage, SIGNAL(wordListImported(WordList*)), this, SLOT(next()));
	connect(workingPage, SIGNAL(failed()), this, SLOT(back()));
	addPage(workingPage);

	addPage(createFinishedPage());
	setWindowTitle(i18n("Importing Dictionary"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(KStandardDirs::locate("appdata", "themes/default/importdict.png")));
}

/**
 * \brief Shows the Wizard
 * \author Peter Grasch
 */
void ImportDictView::show()
{
	QWizard::show();
}


/**
 * \brief Creates the page to import a simon lexicon
 * \author Peter Grasch
 * @return The wizardpage
 */
QWizardPage* ImportDictView::createImportLexiconPage()
{
	return new ImportLexiconPage(this);
}

/**
 * \brief Creates the page to import a simon lexicon
 * \author Peter Grasch
 * @return The wizardpage
 */
QWizardPage* ImportDictView::createImportPLSPage()
{
	return new ImportDictPLSPage(this);
}


/**
 * \brief Creates the intro page
 * \author Peter Grasch
 * 
 * This page contains a short written introduction on what is coming up
 * 
 * @return the created qwizardpage
 */
QWizardPage* ImportDictView::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(i18n("Import Dictionary"));
	QLabel *lbIntro = new QLabel(intro);
	lbIntro->setWordWrap(true);
	lbIntro->setText(i18n("This assistant will help you to import a new dictionary.\n\nA dictionary "
"contains information about the known words like how they are written and how "
"they are pronounced.\n\nThe dictionary is an essential core-component of your "
"language model. Please ensure that every dictionary that you import is of "
"high quality as it will massivly impact your recognition performance.\n\nWe "
"suggest that you use the Voxforge English Dictionary which is a HTK "
"compatible lexicon and of very high quality."));

	QVBoxLayout *lay = new QVBoxLayout(intro);
	lay->addWidget(lbIntro);
	intro->setLayout(lay);
	return intro;
}



/**
 * \brief Creates a new ImportDictSelectSourcePage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictSelectSourcePage* ImportDictView::createSelectSourcePage()
{
	return new ImportDictSelectSourcePage(this);
}

/**
 * \brief Creates a new ImportBOMPPage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictBOMPPage* ImportDictView::createImportBOMPPage() 
{
	return new ImportDictBOMPPage(this);
}


/**
 * \brief Creates a new ImportDictWorkingPage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictWorkingPage* ImportDictView::createImportDictWorkingPage()
{
	ImportDictWorkingPage *page = new ImportDictWorkingPage(this);
	connect(this, SIGNAL(rejected()), page, SLOT(abort()));
	return page;
}

/**
 * \brief Creates a new QWizardPage, builds the gui, and returns it
 * \author Peter Grasch
 * @return the created page
 */
QWizardPage* ImportDictView::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(i18n("Dictionary imported"));
	QLabel *lbFinished = new QLabel(finished);
	lbFinished->setText(i18n("The dictionary has been imported successfully.\n\nThank you for improving simon."));
	lbFinished->setWordWrap(true);
	QVBoxLayout *lay = new QVBoxLayout(finished);
	lay->addWidget(lbFinished);
	finished->setLayout(lay);
	return finished;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictView::~ImportDictView()
{
}
