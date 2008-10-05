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
