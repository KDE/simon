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
#include <QRegExp>
#include <QProcess>
#include <QFile>
#include "../Logging/logger.h"
#include "../Settings/settings.h"


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
	proc->start("\""+Settings::getS("Programs/Files/BZip2")+"\n -d \""+filename+"\"");
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
		emit errorOccured(tr("Es ist ein Fehler beim Entpacken aufgetreten.\n\nBitte überprüfen Sie ob Sie das Paket \"bzip2\" installiert haben und der Pfad richtig gesetzt ist(%1).\n\n(Rückgabewert %2)").arg(Settings::getS("Programs/Files/BZip2")).arg(code));
		cancel();
		Logger::log(tr("[ERR] Programm \"bzip2 -d\" gab nicht %1 zurück").arg(code));
	} else {
		QString extFile = this->filename.remove(QRegExp(".bz2$"));
		emit extractionFinished(extFile);
	}
    
}
