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


#include "simonsoundoutput.h"

#include <simonsound/soundoutputclient.h>
#include <QAudioOutput>
#include <KLocalizedString>
#include <KDebug>

SimonSoundOutput::SimonSoundOutput(QObject *parent) : QIODevice(parent),
	m_output(NULL),
	m_activeOutputClient(NULL)
{
	open(QIODevice::ReadWrite);
}


qint64 SimonSoundOutput::readData(char *toRead, qint64 maxLen)
{
	Q_UNUSED(toRead);
	Q_UNUSED(maxLen);
	
	if (!m_activeOutputClient)
	{
		kDebug() << "No current output client";
		return -1;
	}

	qint64 read = m_activeOutputClient->getDataProvider()->read(toRead, maxLen);

	if (read <= 0)
		deRegisterOutputClient(m_activeOutputClient);

	return read;
	return 0;
}


qint64 SimonSoundOutput::writeData(const char *toWrite, qint64 len)
{
	Q_UNUSED(toWrite);
	Q_UNUSED(len);
	return 0;
}

void SimonSoundOutput::registerOutputClient(SoundOutputClient* client)
{
	if (m_activeOutputClient != NULL)
		m_suspendedOutputClients.insert(0,m_activeOutputClient);

	m_activeOutputClient = client;
}

bool SimonSoundOutput::deRegisterOutputClient(SoundOutputClient* client)
{
	kDebug() << "Deregister output client";
	client->finish();
	if (client != m_activeOutputClient)
	{
		//wasn't active anyways
		m_suspendedOutputClients.removeAll(client);
		return true;
	}

	

	if (m_suspendedOutputClients.isEmpty())
	{
		m_activeOutputClient= NULL;
		kDebug() << "No active clients available... Stopping playback";
		return stopPlayback();
	}

	m_activeOutputClient = m_suspendedOutputClients.takeAt(0);
	return true;
}

bool SimonSoundOutput::startPlayback(SimonSound::DeviceConfiguration& device)
{
	kDebug() << "Starting playback...";

	//FIXME
	//kDebug() << "Suspending recording during playback";
	//suspendRecording();

	QAudioFormat format;
	format.setFrequency(device.sampleRate());
	format.setChannels(device.channels());
	format.setSampleSize(16); // 16 bit
	format.setSampleType(QAudioFormat::SignedInt); // SignedInt currently
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setCodec("audio/pcm");

	QAudioDeviceInfo selectedInfo = QAudioDeviceInfo::defaultOutputDevice();
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
		if (deviceInfo.deviceName() == device.name())
			selectedInfo = deviceInfo;

	if (!selectedInfo.isFormatSupported(format))
	{
		kDebug() << "Format not supported; Trying something similar";
		format = selectedInfo.nearestFormat(format);
	}

	if(format.sampleSize() != 16) {
		kDebug() << "Sample size is not 16 bit. Aborting.";
		emit error(i18n("Sample size not equal to 16 bit."));
		emit outputStateChanged(QAudio::StoppedState);
		return false;
	}
	
	device.setChannels(format.channels());
	device.setSampleRate(format.frequency());

	kDebug() << "Using device: " << selectedInfo.deviceName();

	m_output = new QAudioOutput(selectedInfo, format, this);
	connect(m_output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(slotOutputStateChanged(QAudio::State)));
	connect(m_output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(outputStateChanged(QAudio::State)));
	m_output->start(this);

	kDebug() << "Started audio output";
	return true;
}


void SimonSoundOutput::slotOutputStateChanged(QAudio::State state)
{
	kDebug() << "Output state changed: " << state;

	if (state == QAudio::StoppedState)
	{
		switch (m_output->error())
		{
			case QAudio::NoError:
				kDebug() << "Output stopped without error";
				break;
			case QAudio::OpenError:
				emit error(i18n("Failed to open the audio device.\n\nPlease check your sound configuration."));
				break;

			case QAudio::IOError:
				emit error(i18n("An error occured while writing data to the audio device."));
				break;

			case QAudio::UnderrunError:
				kWarning() << i18n("Buffer underrun when processing the sound data.");
				break;

			case QAudio::FatalError:
				emit error(i18n("A fatal error occured during playback."));
				break;
		}
	}
}

SimonSoundOutput::~SimonSoundOutput()
{
	m_output->deleteLater();
}

void SimonSoundOutput::suspendOutput()
{
	if (!m_output) return;
	m_output->suspend();
}

void SimonSoundOutput::resumeOutput()
{
	if (!m_output) return;
	m_output->suspend();
}

bool SimonSoundOutput::stopPlayback()
{
	kDebug() << "Stop playback...";
	if (!m_output) return true;

	m_output->stop();
	m_output->disconnect(this);
	m_output->deleteLater();
	m_output = NULL;

	//FIXME
//	reset();

	return true;
}

