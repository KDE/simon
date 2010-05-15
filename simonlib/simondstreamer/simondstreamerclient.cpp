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

#include "simondstreamerclient.h"

#include <simonsound/soundserver.h>
#include <simonsound/loudnessmetersoundprocessor.h>
#include <simonwav/wav.h>

#include <QObject>

#include <KDebug>

#ifdef Q_OS_WIN32
SoundServer* SoundServer::instance=NULL;
#endif

/**
 * \brief Constructor
 */
SimondStreamerClient::SimondStreamerClient(qint8 id, SimonSender *s, SimonSound::DeviceConfiguration device, QObject *parent) :
	QObject(parent),
	SoundInputClient(device, SoundClient::Background),
	lastLevel(0),
	lastTimeUnderLevel(0),
	lastTimeOverLevel(0),
	waitingForSampleToStart(true),
	waitingForSampleToFinish(false),
	currentlyRecordingSample(false),
	m_isRunning(false),
	sender(s),
	loudness(new LoudnessMeterSoundProcessor())
{
	this->id = id;
	registerSoundProcessor(loudness);
}


void SimondStreamerClient::inputStateChanged(QAudio::State state)
{
	if (state == QAudio::StoppedState)
		stop();
}

bool SimondStreamerClient::isRunning()
{
	return m_isRunning;
}

bool SimondStreamerClient::start()
{
	lastTimeOverLevel = -1;
	lastTimeUnderLevel = -1;
	bool succ =  SoundServer::getInstance()->registerInputClient(this);

	kDebug() << "Registered input client: " << succ;
	if (succ)
	{
		m_isRunning = true;
		emit started();
	} else {
		m_isRunning = false;
		emit stopped();
	}

	return succ;
}

void SimondStreamerClient::processPrivate(const QByteArray& data, qint64 currentTime)
{
	int levelThreshold = SoundServer::getLevelThreshold(); 
	int headMargin = SoundServer::getHeadMargin(); 
	int tailMargin = SoundServer::getTailMargin();
	int shortSampleCutoff = SoundServer::getShortSampleCutoff();

	int peak = loudness->peak();
	if (peak > levelThreshold)
	{
		if (lastLevel > levelThreshold)
		{
#ifdef SIMOND_DEBUG
			kDebug() << "Still above level - now for : " << currentTime - lastTimeUnderLevel << "ms";
#endif

			currentSample += data; // cache data (waiting for sample) or send it (if already sending)
#ifdef SIMOND_DEBUG
			kDebug() << "Adding data to sample...";
#endif

			//stayed above level
			if (waitingForSampleToStart)
			{
				if (currentTime - lastTimeUnderLevel > shortSampleCutoff)
				{
#ifdef SIMOND_DEBUG
					kDebug() << "Sending started...";
#endif
					waitingForSampleToStart = false;
					waitingForSampleToFinish = true;
					if (!currentlyRecordingSample)
					{
						sender->startSampleToRecognize(id, m_deviceConfiguration.channels(),
							m_deviceConfiguration.sampleRate());
						currentlyRecordingSample = true;
					}
				}
			} else {
				sender->sendSampleToRecognize(id, currentSample);
				currentSample.clear();
#ifdef SIMOND_DEBUG
				kDebug() << "Clearing cached data...";
#endif
			}
		} else {
			//crossed upward
#ifdef SIMOND_DEBUG
			kDebug() << "Crossed level upward...";
#endif
			currentSample += data; 
		}
		lastTimeOverLevel = currentTime;
	} else {
		waitingForSampleToStart = true;
		if (lastLevel < levelThreshold)
		{
			//stayed below level
#ifdef SIMOND_DEBUG
			kDebug() << "Still below level - now for : " << currentTime - lastTimeOverLevel << "ms";
#endif
			if (waitingForSampleToFinish)
			{
				//still append data during tail margin
				currentSample += data; 
				sender->sendSampleToRecognize(id, currentSample);
				currentSample.clear();
				if (currentTime - lastTimeOverLevel > tailMargin)
				{
					sender->recognizeSample(id);
					currentlyRecordingSample = false;
					waitingForSampleToFinish = false;
					kDebug() << "Sample finalized and sent.";
				}
			} else {
				//get a bit of data before the first level cross
				currentSample += data;
				currentSample = currentSample.right(SoundServer::getInstance()->lengthToByteSize(headMargin, m_deviceConfiguration));
			}
		} else {
			//crossed downward
#ifdef SIMOND_DEBUG
			kDebug() << "Crossed level downward...";
#endif
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
bool SimondStreamerClient::stop()
{
	bool succ = true;
	succ = SoundServer::getInstance()->deRegisterInputClient(this);
	if (succ)
	{
		m_isRunning = false;
		emit stopped();
	}
	return succ;
}


/**
 * \brief Destructor
 */
SimondStreamerClient::~SimondStreamerClient()
{
}





