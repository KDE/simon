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
#include "importworkingwizardpage.h"
#include "logger.h"


ImportWorkingWizardPage::ImportWorkingWizardPage(QWidget *parent) : QWizardPage(parent)
{ }

void ImportWorkingWizardPage::startImport(QString path)
{
	if (path.startsWith("http"))
	{
		Logger::log("Starting remote import from \""+path+"\"");
		QuickDownloader *qd = new QuickDownloader(this);
		connect(qd, SIGNAL(downloadFinished(QString)), this,
			SLOT(processText(QString)));
		qd->download(path);
	} else {
		Logger::log("Starting local import from \""+path+"\"");
		parseFile(path);
	}
}



void ImportWorkingWizardPage::processText(QString path)
{
	QFileInfo fi = QFileInfo(path);
	QFile::copy(path, "texts/"+fi.fileName());
	QFile::remove(path);
	
	wizard()->next();
}

void ImportWorkingWizardPage::parseFile(QString path)
{
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
			if (!sentence.isEmpty()) sents << sentence;
			tmp = QString(tmp+currentLine).mid(sentend).trimmed();
		}
	}
	file.close();
	
	QFileInfo fi = QFileInfo(path);
	XMLTrainingText *text = new XMLTrainingText("texts/"+fi.fileName().left(fi.fileName().lastIndexOf("."))+".xml");
	text->setTitle(((ImportLocalWizardPage*)wizard()->page(2))->getField("Textname").toString());
	text->addPages(sents);
	text->save();
	
	wizard()->next();
}
