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
#include <QProcess>
// #include "bzip2stream.hpp"

Bunzip::Bunzip(QObject *parent) : QObject(parent)
{
}

void Bunzip::extract(QString filename)
{
	Logger::log(tr("Extrahiere BZIP2 komprimierte Datei ")+filename);
	
	emit extracting(filename);
// 	QProcess *proc = new QProcess(this);
// 	connect(proc, SIGNAL(finished( int )), this, SLOT(extractingFinishing(int)));
}

void Bunzip::cancel()
{
	
}


void Bunzip::extractingFinishing(int code)
{
	if (!code)
		emit errorOccured(tr("Es ist ein Fehler beim Entpacken aufgetreten.\n\nBitte überprüfen Sie ob Sie das Paket \"bzip2\" installiert haben"));
	emit extractionFinished();
}


Bunzip::~Bunzip()
{
}


