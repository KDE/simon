/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_WAVPLAYERCLIENT_H_272785B973C443B89098D25E583308C1
#define SIMON_WAVPLAYERCLIENT_H_272785B973C443B89098D25E583308C1

#include <QObject>
#include <QList>


class WavPlayerSubClient;

/**
	\class WavPlayerClient
	
	\brief This class is used to get the needed data out of existing wav files to play them
	
	\author Peter Grasch
	\version 0.1
*/
class WavPlayerClient : public QObject {
	Q_OBJECT
	
signals:
	void currentProgress(int);
	void finished();

private:
	QList<WavPlayerSubClient*> clients;
	QList<WavPlayerSubClient*> clientsWaitingToFinish;

private slots:
	void slotCurrentProgress(int currentProgress);
	void slotFinished();

public:
	WavPlayerClient(QObject *parent=0);
	~WavPlayerClient();

	bool play(QString filename);
	void stop();
	


};

#endif

