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
#include <QLabel>
#include <QVBoxLayout>

/**
 * \brief Constructor
 * \author Peter Grasch
 * Generates a new ImportDictView object and sets up the signal/slots
 * \param QWidget *parent
 * The parent of the window.
 */
ImportDictView::ImportDictView(QWidget *parent) : QWizard(parent)
{
	prevId=0;
// 	list = new WordList();
// 	import = new ImportDict();
// 	connect(ui.pbImport, SIGNAL(clicked()), this, SLOT(importDict()));
// 	connect(ui.pbGetPath, SIGNAL(clicked()), this, SLOT(openFileDialog()));
// 	connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
// 	connect(ui.lePath, SIGNAL(textChanged(QString)), this, SLOT(validatePath(QString)));
	addPage(createIntroPage());

	addPage(createSelectSourcePage());
	addPage(createImportBOMPPage());
	addPage(createImportWiktionaryPage());
	ImportDictWorkingPage *workingPage = createImportDictWorkingPage();
	connect(workingPage, SIGNAL(wordListImported(WordList*)), this, SIGNAL(dictGenerated(WordList*)));
	addPage(workingPage);

	addPage(createFinishedPage());
	setWindowTitle(tr("Importiere Wörterbuch"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/importdict.png"));
	
	
	connect(this, SIGNAL(currentIdChanged( int )), this, SLOT(idChanged(int)));
}


QWizardPage* ImportDictView::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(tr("Wörterbuch importieren"));
	QLabel *lbIntro = new QLabel(intro);
	lbIntro->setText(tr("Dieser Assistent wird Ihnen dabei helfen, ein neues\nWörterbuch zu importieren.\n\nEin Wörterbuch beinhaltet Informationen über die\nbekannten Wörter wie zum Beispiel wie Sie geschrieben\nwerden und wie sie ausgesprochen werden.\n\nDas Wörterbuch ist deshalb ein wichtiger Bestandteil\nvon simon.\n\nBitte wählen Sie Ihre Quellen sorgfältig und achten Sie\ndarauf nur hochqualitatives Material zu verwenden."));

	QVBoxLayout *lay = new QVBoxLayout(intro);
	lay->addWidget(lbIntro);
	intro->setLayout(lay);
	return intro;
}

void ImportDictView::idChanged(int newId)
{
	if ((newId ==4))
	{
		if ((prevId <4))
		{
			if (((ImportDictSelectSourcePage*)page(1))->getType() == HADIFIXBOMP)
			{
				((ImportDictWorkingPage*) page(4))->importHADIFIX(
					((ImportBOMPPage*) page(2))->getFileName());
				qDebug() << "bin da";
			} else ((ImportDictWorkingPage*) page(4))->importWiktionary(
					((ImportDictWiktionaryPage*) page(3))->getPath());
		}
		else restart();
	}
	prevId = newId;
}


ImportDictSelectSourcePage* ImportDictView::createSelectSourcePage()
{
	return new ImportDictSelectSourcePage(this);
}

ImportBOMPPage* ImportDictView::createImportBOMPPage() 
{
	return new ImportBOMPPage(this);
}

ImportDictWiktionaryPage* ImportDictView::createImportWiktionaryPage()
{
	return new ImportDictWiktionaryPage(this);
}

ImportDictWorkingPage* ImportDictView::createImportDictWorkingPage()
{
	return new ImportDictWorkingPage(this);
}

QWizardPage* ImportDictView::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(tr("Wörterbuch importiert"));
	QLabel *lbFinished = new QLabel(finished);
	lbFinished->setText(tr("Das Wörterbuch wurde erfolgreich importiert und wird nun übernommen.\n\nDas noch einige Zeit dauern, währenddessen ist simon aber bereits voll einsatzfähig und der Abgleich geschieht im Hintergrund.\n\nDas neue Wörterbuch wird in spätestenens einigen Minuten voll aktiviert sein.\n\nVielen Dank, dass Sie simon verbessert haben."));
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

