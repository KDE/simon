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


#include "bunzip.h"
#include <QRegExp>
#include <QProcess>
#include <QFile>
#include <KLocalizedString>
#include "../Logging/logger.h"
#include "coreconfiguration.h"


/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent Parent of the object
 */
Bunzip::Bunzip(QObject *parent) : Compression(parent)
{ 
	proc = new QProcess(this);
	connect(proc, SIGNAL(finished( int )), this, SLOT(extractingFinishing(int)));
	connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(readError()));
// 	connect(compression, SIGNAL(canceled()), prog, SLOT(cancel()));
}

Bunzip::~Bunzip()
{
    proc->deleteLater();
}

/**
 * \brief Extracts the given filename
 * \author Peter Grasch
 * 
 * This starts the extraction process by calling a new instance of bzip2; It also
 * sets up the signals/slots to emit the following signals:
 *	* extractingFinished(int) - the int is the return code of the process
 *	* readError() - a private slot which will read the error message and publish it
 *	 		through errorOccured(QString)
 *
 *	 The function will remove the filename to extract to if already there.
 *	 For example: In the current dir. there is a wiki.xml.bz2 and a wiki.xml
 *	 The function will try to remove the wiki.xml before extracting the wiki.xml.bz2
 *	 
 *  USE CAREFULLY!
 * 
 * @param filename The filename to extract
 */
void Bunzip::extract(QString filename)
{
	
	emit extracting(filename);
	
	this->filename = filename;
	if (QFile::exists(this->filename.remove(QRegExp(".bz2$")))) 
		QFile::remove(this->filename.remove(QRegExp(".bz2$")));;
	proc->start("\""+CoreConfiguration::bzip2().path()+"\n -d \""+filename+"\"");
}

/**
 * \brief Reads the error and will publish it through errorOccured(QString)
 * \author Peter Grasch
 */
void Bunzip::readError()
{
	QString err = proc->readAllStandardError();
	emit errorOccured(err);
}

/**
 * \brief Terminates the process and emits cnaceled()
 * \author Peter Grasch
 */
void Bunzip::cancel()
{
	proc->terminate();
	emit canceled();
}


/**
 * \brief Called when the process has finished
 * \author Peter Grasch
 * 
 * Will emit errorOccured(...) when the code is non-zero
 * Will emit extractionFinished(filename) if it is...
 * \param code The return code
 */
void Bunzip::extractingFinishing(int code)
{
	if (code!=0)
	{
		emit errorOccured(i18n("Es ist ein Fehler beim Entpacken aufgetreten.\n\nBitte überprüfen Sie ob Sie das Paket \"bzip2\" installiert haben und der Pfad richtig gesetzt ist(%1).\n\n(Rückgabewert %2)", CoreConfiguration::bzip2().path(), code));
		cancel();
		Logger::log(i18n("[ERR] Programm \"bzip2 -d\" gab nicht %1 zurück", code));
	} else {
		QString extFile = this->filename.remove(QRegExp(".bz2$"));
		emit extractionFinished(extFile);
	}
    
}
