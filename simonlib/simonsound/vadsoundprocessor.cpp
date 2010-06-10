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

#include "vadsoundprocessor.h"
#include "soundserver.h"
#include <QByteArray>
#include <QtGlobal>
#include <KDebug>


/**
 * \brief Constructor
 */
VADSoundProcessor::VADSoundProcessor(SimonSound::DeviceConfiguration deviceConfiguration, bool passAll) : 
	QObject(),
	LoudnessMeterSoundProcessor(),
	m_deviceConfiguration(deviceConfiguration),
	m_passAll(passAll),
	lastLevel(0),
	lastTimeUnderLevel(0),
	lastTimeOverLevel(0),
	waitingForSampleToStart(true),
	waitingForSampleToFinish(false),
	currentlyRecordingSample(false)
{
}

void VADSoundProcessor::process(QByteArray& data, qint64& currentTime)
{
	LoudnessMeterSoundProcessor::process(data, currentTime);

	int levelThreshold = SoundServer::getLevelThreshold(); 
	int headMargin = SoundServer::getHeadMargin(); 
	int tailMargin = SoundServer::getTailMargin();
	int shortSampleCutoff = SoundServer::getShortSampleCutoff();

	bool passDataThrough = false;

	m_startListening = false;
	m_doneListening = false;

	if (peak() > levelThreshold)
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
						sampleStartTime = currentTime;
						m_startListening = true;
						emit listening();
						passDataThrough = true;
						currentlyRecordingSample = true;
					}
				}
			} else {
				passDataThrough = true;
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
				passDataThrough = true;
				//sender->sendSampleToRecognize(id, currentSample);
				//currentSample.clear();
				if (currentTime - lastTimeOverLevel > tailMargin)
				{
					m_doneListening = true;
					//sender->recognizeSample(id);
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

	lastLevel = peak();

	if (passDataThrough || m_passAll)
	{
		if (currentSample.count() > data.count())
		{
			//contained cached data as such must be the first sending
			kDebug() << "STARTED!";
			currentTime = sampleStartTime;
		}
		data = currentSample;
		currentSample.clear();
	} else {
		data.clear();
	}

	if (m_doneListening)
		emit complete();
}

void VADSoundProcessor::reset()
{
	lastTimeOverLevel = -1;
	lastTimeUnderLevel = -1;
	waitingForSampleToStart = true;
	waitingForSampleToFinish = false;
	currentlyRecordingSample = false;
}


