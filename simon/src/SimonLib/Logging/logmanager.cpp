//
// C++ Implementation: logmanager
//
// Description: 
//
//
// Author: Phillip Goriup <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "logmanager.h"
#include <QRegExp>
#include <QFile>
#include <QCoreApplication>
#include <QDate>
#include "logger.h"


/**
 * \brief Constructor
 *
 * \author Peter Grasch
 */
LogManager::LogManager()
{
	this->finishedLoading = false;
	entries = new LogEntryList();
	killMe=false;
	connect(this, SIGNAL(finished()), this, SLOT(resetKillFlag()));
	connect(this, SIGNAL(terminated()), this, SLOT(resetKillFlag()));
}


/**
 * \brief Returns true if there would no point in (re-)reading the logfile
 * \author Peter Grasch
 * 
 * If this is false either the logfile changed or we never actually completely read it
 * 
 * @return have we finished reading the logfile?
 */
bool LogManager::hasFinishedReading()
{
	QFile logF("log/simon.log");
	return (this->finishedLoading && (logF.size() == logFilesize));
}


/**
 * \brief reads the logfile, and saves the content into a vector of LogEntry
 * get the information of the loggfile by reading line by line. chops every line into
 * the tags;
 * researches every line if a extra information is given 
 * 	[ERR] for Error
 * 	[INF] for Info
 * 	[UPD] for Update
 * saves the read information into a Vector of LogEntry
 * 
 * \author Phillip Goriup, Peter Grasch
 */
void LogManager::run ()
{
	finishedLoading = false;
	if(!this->entries)
	{
		this->entries = new LogEntryList;
	}
	else
	{
		delete(this->entries);
		this->entries = new LogEntryList;
	}
	QFile *LogF = new QFile("log/simon.log");
	if (!LogF->open(QIODevice::ReadOnly))
	{
		emit logReadFinished(1);
	}
	logFilesize = LogF->size();
	QString str;
	int type;
	
	int i = 0;
	
	QCoreApplication::processEvents();
	while (!LogF->atEnd () && !killMe)
	{
		type = 0;
		
		str = LogF->readLine();
		if(str.contains("[ERR]", Qt::CaseInsensitive))
			type = type|ERR;
		if(str.contains("[INF]", Qt::CaseInsensitive))
			type = type|INF;
		if(str.contains("[UPD]", Qt::CaseInsensitive))
			type = type|UPD;
		if(str.contains("[SET]", Qt::CaseInsensitive))
			type = type|SET;

		//MMMMMMMMMMMMUUUUUUUUUUUUUUUUUUHHHHHHHAAAAAAHHHHHHAAAAAAAA
		QTime funzi_der_erste = QTime::fromString(str.mid(12 ,8),"hh:mm:ss");
		QDate funzus_der_grosse = QDate::fromString(str.mid(1,10),"yyyy/MM/dd");
		//_______________________________________________________________________
		
		this->entries->append(LogEntry(funzus_der_grosse , 
		funzi_der_erste, 
		str.remove(QRegExp("\\[.*\\]")).trimmed().toUtf8(), type));
		
		i++;
	}	
	delete(LogF);
	if (!killMe)
	{
		finishedLoading = true;
		emit this->logReadFinished(0);
	}
	else
	{
		emit this->logReadFinished(2);
	}
}




/**
 * \brief Retrieves the entries of the given day and emits them using foundEntries()
 * 
 * The method will try to determine if the thread is currently running (and thus building the list)
 * by calling isBusy() and will queue the process by connecting itself to the finished() signal of
 * the process
 *
 * \param day The day we want to view
 * 
 * \author Phillip Goriup, Peter Grasch
 */
void LogManager::getDay(QDate day)
{
	
	if (this->isBusy()) 
	{
		dayToGet = day;
		disconnect(this, SIGNAL(finished()));
		connect(this, SIGNAL(finished()), this, SLOT(getDay()));
		return;
	}

	if (day.isNull())
	{
		if (dayToGet.isNull()) return;
		else day = dayToGet;
	}

	LogEntryList *entriesperday = new LogEntryList;

	if (!this->entries || this->entries->count() == 0)
	{
		emit foundEntries(entriesperday,true);
		return; //if we haven't read the logfile
			//there is no point in filtering it afterwards
	}

	int i = 0;
	int size = entries->count();
	while((i<size) && (this->entries->at(i++).getDate() < day))
		;

	i--;
	
	size = entries->count();
	while((i<size) && (this->entries->at(i).getDate() == day))
	{
		entriesperday->append(this->entries->at(i));
		i++;
		
	}
	
	emit foundEntries(entriesperday,true);

	dayToGet = QDate();
}


/**
 * \brief Frees the memory (if the entries are not clean (i.e. hasFinishedReading() returns false)
 *
 * \author Phillip Goriup, Peter Grasch
 */
void LogManager::stop()
{
	disconnect(this, SIGNAL(finished()), 0,0);

	killMe=true;

	if (isRunning())
		wait(5000);
	if (isRunning())
		terminate();	//make ABSOLUTELY sure
	if (isRunning())	//that the thread WILL stop
		wait(500);

	if(!hasFinishedReading())
		this->entries->clear();

	killMe=false;
}



/**
 * \brief Will attempt to create a list of QDates ("Dates") and emit it using daysAvailable(Dates)
 * 
 * The method will try to determine if the thread is currently running (and thus building the list)
 * by calling isBusy() and will queue the process by connecting itself to the finished() signal of
 * the process
 * 
 * \author Peter Grasch
 */
void LogManager::getDateList()
{
	if (this->isBusy()) 
	{
		disconnect(this, SIGNAL(finished()));
		connect(this, SIGNAL(finished()), this, SLOT(getDateList()));
		return;
	}

	if (!entries) {
		emit daysAvailable(Dates());
		return;
	}
	Dates daysAvail;
	QDate currentDate;
	for (int i=0; i < this->entries->count(); i++)
	{
		if (entries->at(i).getDate()!=currentDate)
		{
			currentDate = entries->at(i).getDate();
			daysAvail << currentDate;
		}
	}
	emit daysAvailable(daysAvail);
}


/**
 * \brief Builds a list of all entries and emits them using foundEntries()
 * 
 * The method will try to determine if the thread is currently running (and thus building the list)
 * by calling isBusy() and will queue the process by connecting itself to the finished() signal of
 * the process
 *
 * \author Phillip Goriup
 */
void LogManager::getAll()
{
	if (this->isBusy()) 
	{
		disconnect(this, SIGNAL(finished()));
		connect(this, SIGNAL(finished()), this, SLOT(getAll()));
		return;
	}
	emit foundEntries(this->entries,false);
}


/**
 * \brief Destructor
 * 
 * \author Phillip Goriup
 */
LogManager::~LogManager()
{
	this->entries->clear();
    delete entries;
}



