//
// C++ Implementation: logentry
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "logentry.h"



LogEntry::LogEntry(QDate date, QTime time, QString message, int type)
{
	this->time = time;
	this->date = date;
	this->message = message;
	this->type = type;
}


LogEntry::~LogEntry()
{
}


