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


#ifndef SIMON_LOGENTRY_H_E0FB1742B6F544438CEE7690A60FB94C
#define SIMON_LOGENTRY_H_E0FB1742B6F544438CEE7690A60FB94C


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
QByteArray getMessage() const {return this->message;}

};

#endif
