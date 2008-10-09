#include "sync_filereader.h"
#include <QMessageBox>

#include <QDebug>
	
	//"yyyy.MM.dd-hh:mm:ss:zzz"

FileReader::FileReader(SyncManager *man):Reader(man)
{
	this->container = new MergeContainer();
}

void FileReader::createMergeContainer()
{
	QFile *datafile = new QFile();
	datafile->setFileName(this->datapath);
	QFile *stampfile = new QFile();
	stampfile->setFileName(this->stamppath);
	this->container->type = 1;
	
	if(!datafile->open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(NULL,"Fehler","Fehler beim lesen der Daten-Datei");
	}
	if(!stampfile->open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(NULL,"Fehler","Fehler beim lesen der Shadow-Datei");
	}

	QByteArray datareader, stampreader;
	QDateTime dt;

	int i = 0;


	while(!datafile->atEnd())
	{
		datareader = datafile->readLine();
		stampreader = stampfile->readLine();
		stampreader = stampreader.remove(23,2);
		QString temp(stampreader);
		
		dt = dt.fromString(stampreader,"yyyy.MM.dd-hh:mm:ss:zzz");
		qDebug() << dt;
		StampPair *stampp = new StampPair();
		stampp->first = QString(datareader);
		stampp->second = dt;
		
		this->container->vsp.append(stampp);
		++i;
	}

	FileConfiguration *conf = new FileConfiguration(1,datafile->fileName());
	emit Reader::readyContainer(this->container,conf);
}

void FileReader::setFiles(QString datapath, QString stamppath)
{
	this->datapath = datapath;
	this->stamppath = stamppath;
	this->createMergeContainer();
}
