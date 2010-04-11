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
#include "soundinputclient.h"
#include "soundoutputclient.h"

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>

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
	input(NULL),
	output(NULL),
	currentOutputTimeStamp(0),
	currentOutputClient(NULL)
{
	connect(&inputData, SIGNAL(bytesWritten(qint64)), this, SLOT(inputDataAvailable(qint64)));
}

bool SoundServer::registerInputClient(SoundInputClient* client)
{
	if (client->isExclusive())
	{
		//suspend all other inputs
		QHashIterator<SoundInputClient*, qint64> i(activeInputClients);
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

bool SoundServer::deRegisterInputClient(SoundInputClient* client)
{
	if (activeInputClients.remove(client) == 0)
	{
		//wasn't active anyways
		suspendedInputClients.remove(client);
		return true;
	}
	if (client->isExclusive())
	{
		QHashIterator<SoundInputClient*, qint64> i(suspendedInputClients);

		/// if we have one exclusive input in the suspended list move it to the active
		/// list and ignore the rest
		///
		/// @note The order in which the input clients are resumed is undefined
		///
		///otherwise move everything back
		
		bool hasExclusive = false;
		while (i.hasNext())
		{
			i.next();
			if (i.key()->isExclusive())
			{
				activeInputClients.insert(i.key(), i.value());
				suspendedInputClients.remove(i.key());
				hasExclusive = true;
				break;
			}
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
	QList<SoundInputClient*> active = activeInputClients.keys();
	foreach (SoundInputClient *c, active)
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
		return false;
	}

#ifdef USE_WITH_SIMON
	//FIXME: Remove me (just for backwards compatibility with old adinstreamer)
	if (AdinStreamer::hasInstance())
		AdinStreamer::getInstance()->stopSoundStream();
#endif
	
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


bool SoundServer::registerOutputClient(SoundOutputClient* client)
{
	if (currentOutputClient != NULL)
		suspendedOutputClients.insert(currentOutputClient, currentOutputTimeStamp);

	currentOutputTimeStamp = 0;
	currentOutputClient = client;

	bool succ = true;

	if (!output) //playback not currently running
	{
		//then start playback
		succ = startPlayback();
	}


	return succ;
}

bool SoundServer::deRegisterOutputClient(SoundOutputClient* client)
{
	if (client != currentOutputClient)
	{
		//wasn't active anyways
		suspendedOutputClients.remove(client);
		return true;
	}


	if (suspendedOutputClients.isEmpty())
	{
		currentOutputTimeStamp = 0;
		currentOutputClient = NULL;
		kDebug() << "No active clients available... Stopping playback";
		return stopPlayback();
	}

	QHashIterator<SoundOutputClient*, qint64> i(suspendedOutputClients);
	i.next();
	currentOutputClient = i.key();
	currentOutputTimeStamp = i.value();
	suspendedOutputClients.remove(i.key());
	return true;
}

bool SoundServer::startPlayback()
{
	kDebug() << "Starting playback...";
	QAudioFormat format;
	format.setFrequency(sampleRate);
	format.setChannels(channels);
	format.setSampleSize(16); // 16 bit
	format.setSampleType(QAudioFormat::SignedInt); // SignedInt currently
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setCodec("audio/pcm");

	QString outputDevice = SoundConfiguration::soundOutputDevice();
	QAudioDeviceInfo selectedInfo = QAudioDeviceInfo::defaultInputDevice();
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
		if (deviceInfo.deviceName() == SoundConfiguration::soundOutputDevice())
			selectedInfo = deviceInfo;

	if (!selectedInfo.isFormatSupported(format))
	{
		kDebug() << "Format not supported; Trying something similar";
		format = selectedInfo.nearestFormat(format);
	}

	if(format.sampleSize() != 16) {
		kDebug() << "Sample size is not 16 bit. Aborting.";
		return false;
	}

	output = new QAudioOutput(selectedInfo, format, this);
	inputData.open(QIODevice::ReadWrite);
	output->start(currentOutputClient->getDataProvider());
	kDebug() << "Started audio output";
	return true;
}

bool SoundServer::stopPlayback()
{
	Q_ASSERT(output);

	output->stop();
	delete output;
	output = NULL;

	return true;
}


/**
 * \brief Destructor
 */
SoundServer::~SoundServer()
{
}



