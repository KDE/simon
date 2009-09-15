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



#include "importtrainingtextworkingpage.h"
#include "importtrainingtextlocalpage.h"
#include "xmltrainingtextlist.h"

#include <simonlogging/logger.h>
#ifdef SIMON_SCENARIOS
#include <speechmodelmanagement_scenario/trainingmanager.h>
#else
#include <speechmodelmanagement/trainingmanager.h>
#endif

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <QTimer>

#include <KUrl>
#include <KMimeType>
#include <KStandardDirs>
#include <KMessageBox>
#include <kio/job.h>
#include <KFilterDev>
#include <kencodingdetector.h>
#include <kio/jobuidelegate.h>


/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent sets the parent to the given parent
 */
ImportTrainingTextWorkingPage::ImportTrainingTextWorkingPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(i18n("Text is being added"));
	ui.setupUi(this);
}

/**
 * \brief Sets the path to the given Path
 * @param path The path to import from (this can be a url too)
 * \author Peter Grasch
 */
void ImportTrainingTextWorkingPage::startImport(KUrl path)
{
	bool removeInput=false;
	if (!path.isLocalFile())
	{
		Logger::log(i18n("[INF] Starting remove import from \"%1\"", path.prettyUrl()));

		KUrl tmpPath = KUrl(KStandardDirs::locateLocal("tmp", "tmp_trainingstext.xml"));

		KIO::FileCopyJob *job = KIO::file_copy(path, tmpPath, 
						-1 /* no special permissions */, KIO::Overwrite);
		
		if (!job->exec())
		{
			job->ui()->showErrorMessage();
			return;
		}

		path = tmpPath;
		removeInput=true;
	}
	if (KMimeType::findByFileContent(path.path())->is("application/xml"))
		processText(path.path(), removeInput);
	else parseFile(path.path());
	
}

void ImportTrainingTextWorkingPage::initializePage()
{
	ui.pbProgress->setMaximum(0);
	if (field("importTrainingTextLocal").toBool())
	{
		startImport(field("importTrainingTextLFilename").value<KUrl>());
	} else startImport(KUrl(field("textDownloadURL").toString()));
}


/**
 * \brief Processes the text at the given path 
 * \author Peter Grasch
 * @param path the path to the new text
 */
void ImportTrainingTextWorkingPage::processText(QString path, bool removeInput)
{
	QFileInfo fi = QFileInfo(path);
	QFile::copy(path, KStandardDirs::locateLocal("appdata", "texts/")+"/"+fi.fileName());

	if (removeInput)
		QFile::remove(path);
	
	//wizard()->next();
	QTimer::singleShot(500, wizard(), SLOT(next()));
}

/**
 * \brief Parses the textfile at the given path
 * @param path Path to the textfile
 * @author Peter Grasch
 */
void ImportTrainingTextWorkingPage::parseFile(QString path)
{
	ui.pbProgress->setMaximum(3);
	ui.pbProgress->setValue(0);

	QString tmpPath = KStandardDirs::locateLocal("tmp", "simontrainingstextimport");
	KIO::FileCopyJob *job = KIO::file_copy(path, tmpPath, -1, KIO::Overwrite);
	if (!job->exec()) {
		job->ui()->showErrorMessage();
		return;
	}

	ui.pbProgress->setValue(1);

	QIODevice *file = KFilterDev::deviceForFile(tmpPath, KMimeType::findByFileContent(tmpPath)->name());
	if ((!file) || (!file->open(QIODevice::ReadOnly)))
		return;

	//TODO: determine encoding
	QTextStream ts(file);
	QString encoding = field("importTrainingTextLEncoding").toString();
	if (encoding == i18n("Automatic"))
	{
		//read first 5000 bytes and run encoding detection
		//seek back to the beginning and parse file using the guessed encoding
		QByteArray preview = file->peek(5000);
		KEncodingDetector detector;
		detector.setAutoDetectLanguage(KEncodingDetector::WesternEuropean);
		QString out=detector.decode(preview);
		ts.setCodec(QTextCodec::codecForName(detector.encoding()));
	} else 
		ts.setCodec(QTextCodec::codecForName(encoding.toAscii()));

	ui.pbProgress->setValue(2);

	QStringList sents;
	QString tmp;
	
	int sentend;
	QRegExp reg("(\\.|\\!|\\?)"); //TODO: maybe add an option to treat "\n" as sentence-stopper
	QRegExp spec("(\\.\\.\\.|\\!\\!\\!|\\?\\?\\?)");
	QString currentLine;
	while ((!ts.atEnd()) || (!tmp.trimmed().isEmpty()))
	{
		if (!ts.atEnd())
			currentLine = ts.readLine();
		else currentLine = "";
		
		QString currentProcessQueue = tmp+currentLine;
		
		int regIndex = currentProcessQueue.indexOf(reg);
		int specIndex = currentProcessQueue.indexOf(spec);
		
		if (regIndex != -1)
		{
			if ((specIndex <=regIndex) && (specIndex != -1))
				sentend = specIndex+3;
			else
				sentend = regIndex+1;
		} else sentend = currentProcessQueue.length();
		
	
		QString sentence = QString(currentProcessQueue).left(sentend).trimmed();
		sentence.remove("\"");
		sentence.remove(",");
		sentence.remove(".");
		sentence.remove("#");
		sentence.remove("`");
		sentence.remove("!");
		sentence.remove("?");
		sentence.replace(QRegExp("( |^)'"), " ");
		sentence.remove(".");
		sentence.replace("-", " ");
		sentence.replace("\n", " ");
		sentence.replace(QRegExp("  *"), " ");
		if (!sentence.isEmpty()) sents << sentence;
		
		tmp = currentProcessQueue.mid(sentend).trimmed()+" ";
	}
	file->close();
	delete file;

	QFile::remove(KStandardDirs::locateLocal("tmp", "simontrainingstextimport"));

	if (!TrainingManager::getInstance()->saveTrainingsText(field("importTrainingTextLTextname").toString(), sents))
		KMessageBox::error(this, i18n("Couldn't store Trainingstext"));
	ui.pbProgress->setValue(3);
	QTimer::singleShot(500, wizard(), SLOT(next()));
}

