//
// C++ Implementation: importworkingwizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "importtrainingtextworkingpage.h"
#include <QObject>
#include <QString>
#include <QWidget>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <KUrl>
#include "../../../SimonLib/Logging/logger.h"
#include "../../../SimonLib/QuickDownloader/quickdownloader.h"
#include "../../../SimonLib/Settings/settings.h"
#include "../xmltrainingtext.h"
#include "importtrainingtextlocalpage.h"
#include "xmltrainingtextlist.h"


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
void ImportTrainingTextWorkingPage::startImport(QString path)
{
	if (path.startsWith("http"))
	{
		Logger::log(i18n("[INF] Starte Remote Import von \"")+path+i18n("\""));
		QuickDownloader *qd = new QuickDownloader(this);
		connect(qd, SIGNAL(downloadFinished(QString)), this,
			SLOT(processText(QString)));
		qd->download(path);
	} else {
		Logger::log(i18n("[INF] Starte Lokalen Import von\"")+path+i18n("\""));
		parseFile(path);
	}
}


void ImportTrainingTextWorkingPage::initializePage()
{
	ui.pbProgress->setMaximum(0);
	if (field("importTrainingTextLocal").toBool())
	{
		startImport(field("importTrainingTextLFilename").value<KUrl>().path());
	} else startImport(field("textDownloadURL").toString());
}


/**
 * \brief Processes the text at the given path 
 * \author Peter Grasch
 * @param path the path to the new text
 */
void ImportTrainingTextWorkingPage::processText(QString path)
{
	QFileInfo fi = QFileInfo(path);
	QFile::copy(path, Settings::getS("PathToTexts")+"/"+fi.fileName());
	QFile::remove(path);

	
	QuickDownloader *down = qobject_cast<QuickDownloader*>(sender());
	if (down) down->deleteLater();
	
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


	QString xmlPath = Settings::getS("PathToTexts")+"/"+fi.fileName().left(fi.fileName().lastIndexOf("."))+".xml";
	XMLTrainingText *text = new XMLTrainingText(xmlPath);
	text->setTitle(field("importTrainingTextLTextname").toString());
	text->addPages(sents);
	text->save(xmlPath);
	delete text;
	ui.pbProgress->setValue(1);
// 	wizard()->next();
}
