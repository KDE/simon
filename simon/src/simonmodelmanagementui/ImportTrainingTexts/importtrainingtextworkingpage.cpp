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
#include <speechmodelmanagement/trainingmanager.h>

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <KUrl>
#include <KMimeType>
#include <KStandardDirs>
#include <KMessageBox>
#include <kio/job.h>
#include <kio/jobuidelegate.h>


/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent sets the parent to the given parent
 */
ImportTrainingTextWorkingPage::ImportTrainingTextWorkingPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(i18n("Text wird hinzugefügt"));
	ui.setupUi(this);
}

/**
 * \brief Sets the path to the given Path
 * @param path The path to import from (this can be a url too)
 * \author Peter Grasch
 */
void ImportTrainingTextWorkingPage::startImport(KUrl path)
{
	if (!path.isLocalFile())
	{
		Logger::log(i18n("[INF] Starte Remote Import von \"%1\"", path.prettyUrl()));

		KUrl tmpPath = KUrl(KStandardDirs::locateLocal("tmp", "tmp_trainingstext.xml"));

		KIO::FileCopyJob *job = KIO::file_copy(path, tmpPath, 
						-1 /* no special permissions */, KIO::Overwrite);
		
		if (!job->exec())
		{
			job->ui()->showErrorMessage();
			return;
		}

		path = tmpPath;
	}
	if (KMimeType::findByFileContent(path.path())->is("application/xml"))
		processText(path.path());
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
void ImportTrainingTextWorkingPage::processText(QString path)
{
	QFileInfo fi = QFileInfo(path);
	QFile::copy(path, KStandardDirs::locateLocal("appdata", "texts/")+"/"+fi.fileName());
	QFile::remove(path);
	
	wizard()->next();
}

/**
 * \brief Parses the textfile at the given path
 * @param path Path to the textfile
 * @author Peter Grasch
 */
void ImportTrainingTextWorkingPage::parseFile(QString path)
{
	ui.pbProgress->setMaximum(1);
	ui.pbProgress->setValue(0);
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return;
	

	QStringList sents;
	QString tmp;
	QTextStream ts(&file);
	
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
		sentence.remove("'");
		sentence.remove("`");
		sentence.remove("!");
		sentence.remove("?");
		sentence.remove(".");
		sentence.replace("-", " ");
		sentence.replace("\n", " ");
		sentence.replace(QRegExp("  *"), " ");
		if (!sentence.isEmpty()) sents << sentence;
		
		tmp = currentProcessQueue.mid(sentend).trimmed()+" ";
	}
	file.close();
	QFileInfo fi = QFileInfo(path);

	if (!TrainingManager::getInstance()->saveTrainingsText(field("importTrainingTextLTextname").toString(), sents))
		KMessageBox::error(this, i18n("Konnte Trainingstext nicht speichern"));
	ui.pbProgress->setValue(1);
}

