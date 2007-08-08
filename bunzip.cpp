//
// C++ Implementation: bunzip
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bunzip.h"
#include "logger.h"



#include <QRegExp>
#include <QProcess>


Bunzip::Bunzip(QObject *parent) : QObject(parent)
{
}

void Bunzip::extract(QString filename)
{
	
	emit extracting(filename);
	proc = new QProcess(this);
	connect(proc, SIGNAL(finished( int )), this, SLOT(extractingFinishing(int)));
	connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(readError()));
	
	this->filename = filename;
	if (QFile::exists(this->filename.remove(QRegExp(".bz2$")))) 
		QFile::remove(this->filename.remove(QRegExp(".bz2$")));;
	proc->start("bzip2 -d "+filename);
}

void Bunzip::readError()
{
	QString err = proc->readAllStandardError();
	emit errorOccured(err);
}

void Bunzip::cancel()
{
	proc->terminate();
	emit canceled();
}


void Bunzip::extractingFinishing(int code)
{
	if (code!=0)
	{
		emit errorOccured(tr("Es ist ein Fehler beim Entpacken aufgetreten.\n\nBitte überprüfen Sie ob Sie das Paket \"bzip2\" installiert haben.\n\n(Rückgabewert %1)").arg(code));
		cancel();
		Logger::log(tr("[FEHLER]")+" "+tr("Programm \"bzip2 -d\" gab nicht %1 zurück").arg(code));
	} else {
		QString extFile = this->filename.remove(QRegExp(".bz2$"));
// 		Logger::log(this->filename+" "+tr("erfolgreich extrahiert zsu")+" "+extFile);
		emit extractionFinished(extFile);
	}
}


Bunzip::~Bunzip()
{
}


