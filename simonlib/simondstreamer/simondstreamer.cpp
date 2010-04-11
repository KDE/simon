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

#include "simondstreamer.h"

#include <simonsound/soundserver.h>
#include <simonsound/wav.h>
#include <simonsound/loudnessmetersoundprocessor.h>

#include <QObject>

#include <KDebug>


/**
 * \brief Constructor
 */
SimondStreamer::SimondStreamer(SimonSender *s, QObject *parent) :
	QObject(parent),
	SoundInputClient(SoundClient::None),
	levelCrossCount(0),
	sender(s),
	loudness(new LoudnessMeterSoundProcessor())
{
	registerSoundProcessor(loudness);
}



bool SimondStreamer::start()
{
	bool succ =  SoundServer::getInstance()->registerInputClient(this);

	return succ;
}

#if 0
void SimondStreamer::processPrivate(const QByteArray& data, qint64 currentTime)
{
	//kDebug() << "Got sound... Peak: " << loudness->peak() << " Clip: " << loudness->clipping();

	int peak = loudness->peak();
	bool finalSample = false;

	if (peak >= 2000 /* level */)
	{
		levelCrossCount += 2;
	}
	if (levelCrossCount > 0)
	{
		currentSample += data;
		levelCrossCount--;
		if (levelCrossCount == 0)
			finalSample = true;
	} else {
		currentSample.clear();
	}

	if (levelCrossCount >=  5)
	{
		sender->sendSampleToRecognize(SoundServer::getInstance()->getChannels(),
				SoundServer::getInstance()->getSampleRate(),
				data);
		currentSample.clear();
	}
	if (finalSample)
		sender->recognizeSample();
}
#endif

void SimondStreamer::processPrivate(const QByteArray& data, qint64 currentTime)
{
	//kDebug() << "Got sound... Peak: " << loudness->peak() << " Clip: " << loudness->clipping();

	int peak = loudness->peak();
	bool finalSample = false;

	if (peak >= 2000 /* level */)
	{
		levelCrossCount += 2;
	}
	if (levelCrossCount > 0)
	{
		currentSample += data;
		levelCrossCount--;
		if (levelCrossCount == 0)
			finalSample = true;
	} else {
		currentSample.clear();
	}

	if (levelCrossCount >=  5)
	{
		sender->sendSampleToRecognize(SoundServer::getInstance()->getChannels(),
				SoundServer::getInstance()->getSampleRate(),
				data);
		currentSample.clear();
	}
	if (finalSample)
		sender->recognizeSample();
}

/**
 * \brief This will stop the current recording
 * 
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
bool SimondStreamer::stop()
{
	bool succ = true;
	succ = SoundServer::getInstance()->deRegisterInputClient(this);
	return succ;
}


/**
 * \brief Destructor
 */
SimondStreamer::~SimondStreamer()
{
}




