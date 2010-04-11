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


#include "wavplayerclient.h"
#include "soundserver.h"
#include "wav.h"

#define WAV_PLAYER_CLIENT_BUFFER_SIZE 16384


/**
 * \brief Constructor
 * \author Peter Grasch
 */
WavPlayerClient::WavPlayerClient(QObject* parent) : QObject(parent),
	SoundOutputClient(),
	wav(0)
{
}



QByteArray WavPlayerClient::getChunk(qint64 streamTime)
{
//	if (wav->atEnd())
//		return QByteArray();
	QByteArray data = wav->read(WAV_PLAYER_CLIENT_BUFFER_SIZE);

	emit currentProgress(streamTime);
	return data;
}

QIODevice* WavPlayerClient::getDataProvider()
{
	return wav;
}

/**
 * \brief Plays back the given file
 * \author Peter Grasch
 */
bool WavPlayerClient::play( QString filename )
{
	if (wav)
	{
		delete wav;
		wav = 0;
	}

	wav = new WAV(filename);
	length = wav->getLength();
	if (length==0) {
		delete wav;
		wav = 0;
		return false;
	}
	wav->beginReadSequence();

	if (!SoundServer::getInstance()->registerOutputClient(this))
	{
		delete wav;
		wav = 0;
		return false;
	}
	return true;
}


/**
 * \brief Stops the current playback
 * 
 * \author Peter Grasch
 */
void WavPlayerClient::stop()
{
	SoundServer::getInstance()->deRegisterOutputClient(this);
	delete wav;
	wav = 0;
	emit finished();
//	stopTimer = true; // to work around the issue that you can't stop the timer from a different thread
	//which would be the case if we would stop it here (this is called from the callback thread)
    //this also triggers the closing of the stream as we can't stop it here because it would still be open
    //from the callback function
}



/**
 * \brief Destructor
 * \author Peter Grasch
 */
WavPlayerClient::~WavPlayerClient()
{
	if (wav) delete wav;
}



