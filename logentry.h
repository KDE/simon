//
// C++ Interface: logentry
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOGENTRY_H
#define LOGENTRY_H


#include <QDate>
#include <QList>


#define ERR 1
#define INF 2
#define UPD 4
#define SET 8
class LogEntry;
class QTime;
class QByteArray;
typedef QList <LogEntry> LogEntryList;


/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class LogEntry{
	
private:
	QTime time;
	QDate date;
	QByteArray message;
	//todo
	short type;

public:
    LogEntry(QDate date, QTime time, QByteArray message, short type);

    ~LogEntry();
//Set-Methoden
void setTime(QTime time) {this->time = time;}
void setDate(QDate date) {this->date = date;}
void setType(int type) {this->type = type;}
void setMessage(QByteArray message) {this->message = message;}

//Get-Methoden
QDate getDate() const {return this->date;}
QTime getTime() const {return this->time;}
short getType() const {return this->type;}
QByteArray getMessage(){return this->message;}

};

#endif
