//
// C++ Implementation: importdictview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importdictview.h"

#include "importdictworkingpage.h"
#include "importdictselectsourcepage.h"
#include "importbomppage.h"
#include "importdictwiktionarypage.h"
#include "importlexiconpage.h"
#include <QLabel>
#include <QVBoxLayout>

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
	addPage(createImportWiktionaryPage());
	addPage(createImportLexiconPage());
	ImportDictWorkingPage *workingPage = createImportDictWorkingPage();
	connect(workingPage, SIGNAL(wordListImported(WordList*)), this, SIGNAL(dictGenerated(WordList*)));
	connect(workingPage, SIGNAL(wordListImported(WordList*)), this, SLOT(next()));
	connect(workingPage, SIGNAL(failed()), this, SLOT(back()));
	addPage(workingPage);

	addPage(createFinishedPage());
	setWindowTitle(i18n("Importiere Wörterbuch"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/importdict.png"));
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
	intro->setTitle(i18n("Wörterbuch importieren"));
	QLabel *lbIntro = new QLabel(intro);
	lbIntro->setWordWrap(true);
	lbIntro->setText(i18n("Dieser Assistent wird Ihnen dabei helfen, ein neues Wörterbuch zu importieren.\n\nEin Wörterbuch beinhaltet Informationen über die bekannten Wörter wie zum Beispiel wie Sie geschrieben werden und wie sie ausgesprochen werden.\n\nDas Wörterbuch ist deshalb ein wichtiger Bestandteil von simon.\n\nWir empfehlen ein BOMP Wörterbuch aufgrund dessen hohen Qualitätsstandards.\n\nBitte wählen Sie Ihre Quellen sorgfältig und achten Sie darauf nur hochqualitatives Material zu verwenden."));

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
ImportBOMPPage* ImportDictView::createImportBOMPPage() 
{
	return new ImportBOMPPage(this);
}

/**
 * \brief Creates a new ImportDictWiktionaryPage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictWiktionaryPage* ImportDictView::createImportWiktionaryPage()
{
	return new ImportDictWiktionaryPage(this);
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
	finished->setTitle(i18n("Wörterbuch importiert"));
	QLabel *lbFinished = new QLabel(finished);
	lbFinished->setText(i18n("Das Wörterbuch wurde erfolgreich importiert und wird nun übernommen.\n\nDas noch einige Zeit dauern, währenddessen ist simon aber bereits voll einsatzfähig und der Abgleich geschieht im Hintergrund.\n\nDas neue Wörterbuch wird in spätestenens einigen Minuten voll aktiviert sein.\n\nVielen Dank, dass Sie simon verbessert haben."));
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
