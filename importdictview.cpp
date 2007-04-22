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


/**
 * \brief Constructor
 * \author Peter Grasch
 * Generates a new ImportDictView object and sets up the signal/slots
 * \param QWidget *parent
 * The parent of the window.
 */
ImportDictView::ImportDictView(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	list = new WordList();
	import = new ImportDict();
	connect(ui.pbImport, SIGNAL(clicked()), this, SLOT(importDict()));
	connect(ui.pbGetPath, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

/**
 * \brief Displays the given Status (explaining the progress
 * \author Peter Grasch
 * \param QString status
 * The current Status message
 */
void ImportDictView::displayStatus(QString status)
{
	ui.lbStatus->setText(status);
}

/**
 * \brief Displays the given progress
 * \author Peter Grasch
 * \param int progress
 * The current progress (between 0 and 1000)
 */
void ImportDictView::displayProgress(int progress)
{
	ui.pbProgress->setValue(progress);
}

/**
 * \brief Displays a Filedialog to choose the file to import.
 * \author Peter Grasch
 */
void ImportDictView::openFileDialog()
{
	QString file= QFileDialog::getOpenFileName(this, "Wiktionary kompatibles Wörterbuch öffnen", ".", "*.xml");
	ui.lePath->setText(file);
}

/**
 * \brief Sets the progressbar to displays a real progress - not just a "please wait" thingy
 * \author Peter Grasch
 * 
 * While we open the file there is no information of how long it will take.
 * But once we opened it we know how long it will take to finish. This is why
 * we just provide a waiting... progressbar (ie maximum=0) while wo open the file
 * afterwards we provide a nice, clean progressbar (maximum=1000).
 * 
 * This function basically sets the maximum to 1000.
 */
void ImportDictView::openingFinished()
{
	ui.pbProgress->setMaximum(1000);
}

/**
 * \brief Imports the dictionary 
 * \author Peter Grasch
 * Makes the progressbar displaying a waiting... thingy (we don't know how long it will take;
 * Connect the signal/slots
 * Start parsing the WordList
 */
void ImportDictView::importDict()
{
	ui.pbProgress->setMaximum(0);
	ui.swMain->setCurrentIndex(1);
	connect (import, SIGNAL(status(QString)), this, SLOT(displayStatus(QString)));
	connect (import, SIGNAL(progress(int)), this, SLOT(displayProgress(int)));
	connect (import, SIGNAL(finished()), this, SLOT(finishedImporting()));
	
	connect (import, SIGNAL(opened()), this, SLOT(openingFinished()));
	import->parseWordList(ui.lePath->text());
}

/**
 * \brief We are done importing: Close the dialog and set the list (member) to the retrieved wordlist
 * \author Peter Grasch
 */
void ImportDictView::finishedImporting()
{
	list = import->getWordList();
	accept();
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictView::~ImportDictView()
{
}

