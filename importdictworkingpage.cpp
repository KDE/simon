//
// C++ Implementation: importdictworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importdictworkingpage.h"
#include <QLabel>
#include <QProgressBar>
#include <QVariant>
#include <QVBoxLayout>
#include "bunzip.h"
#include "quickdownloader.h"
#include "importdict.h"
#include "quickunpacker.h"

/**
 * \brief Constructor - inits the gui
 * \author Peter Grasch
 * @param parent the parent of the page
 */
ImportDictWorkingPage::ImportDictWorkingPage(QWidget* parent): QWizardPage(parent)
{
	ready = false;
	
	setTitle(tr("Importiere Wörterbuch..."));
	QLabel *desc = new QLabel(this);
	desc->setText(tr("Importiere das Wörterbuch... Je nach dessen Art und Größe kann das einige Zeit dauern.\n\nBitte haben Sie etwas Geduld...\n\n"));
	desc->setWordWrap(true);
	lbStatus = new QLabel(this);
	
	pbMain = new QProgressBar(this);
	
	pbMain->setMaximum(0);
	
	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addWidget(desc);
	lay->addWidget(pbMain);
	lay->addWidget(lbStatus);
	setLayout(lay);
	
	import = new ImportDict(this);
	connect(import, SIGNAL(status(QString)), this, SLOT(displayStatus(QString)));
	connect(import, SIGNAL(progress(int)), this, SLOT(displayProgress(int)));
	connect(import, SIGNAL(finished(WordList*)), this, SIGNAL(wordListImported(WordList*)));

	connect(import, SIGNAL(finished(WordList*)), this, SLOT(setCompleted()));
}


/**
 * \brief Aborts the process
 * \author Peter Grasch
 */
void ImportDictWorkingPage::abort()
{
	import->deleteDict();
}

/**
 * \brief Returns true if we completed the wizard
 * \author Peter Grasch
 * @return completed?
 */
bool ImportDictWorkingPage::isComplete() const
{
	return ready;
}



/**
 * \brief Imports a lexicon
 * @param path the path to the lexicon dict.
 * \author Peter Grasch
 */
void ImportDictWorkingPage::importLexicon(QString path)
{
	ready=false;
	completeChanged();
	
	displayStatus(tr("Importiere Lexicon-Wörterbuch %1...").arg(path));
	pbMain->setMaximum(1000);
	
	import->parseWordList(path, 3);
}

/**
 * \brief Tells the ImportDict class to imoprt the hadifix dict from the given path
 * @param path the path to the hadifix dict.
 */
void ImportDictWorkingPage::importHADIFIX(QString path)
{
	ready=false;
	completeChanged();
	
	displayStatus(tr("Importiere Hadifix-Wörterbuch %1...").arg(path));
	pbMain->setMaximum(1000);
	
	import->parseWordList(path, 1);
}

/**
 * \brief Imports the wiki at the given url (local or remote)
 * \author Peter Grasch
 * @param url the url to import from
 */
void ImportDictWorkingPage::importWiktionary(QString url)
{
	if (url.startsWith("http"))
	{
		pbMain->setMaximum(0);
		displayStatus(tr("Lade Wörterbuch herunter..."));
		QuickDownloader *loader = new QuickDownloader(this);
		connect(loader, SIGNAL(aborted()), this, SIGNAL(failed()));
		connect(loader, SIGNAL(errorOccured(QString)), this, SIGNAL(failed()));
		connect(loader, SIGNAL(downloadFinished(QString)), this, 
				SLOT(unpackWikiIfNecessary(QString)));
		loader->download(url, "wiki_tmp.bz2");
	} else unpackWikiIfNecessary(url);
}

/**
 * \brief Unpacks the wiki if necessary
 * \author Peter Grasch
 * @param file the wiki
 * \todo The distinction if the file is packed or not is entirely done by using the filename
 */
void ImportDictWorkingPage::unpackWikiIfNecessary(QString file)
{
	if (file.endsWith("bz2"))
	{
		QuickUnpacker *unpacker = new QuickUnpacker(this);
		unpacker->unpack(file);
		connect(unpacker, SIGNAL(unpackedTo(QString)), this, 
			SLOT(importWiktionaryFile(QString)));
	} else importWiktionaryFile(file);

	QuickDownloader *dl = qobject_cast<QuickDownloader*>(sender());
	if (dl) dl->deleteLater();
}

/**
 * \brief Displays the given status
 * \author Peter Grasch
 * @param status The status to set to
 */
void ImportDictWorkingPage::displayStatus(QString status)
{
	lbStatus->setText(status);
}


/**
 * \brief Displays the given progress
 * @param progress The progress to set to
 */
void ImportDictWorkingPage::displayProgress(int progress)
{
	pbMain->setValue(progress);
}

/**
 * \brief Starts the importing progress
 * \author Peter Grasch
 */
void ImportDictWorkingPage::initializePage()
{
	if (field("hadifix").toBool())
		importHADIFIX(field("bompFileName").toString());
	else if (field("wiktionary").toBool())
	{
		QString path;
		if (field("importWikiLocal").toBool()) path = field("wikiFileName").toString();
		if (field("importWikiRemote").toBool()) path = field("wikiRemoteURL").toString();

		importWiktionary(path);
	} else
		importLexicon(field("lexiconFilename").toString());
}

/**
 * \brief Imports the wiktionary at <path>
 * @param path the path of the (downloaded and extracted) wiki
 */
void ImportDictWorkingPage::importWiktionaryFile(QString path)
{
	QuickUnpacker *qu = qobject_cast<QuickUnpacker*>(sender());
	if (qu) qu->deleteLater();
	displayStatus(tr("Importiere Wiktionary-Wörterbuch %1...").arg(path));
	pbMain->setMaximum(1000);
	
	import->parseWordList(path, 2);
	
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictWorkingPage::~ImportDictWorkingPage()
{
    if (pbMain) pbMain->deleteLater();
    if (import) import->deleteLater();
    if (lbStatus) lbStatus->deleteLater();
}


