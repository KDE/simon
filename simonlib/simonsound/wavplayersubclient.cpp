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


#include "wavplayersubclient.h"
#include "soundserver.h"
#include <simonwav/wav.h>
#include <KDebug>

/**
 * \brief Constructor
 * \author Peter Grasch
 */
WavPlayerSubClient::WavPlayerSubClient(SimonSound::DeviceConfiguration device, QObject* parent) : QIODevice(parent),
	SoundOutputClient(device),
	wav(0)
{
}

qint64 WavPlayerSubClient::readData(char *data, qint64 maxlen)
{
	qint64 read = wav->read(data, maxlen);
	emit currentProgress(SoundServer::getInstance()->byteSizeToLength(wav->pos(), m_deviceConfiguration));

	return read;

}

qint64 WavPlayerSubClient::writeData(const char *data, qint64 len)
{
	Q_UNUSED(data);
	Q_UNUSED(len);
	return -1;
}

bool WavPlayerSubClient::open (OpenMode mode)
{
	wav->beginReadSequence();
	return QIODevice::open(mode);
}

void WavPlayerSubClient::close()
{
	if (wav)
		wav->endReadSequence();
	QIODevice::close();
}

/**
 * \brief Plays back the given file
 * \author Peter Grasch
 */
bool WavPlayerSubClient::play( QString filename )
{
	if (wav)
	{
		wav->deleteLater();
		wav = 0;
	}

	wav = new WAV(filename);
	length = wav->getLength();
	if (length==0) {
		wav->deleteLater();
		wav = 0;
		return false;
	}
	open(QIODevice::ReadOnly);

	if (!SoundServer::getInstance()->registerOutputClient(this))
	{
		wav->deleteLater();
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
void WavPlayerSubClient::stop()
{
	SoundServer::getInstance()->deRegisterOutputClient(this);
}

void WavPlayerSubClient::finish()
{
	close();
	if (wav)
		wav->deleteLater();
	wav = 0;
	emit finished();
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
WavPlayerSubClient::~WavPlayerSubClient()
{
	if (wav) wav->deleteLater();
}




