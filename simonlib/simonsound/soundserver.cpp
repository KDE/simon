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

#include "soundconfig.h"
#include "soundserver.h"
#include "soundclient.h"

#include <QObject>
#include <QAudioInput>

#include <KDebug>

//FIXME: Deprecated
#ifdef USE_WITH_SIMON
#include <adinstreamer/adinstreamer.h>
#endif

SoundServer* SoundServer::instance=NULL;

/**
 * \brief Constructor
 */
SoundServer::SoundServer(QObject* parent) : QObject(parent),
	channels(SoundConfiguration::soundChannels()),
	sampleRate(SoundConfiguration::soundSampleRate()),
	input(NULL)
{
	connect(&inputData, SIGNAL(bytesWritten(qint64)), this, SLOT(inputDataAvailable(qint64)));
}

bool SoundServer::registerInputClient(SoundClient* client)
{
	if (client->isExclusive())
	{
		//suspend all other inputs
		QHashIterator<SoundClient*, qint64> i(activeInputClients);
		while (i.hasNext())
		{
			i.next();
			i.key()->suspend();
			suspendedInputClients.insert(i.key(), i.value());
			activeInputClients.remove(i.key());
		}
	}


	activeInputClients.insert(client, 0);
	bool succ = true;

	if (!input) //recording not currently running
	{
		//then start recording
		succ = startRecording();
	}


	return succ;
}

bool SoundServer::deRegisterInputClient(SoundClient* client)
{
	activeInputClients.remove(client);
	kDebug() << "Stopping client";
	if (client->isExclusive())
	{
		kDebug() << "This was an exclusive client";
		QHashIterator<SoundClient*, qint64> i(suspendedInputClients);

		/// if we have one exclusive input in the suspended list move it to the active
		/// list and ignore the rest
		///
		/// @note The order in which the input clients are resumed is undefined
		///
		///otherwise move everything back
		
		kDebug() << "Looking for other exclusive inputs";
		bool hasExclusive = false;
		while (i.hasNext())
		{
			i.next();
			if (i.key()->isExclusive())
			{
				kDebug() << "Other exclusive input found - adding this to the active list";
				activeInputClients.insert(i.key(), i.value());
				suspendedInputClients.remove(i.key());
				hasExclusive = true;
				break;
			} else kDebug() << "Not exclusive: " << i.value();
		}
		if (!hasExclusive)
		{
			i.toFront();
			while (i.hasNext())
			{
				i.next();
				i.key()->resume();
				activeInputClients.insert(i.key(), i.value());
				suspendedInputClients.remove(i.key());
			}
		}
	}

	if (activeInputClients.isEmpty()) //don't need to record any longer
	{
		//then stop recording
		kDebug() << "No active clients available... Stopping recording";
		return stopRecording();
	}

	return true;
}

void SoundServer::inputDataAvailable(qint64 size)
{
	Q_UNUSED(size);

	bool succ = true;
	QByteArray data = inputData.getChunk(succ);

	if (!succ) return;

	//length is in ms
	qint64 length = data.count() / (channels * 2 /* 16 bit */ * ((float)sampleRate / 1000.0f));

	//pass data on to all registered, active clients
	QList<SoundClient*> active = activeInputClients.keys();
	foreach (SoundClient *c, active)
	{
		qint64 streamTime = activeInputClients.value(c)+length;
		c->process(data, streamTime);
		//update time stamp
		activeInputClients.insert(c, streamTime);
	}
}



bool SoundServer::startRecording()
{
	kDebug() << "Starting recording...";
	QAudioFormat format;
	format.setFrequency(sampleRate);
	format.setChannels(channels);
	format.setSampleSize(16); // 16 bit
	format.setSampleType(QAudioFormat::SignedInt); // SignedInt currently
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setCodec("audio/pcm");

	QString inputDevice = SoundConfiguration::soundInputDevice();
	QAudioDeviceInfo selectedInfo = QAudioDeviceInfo::defaultInputDevice();
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
		if (deviceInfo.deviceName() == SoundConfiguration::soundInputDevice())
			selectedInfo = deviceInfo;

	if (!selectedInfo.isFormatSupported(format))
	{
		kDebug() << "Format not supported";

#ifdef USE_WITH_SIMON
		//FIXME: Remove me (just for backwards compatibility with old adinstreamer)
		if (AdinStreamer::hasInstance())
			AdinStreamer::getInstance()->restartSoundStream();
#endif
	
		return false;
	}

	input = new QAudioInput(selectedInfo, format, this);
	inputData.open(QIODevice::ReadWrite);
	input->start(&inputData);
	kDebug() << "Started audio input";
	return true;
}


bool SoundServer::stopRecording()
{
	Q_ASSERT(input);

	input->stop();
	delete input;
	input = NULL;
	inputData.close();

#ifdef USE_WITH_SIMON
	//FIXME: Remove me (just for backwards compatibility with old adinstreamer)
	if (AdinStreamer::hasInstance())
		AdinStreamer::getInstance()->restartSoundStream();
#endif

	return true;
}



/**
 * \brief Destructor
 */
SoundServer::~SoundServer()
{
}



