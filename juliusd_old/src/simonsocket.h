/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SIMONSOCKET_H
#define SIMONSOCKET_H


#include <QSslSocket>

class JuliusControl;
class DatabaseAccess;

const qint8 protocolVersion=0;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class SimonSocket : public  QSslSocket
{
Q_OBJECT
signals:
	void login(float version, QString user, QString pass);
	void gotModel(QString userName, QString tiedList, QString hmmdefs);
	void errorOccured(QString, int);
	void info(QString, int);
	void wordSent(QString, int);
	
	//TextsYnc
	void sendId(int, QByteArray);
	//_____
	
private:
	DatabaseAccess *db;
	JuliusControl *julius; //!< Julius handle - controls the recogizer

private slots:
	void sendAnswer(qint32 request);
	void readMessage();
	void socketError();

	void juliusStopped(int exitCode);
	
	void sendError(QString);
	void sendInfo(QString);

public slots:
	void sendWord(QString word, QString sampa, QString samparaw);

public:
	SimonSocket(int socketDescriptor, DatabaseAccess *db);

    ~SimonSocket();

};

#endif
