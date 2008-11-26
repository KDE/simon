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


#include "importdictworkingpage.h"
#include "importdict.h"
#include <QLabel>
#include <QProgressBar>
#include <QVariant>
#include <QVBoxLayout>
#include <KUrl>
#include <KLocalizedString>
#include <KStandardDirs>
#include <kio/job.h>
#include <kio/jobuidelegate.h>

/**
 * \brief Constructor - inits the gui
 * \author Peter Grasch
 * @param parent the parent of the page
 */
ImportDictWorkingPage::ImportDictWorkingPage(QWidget* parent): QWizardPage(parent)
{
	ready = false;
	
	setTitle(i18n("Importiere Wörterbuch..."));
	QLabel *desc = new QLabel(this);
	desc->setWordWrap(true);
	desc->setText(i18n("Importiere das Wörterbuch... Je nach dessen Art und Größe kann das einige Zeit dauern.\n\nBitte haben Sie etwas Geduld...\n\n"));
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
	if (path.isEmpty()) return;

	displayStatus(i18n("Importiere Lexicon-Wörterbuch %1...", path));
	
	import->parseWordList(path, Dict::HTKLexicon, true /* remove input file when done */);
}

/**
 * \brief Tells the ImportDict class to imoprt the hadifix dict from the given path
 * @param path the path to the hadifix dict.
 */
void ImportDictWorkingPage::importHADIFIX(QString path)
{
	if (path.isEmpty()) return;

	displayStatus(i18n("Importiere Hadifix-Wörterbuch %1...", path));
	
	import->parseWordList(path, Dict::HadifixBOMP, true /* remove input file when done */);
}


QString ImportDictWorkingPage::prepareDict(KUrl url)
{
	KIO::FileCopyJob *job = KIO::file_copy(url, KStandardDirs::locateLocal("tmp", url.fileName()));
	
	if (!job->exec()) {
		job->ui()->showErrorMessage();
		return "";
	}

	return job->destUrl().path();
}


/**
 * \brief Starts the importing progress
 * \author Peter Grasch
 */
void ImportDictWorkingPage::initializePage()
{
	ready=false;
	completeChanged();
	pbMain->setMaximum(1000);

	if (field("hadifix").toBool())
	{
		importHADIFIX(prepareDict(field("bompFileName").value<KUrl>()));
	} else
		importLexicon(prepareDict(field("lexiconFilename").value<KUrl>()));
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
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictWorkingPage::~ImportDictWorkingPage()
{
    if (pbMain) pbMain->deleteLater();
    if (import) import->deleteLater();
    if (lbStatus) lbStatus->deleteLater();
}


