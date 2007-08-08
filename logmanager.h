//
// C++ Interface: logmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "logentry.h"


/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class LogManager{

private:
	QVector<LogEntry*> entries;
	
public:
    LogManager();

    ~LogManager();

	LogEntryList getDay(QDate day);
	LogEntryList getAll();
	bool readLog();

};

#endif
