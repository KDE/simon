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

#include "wavrecorderclient.h"
#include <simonwav/wav.h>

#include "soundserver.h"
#include "loudnessmetersoundprocessor.h"

#include <QObject>

#include <KDebug>


/**
 * \brief Constructor
 */
WavRecorderClient::WavRecorderClient(const SimonSound::DeviceConfiguration& deviceConfiguration, QObject* parent) : 
	QObject(parent),
	SoundInputClient(deviceConfiguration),
	wavData(0),
	loudness(new LoudnessMeterSoundProcessor())
{
	//TODO
	registerSoundProcessor(loudness);
}



/**
 * \brief Records a WAV file to the given filename
 * \author Peter Grasch
 * \param QString filename
 * Filename to write to
 */
bool WavRecorderClient::record(QString filename)
{
	if (wavData)
	{
		delete wavData;
		wavData = NULL;
	}

	wavData = new WAV(filename, m_deviceConfiguration.channels(), 
			m_deviceConfiguration.sampleRate());
	wavData->beginAddSequence();

	bool succ =  SoundServer::getInstance()->registerInputClient(this);

	return succ;
}

void WavRecorderClient::processPrivate(const QByteArray& data, qint64 currentTime)
{
	wavData->write(data);

	float peak = loudness->peak() / 32768.0f;
	emit currentProgress(currentTime, peak);
	if (loudness->clipping())
		emit clippingOccured();
}

/**
 * \brief This will stop the current recording
 * 
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
bool WavRecorderClient::finish()
{
	bool succ = true;

	succ = SoundServer::getInstance()->deRegisterInputClient(this);

	wavData->endAddSequence();
	if (! wavData->writeFile()) succ = false;
	delete wavData;
	wavData = 0;

	return succ;
}


/**
 * \brief Destructor
 */
WavRecorderClient::~WavRecorderClient()
{
    if (wavData) delete wavData;
}




