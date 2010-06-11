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


#include "simonsoundinput.h"
#include <simonsound/soundinputclient.h>
#include <simonsound/soundserver.h>

#include <QAudioInput>
#include <KDebug>
#include <KLocalizedString>


SimonSoundInput::SimonSoundInput(QObject *parent) : QIODevice(parent),
	m_input(NULL)
{
	open(QIODevice::ReadWrite);
}

qint64 SimonSoundInput::readData(char *toRead, qint64 maxLen)
{
	Q_UNUSED(toRead);
	Q_UNUSED(maxLen);
	return 0;
}


qint64 SimonSoundInput::writeData(const char *toWrite, qint64 len)
{
	QByteArray data;
	data.append(toWrite, len);

	//length is in ms
	qint64 length = SoundServer::getInstance()->byteSizeToLength(data.count(), m_device);

	//pass data on to all registered, active clients
	foreach (SoundInputClient *c, m_activeInputClients.keys())
	{
		qint64 streamTime = m_activeInputClients.value(c)+length;
		c->process(data, streamTime);
		//update time stamp
		m_activeInputClients.insert(c, streamTime);
	}
	return len;
}

bool SimonSoundInput::startRecording(SimonSound::DeviceConfiguration& device)
{
	kDebug() << "Starting recording";

	QAudioFormat format;
	format.setFrequency(device.sampleRate());
	format.setChannels(device.channels());
	format.setSampleSize(16); // 16 bit
	format.setSampleType(QAudioFormat::SignedInt); // SignedInt currently
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setCodec("audio/pcm");

	QAudioDeviceInfo selectedInfo = QAudioDeviceInfo::defaultInputDevice();
	bool deviceFound = false;
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
	{
		if (deviceInfo.deviceName() == device.name())
		{
			selectedInfo = deviceInfo;
			deviceFound = true;
		}
	}
	if (!deviceFound)
	{
		emit error(i18n("The selected sound input device \"%1\" is not available.", device.name()));
		return false;
	}


	if (!selectedInfo.isFormatSupported(format))
	{
		kDebug() << "Format not supported";
		emit error(i18n("Recording format not supported."));
		emit inputStateChanged(QAudio::StoppedState);
		return false;
	}

	m_input = new QAudioInput(selectedInfo, format, this);
	connect(m_input, SIGNAL(stateChanged(QAudio::State)), this, SLOT(slotInputStateChanged(QAudio::State)));
	connect(m_input, SIGNAL(stateChanged(QAudio::State)), this, SIGNAL(inputStateChanged(QAudio::State)));

	m_input->start(this);

	kDebug() << "Started audio input";
	m_device = device;
	return true;
}

void SimonSoundInput::suspendInputClients()
{
	QHashIterator<SoundInputClient*, qint64> j(m_activeInputClients);
	while (j.hasNext())
	{
		j.next();
		suspend(j.key());
	}
}

void SimonSoundInput::suspend(SoundInputClient* client)
{
	client->suspend();
	m_suspendedInputClients.insert(client, m_activeInputClients.value(client));
	m_activeInputClients.remove(client);
}

void SimonSoundInput::resume(SoundInputClient* client)
{
	m_activeInputClients.insert(client, m_suspendedInputClients.value(client));
	m_suspendedInputClients.remove(client);
	client->resume();
}

void SimonSoundInput::registerInputClient(SoundInputClient* client)
{
	m_activeInputClients.insert(client, 0);
}


bool SimonSoundInput::deRegisterInputClient(SoundInputClient* client)
{
	kDebug() << "Deregistering input client";

	if (m_activeInputClients.remove(client) == 0)
	{
		//wasn't active anyways
		/*return */
		m_suspendedInputClients.remove(client);
	}

	bool success = true;

	if (m_activeInputClients.isEmpty() && m_suspendedInputClients.isEmpty()) //don't need to record any longer
	{
		//then stop recording
		kDebug() << "No active clients available... Stopping recording";
		success = stopRecording();
		if (success)
			emit recordingFinished(); // destroy this sound input
	}

	return success;
}

SoundClient::SoundClientPriority SimonSoundInput::getHighestPriority()
{
	SoundClient::SoundClientPriority priority = SoundClient::Background;
	QHashIterator<SoundInputClient*, qint64> j(m_activeInputClients);
	while (j.hasNext())
	{
		j.next();
		priority = qMax(priority, j.key()->priority());
	}
	QHashIterator<SoundInputClient*, qint64> i(m_suspendedInputClients);
	while (i.hasNext())
	{
		i.next();
		priority = qMax(priority, i.key()->priority());
	}
	return priority;
}

bool SimonSoundInput::activate(SoundClient::SoundClientPriority priority)
{
	kDebug() << "Activating priority: " << priority;
	bool activated = false;
	QHashIterator<SoundInputClient*, qint64> j(m_activeInputClients);
	while (j.hasNext())
	{
		j.next();
		if (j.key()->priority() == priority)
		{
			if (priority == SoundClient::Exclusive)
			{
				if (activated)
					suspend(j.key());
				else activated = true;
			}
		}
		if (j.key()->priority() < priority)
		{
			kDebug() << "Suspending key...";
			suspend(j.key());
		}
	}

	if (activated && priority == SoundClient::Exclusive)
		return true; // never more than one exclusive client

	QHashIterator<SoundInputClient*, qint64> i(m_suspendedInputClients);
	while (i.hasNext())
	{
		i.next();
		if (priority == i.key()->priority())
		{
			resume(i.key());
			activated = true;
			if (i.key()->priority() == SoundClient::Exclusive)
				return true;
		}

	}
	return activated;
}

void SimonSoundInput::slotInputStateChanged(QAudio::State state)
{
	kDebug() << "Input state changed: " << state;

	foreach (SoundInputClient *c, m_activeInputClients.keys())
		c->inputStateChanged(state);

	if (!m_input) return;

	if (state == QAudio::StoppedState)
	{
		switch (m_input->error())
		{
			case QAudio::NoError:
				kDebug() << "Input stopped without error";
				break;
			case QAudio::OpenError:
				emit error(i18n("Failed to open the input audio device.\n\nPlease check your sound configuration."));
				break;

			case QAudio::IOError:
				emit error(i18n("An error occured while reading data from the audio device."));
				break;

			case QAudio::UnderrunError:
				emit error(i18n("Buffer underrun when processing the sound data."));
				break;

			case QAudio::FatalError:
				emit error(i18n("A fatal error occured during recording."));
				break;
		}
	}
}

bool SimonSoundInput::stopRecording()
{
	kDebug() << "Stopping recording";
	if (!m_input) return true;

	m_input->disconnect(this);
	m_input->stop();
	m_input->deleteLater();
	m_input = NULL;

	return true;
}

void SimonSoundInput::suspendInput()
{
	if (!m_input) return;
	m_input->suspend();
}

void SimonSoundInput::resumeInput()
{
	if (!m_input) return;
	m_input->resume();
}
	
SimonSoundInput::~SimonSoundInput()
{
	if (m_input)
		m_input->deleteLater();
}

