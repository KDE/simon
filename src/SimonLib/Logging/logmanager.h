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
#include <QVector>

class QDate;


typedef QVector<QDate> Dates;

/**
 * \class LogManager
 * \brief Manages the logfile
 * \author Phillip Goriup
 * \date 6.8.2007
 * \version 0.1
 */
class LogManager : public QThread
{
	Q_OBJECT
	
private:
	LogEntryList *entries;
	bool killMe;
	qint64 logFilesize;
	bool finishedLoading;
	QDate dayToGet;
	
private slots:
	void resetKillFlag()  { killMe = false; }

public:
    LogManager();

    ~LogManager();

	bool isBusy() { return isRunning(); }


	bool hasFinishedReading();
	void run ();

public slots:
	void getDateList();
	void stop();
	void getDay(QDate day=QDate());
	void getAll();

signals:
	void done();
	void logReadFinished(int value);
	void foundEntries(LogEntryList* entries,bool copy);
	void daysAvailable(Dates days);


};

#endif
