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
	//test("/home/bedahr/simondstreamer.raw"),
	levelCrossCount(0),
	lastLevel(0),
	lastLevelCrossPos(-1),
	lastLevelCrossNeg(-1),
	lastTimeUnderLevel(0),
	lastTimeOverLevel(0),
	waitingForSampleToStart(true),
	waitingForSampleToFinish(false),
	sender(s),
	loudness(new LoudnessMeterSoundProcessor())
{
	registerSoundProcessor(loudness);
	//test.open(QIODevice::WriteOnly);
}



bool SimondStreamer::start()
{
	lastTimeOverLevel = -1;
	lastTimeUnderLevel = -1;
	bool succ =  SoundServer::getInstance()->registerInputClient(this);

	if (succ)
		emit started();

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
	int levelThreshold = 2000; 
	int headMargin = 200; 
	int tailMargin = 350;
	int headPreCacheMargin = 50;

	int peak = loudness->peak();
	if (peak > levelThreshold)
	{
		if (lastLevel > levelThreshold)
		{
			//kDebug() << "Still above level - now for : " << currentTime - lastTimeUnderLevel << "ms";

			currentSample += data; // cache data (waiting for sample) or send it (if already sending)
			kDebug() << "Adding data to sample...";

			//stayed above level
			if (waitingForSampleToStart)
			{
				if (currentTime - lastTimeUnderLevel > headMargin)
				{
					kDebug() << "Sending started...";
					waitingForSampleToStart = false;
					waitingForSampleToFinish = true;
				}
			} else {
				//test.write(currentSample);
				sender->sendSampleToRecognize(SoundServer::getInstance()->getChannels(),
						SoundServer::getInstance()->getSampleRate(),
						currentSample);
				currentSample.clear();
				kDebug() << "Clearing cached data...";
			}
		} else {
			//crossed upward
			kDebug() << "Crossed level upward...";
			currentSample += data; 
		}
		lastTimeOverLevel = currentTime;
	} else {
		waitingForSampleToStart = true;
		if (lastLevel < levelThreshold)
		{
			//stayed below level
			//kDebug() << "Still below level - now for : " << currentTime - lastTimeOverLevel << "ms";
			if (waitingForSampleToFinish)
			{
				//still append data during tail margin
				currentSample += data; 
				//test.write(currentSample);
				sender->sendSampleToRecognize(SoundServer::getInstance()->getChannels(),
					SoundServer::getInstance()->getSampleRate(),
					currentSample);
				currentSample.clear();
				if (currentTime - lastTimeOverLevel > tailMargin)
				{
					sender->recognizeSample();
				//	kDebug() << "Clearing cached data...";
					waitingForSampleToFinish = false;
					kDebug() << "Sample finalized and sent.";
				}
			} else {
				//get a bit of data before the first level cross

				currentSample += data;
				currentSample = currentSample.right(SoundServer::getInstance()->lengthToByteSize(headPreCacheMargin));
			}
		} else {
			//crossed downward
			kDebug() << "Crossed level downward...";
			currentSample += data; 
		}
		lastTimeUnderLevel = currentTime;
	}

	lastLevel = peak;
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
	if (succ)
		emit stopped();
	return succ;
}


/**
 * \brief Destructor
 */
SimondStreamer::~SimondStreamer()
{
}




