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


/**
 * \brief Constructor
 * creating a new Instanz of LogManager
 *
 * \author Phillip Goriup
 */
LogManager::LogManager()
{

}


/**
 * \brief reads the logfile, and saves the content into a vector of LogEntry
 * get the information of the loggfile by reading line by line. chops every line into
 * the tags;
 * researches every line if a extra information is given 
 * 	[ERR] for Error
 * 	[INF] for Info
 * 	[UPD] for Update
 * saves the read informations into a Vector of LogEntry
 * 
 * \author Phillip Goriup, Peter Grasch
 */
bool LogManager::readLog()
{
	this->entries.clear();
	QFile *LogF = new QFile("log/simon.log");
	if (!LogF->open(QIODevice::ReadOnly))
	{
		return false;
	}
	QString str, strdate, strtime;
	QTime time;
	QDate date;
	int type, datestart;
	
	while (!LogF->atEnd ())
	{
		type = 0;
		
		str = LogF->readLine();
		datestart = str.indexOf(QRegExp("\\d{4,4}\\/\\d{2,2}\\/\\d{2,2}"));
		strdate = str.mid(datestart,10);
		strtime = str.mid(str.indexOf(QRegExp("\\d{2,2}:\\d{2,2}:\\d{2,2}"), datestart) ,8);
		
		date = QDate::fromString (strdate,"yyyy/MM/dd") ;
		time = QTime::fromString(strtime,"hh:mm:ss");
		
		if(str.contains("[ERR]", Qt::CaseInsensitive))
			type = type|ERR;
		if(str.contains("[INF]", Qt::CaseInsensitive))
			type = type|INF;
		if(str.contains("[UPD]", Qt::CaseInsensitive))
			type = type|UPD;
		if(str.contains("[SET]", Qt::CaseInsensitive))
			type = type|SET;
		
		str.remove(QRegExp("\\[.*\\]"));
		
		this->entries.append(new LogEntry(date, time, str.trimmed(), type));
	}		
	return true;
}


/**
 * \brief returns entries per one day
 *
 * \param 
 * 
 * \author Phillip Goriup
 */
LogEntryList LogManager::getDay(QDate day)
{
	LogEntryList entriesperday;
	int i = 0;
	int size = entries.size();
	while((i<size) && (this->entries[i++]->getDate() < day)) ;
		
	while((i<size) && (this->entries[i]->getDate() == day))
	{
		entriesperday.append(entries[i++]);
	}
	return entriesperday;
}


/**
 * \brief returns all entries
 *
 * \author Phillip Goriup
 */
LogEntryList LogManager::getAll()
{
	return this->entries;
}


/**
 * \brief Destructor
 * 
 * \author Phillip Goriup
 */
LogManager::~LogManager()
{
}



