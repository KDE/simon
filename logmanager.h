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
#include <QThread>


/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class LogManager : public QThread
{
	Q_OBJECT
	
private:
	LogEntryList *entries;
	bool killMe;
	bool finishedLoading;
	
private slots:
	void resetKillFlag()  { killMe = false; }

public:
    LogManager();

    ~LogManager();

	LogEntryList* getDay(QDate day);
	LogEntryList* getAll();
	bool readLog();
	bool hasFinishedReading(){return this->finishedLoading;}

	void run ();

public slots:
	void stop(bool free);

signals:
	void logReadFinished(int value);


};

#endif
