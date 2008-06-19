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
	setTitle(tr("Text wird hinzugefügt"));
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
		Logger::log(QObject::tr("[INF] Starte Remote Import von \"")+path+QObject::tr("\""));
		QuickDownloader *qd = new QuickDownloader(this);
		connect(qd, SIGNAL(downloadFinished(QString)), this,
			SLOT(processText(QString)));
		qd->download(path);
	} else {
		Logger::log(QObject::tr("[INF] Starte Lokalen Import von\"")+path+QObject::tr("\""));
		parseFile(path);
	}
}


void ImportTrainingTextWorkingPage::initializePage()
{
	ui.pbProgress->setMaximum(0);
	if (field("importTrainingTextLocal").toBool())
	{
		startImport(field("importTrainingTextLFilename").toString());
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
	QRegExp reg("(\\.|\\!|\\?)");
	QRegExp spec("(\\.\\.\\.|\\!\\!\\!|\\?\\?\\?)");
	QString currentLine;
	while ((!ts.atEnd()) || (!tmp.isEmpty()))
	{
		if (!ts.atEnd())
			currentLine = ts.readLine();
		else currentLine = "";
		
		if ((QString(tmp+currentLine).indexOf(spec) <= 
				   QString(tmp+currentLine).indexOf(reg)) && 
				   (QString(tmp+currentLine).indexOf(spec) != -1))
			sentend = QString(tmp+currentLine).indexOf(spec)+3;
		else if (QString(tmp+currentLine).indexOf(reg) != -1)
			sentend = QString(tmp+currentLine).indexOf(reg)+1;
		else sentend = QString(tmp+currentLine).length();
		
		if (sentend == -1)
			tmp += currentLine;
		else  {
			QString sentence = QString(tmp+" "+currentLine).left(sentend).trimmed();
			sentence.remove("\"");
			sentence.remove(",");
			sentence.remove(".");
			sentence.replace("-", " ");
			sentence.remove("#");
			sentence.remove("'");
			sentence.remove("`");
			sentence.remove("!");
			sentence.remove("?");
			sentence.remove(".");
			if (!sentence.isEmpty()) sents << sentence;
			tmp = QString(tmp+currentLine).mid(sentend).trimmed();
		}
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
