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


#ifndef SIMON_SOUNDINPUTCLIENT_H_4AC60251BE6A419BA1236280815A2AAD
#define SIMON_SOUNDINPUTCLIENT_H_4AC60251BE6A419BA1236280815A2AAD


#include <QIODevice>
#include "soundclient.h"
#include "simonsound_export.h"

class SIMONSOUND_EXPORT SoundOutputClient : public SoundClient {
public:
	SoundOutputClient();
	virtual ~SoundOutputClient();

//	virtual QByteArray getChunk(qint64 streamTime /* this is just for your info; */
//			/*your input does NOT need to seek to this position */)=0;

	virtual QIODevice* getDataProvider()=0;
	virtual void finish()=0;
	//virtual qint64 readData(char *data, qint64 maxlen)=0;
};


/*
class SoundOutputClientWrapper : public QIODevice
{
Q_OBJECT
public:
	SoundOutputClientWrapper(QObject *parent, SoundOutputClient *client);

protected:
	qint64 readData(char *data, qint64 maxlen);
	qint64 writeData(const char *data, qint64 len);

private:
	SoundOutputClient *m_client;
	qint64 m_currentStreamTime;
};
*/



#endif

