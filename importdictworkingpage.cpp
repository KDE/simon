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
#include <QDebug>
#include <QVBoxLayout>
#include "bunzip.h"
#include "quickdownloader.h"
#include "importdict.h"
#include "quickunpacker.h"

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
}



bool ImportDictWorkingPage::isComplete() const
{
	return ready;
}


void ImportDictWorkingPage::importHADIFIX(QString path)
{
	ready=false;
	completeChanged();
	
	displayStatus(tr("Importiere Hadifix-Wörterbuch %1...").arg(path));
	pbMain->setMaximum(1000);
	
	import->parseWordList(path, 1);

	connect(import, SIGNAL(finished(WordList*)), this, SLOT(setCompleted()));
}

void ImportDictWorkingPage::importWiktionary(QString url)
{
	if (url.startsWith("http"))
	{
		pbMain->setMaximum(0);
		displayStatus(tr("Lade Wörterbuch herunter..."));
		QuickDownloader *loader = new QuickDownloader(this);
		connect(loader, SIGNAL(downloadFinished(QString)), this, 
				SLOT(importWiktionaryFile(QString)));
		loader->download(url);
	} else importWiktionaryFile(url);
}


void ImportDictWorkingPage::displayStatus(QString status)
{
	lbStatus->setText(status);
}


void ImportDictWorkingPage::displayProgress(int progress)
{
	pbMain->setValue(progress);
}

void ImportDictWorkingPage::importWiktionaryFile(QString path)
{
	if (path.endsWith("bzip2"))
	{
		QuickUnpacker *unpacker = new QuickUnpacker(this);
		unpacker->unpack(path);
	}
	displayStatus(tr("Importiere Wiktionary-Wörterbuch %1...").arg(path));
	pbMain->setMaximum(1000);
	
	import->parseWordList(path, 2);

	setCompleted();
}

void ImportDictWorkingPage::unpackFile(QString path)
{
	
}

ImportDictWorkingPage::~ImportDictWorkingPage()
{
}


