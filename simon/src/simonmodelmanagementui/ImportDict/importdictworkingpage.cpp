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
#include <KFilterDev>
#include <KMimeType>
#include <kio/job.h>
#include <kio/jobuidelegate.h>
#include  <kencodingdetector.h>

/**
 * \brief Constructor - inits the gui
 * \author Peter Grasch
 * @param parent the parent of the page
 */
ImportDictWorkingPage::ImportDictWorkingPage(QWidget* parent) : QWizardPage(parent),
	pbMain(new QProgressBar(this)),
	import(new ImportDict(this)),
	lbStatus(new QLabel(this)),
	ready(false)
{
	
	setTitle(i18n("Importing dictionary..."));
	QLabel *desc = new QLabel(this);
	desc->setWordWrap(true);
	desc->setText(i18n("Importing the dictionary... Depending on its type and size this can take a "
"while. Please be patient...\n"));
	
	pbMain->setMaximum(0);
	
	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addWidget(desc);
	lay->addWidget(pbMain);
	lay->addWidget(lbStatus);
	setLayout(lay);
	
	connect(import, SIGNAL(status(QString)), this, SLOT(displayStatus(QString)));
	connect(import, SIGNAL(progress(int, int)), this, SLOT(displayProgress(int, int)));
	connect(import, SIGNAL(successful()), this, SIGNAL(done()));
	connect(import, SIGNAL(failed()), this, SIGNAL(failed()));

	connect(import, SIGNAL(successful()), this, SLOT(setCompleted()));
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

QString ImportDictWorkingPage::guessEncoding(const QString& path)
{
	QIODevice *dict = KFilterDev::deviceForFile(path,
					KMimeType::findByFileContent(path)->name());
	if ((!dict) || (!dict->open(QIODevice::ReadOnly)))
		return "";
	
	QByteArray preview = dict->readAll();
	dict->close();
	delete dict;

	KEncodingDetector detector;
	detector.setAutoDetectLanguage(KEncodingDetector::WesternEuropean);
	QString out=detector.decode(preview);
	return detector.encoding();
}


/**
 * \brief Imports a lexicon
 * @param path the path to the lexicon dict.
 * \author Peter Grasch
 */
void ImportDictWorkingPage::importLexicon(QString path)
{
	if (path.isEmpty()) return;

	displayStatus(i18n("Importing HTK-Dictionary %1...", path));
	
	QString encoding = field("lexiconEncoding").toString();
	if (encoding == i18n("Automatic"))
		encoding = guessEncoding(path);
	import->parseWordList(path, encoding, Dict::HTKLexicon, true /* remove input file when done */);
}

/**
 * \brief Tells the ImportDict class to imoprt the hadifix dict from the given path
 * @param path the path to the hadifix dict.
 */
void ImportDictWorkingPage::importHADIFIX(QString path)
{
	if (path.isEmpty()) return;

	displayStatus(i18n("Importing HADIFIX-dictionary %1...", path));
	
	QString encoding;
	if (field("bompSource").toBool()) { //true means a manual import
		encoding = field("bompEncoding").toString();
		if (encoding == i18n("Automatic"))
			encoding = guessEncoding(path);
	} else encoding = "ISO-8859-15"; //download is always ISO-8859-15

	import->parseWordList(path, encoding, Dict::HadifixBOMP, true /* remove input file when done */);
}

void ImportDictWorkingPage::importPLS(QString path)
{
	if (path.isEmpty()) return;

	displayStatus(i18n("Importing PLS-dictionary %1...", path));
	
	import->parseWordList(path, "" /*encoding is determined by SAX*/, Dict::PLS, true /* remove input file when done */);
}


void ImportDictWorkingPage::importSPHINX(QString path)
{
	if (path.isEmpty()) return;

	displayStatus(i18n("Importing SPHINX-dictionary %1...", path));
	
	QString encoding = field("sphinxEncoding").toString();
	if (encoding == i18n("Automatic"))
		encoding = guessEncoding(path);
	import->parseWordList(path, encoding, Dict::SPHINX, true /* remove input file when done */);
}


void ImportDictWorkingPage::importJulius(QString path)
{
	if (path.isEmpty()) return;

	displayStatus(i18n("Importing Julius vocabulary %1...", path));
	
	QString encoding = field("juliusVocabularyEncoding").toString();
	if (encoding == i18n("Automatic"))
		encoding = guessEncoding(path);
	import->parseWordList(path, encoding, Dict::JuliusVocabulary, true /* remove input file when done */);
}


QList<Word*>* ImportDictWorkingPage::getCurrentWordList()
{
	return import->getCurrentWordList();
}

QString ImportDictWorkingPage::prepareDict(KUrl url)
{
	KIO::FileCopyJob *job = KIO::file_copy(url, KStandardDirs::locateLocal("tmp", url.fileName()), -1, KIO::Overwrite);
	
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
		if (field("bompSource").toBool()) //true means a manual import
			importHADIFIX(prepareDict(field("bompFileName").value<KUrl>()));
		else 
			importHADIFIX(KStandardDirs::locateLocal("tmp", "bomp"));
	} else if (field("lexicon").toBool())
		importLexicon(prepareDict(field("lexiconFilename").value<KUrl>()));
	else if (field("pls").toBool())
		importPLS(prepareDict(field("plsFilename").value<KUrl>()));
	else if (field("julius").toBool())
		importJulius(prepareDict(field("juliusVocabularyFilename").value<KUrl>()));
	else
		importSPHINX(prepareDict(field("sphinxFilename").value<KUrl>()));
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
void ImportDictWorkingPage::displayProgress(int progress, int max)
{
	if (pbMain->value() != progress)
		pbMain->setValue(progress);
	if (pbMain->maximum() != max)
		pbMain->setMaximum(max);
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


