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
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
		if (deviceInfo.deviceName() == device.name())
			selectedInfo = deviceInfo;

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
		m_suspendedInputClients.remove(client);
		return true;
	}
	if (client->isExclusive())
	{
		QHashIterator<SoundInputClient*, qint64> j(m_suspendedInputClients);

		/// if we have one exclusive input in the suspended list move it to the active
		/// list and ignore the rest
		///
		/// @note The order in which the input clients are resumed is undefined
		///
		///otherwise move everything back
		
		bool hasExclusive = false;
		while (j.hasNext())
		{
			j.next();
			if (j.key()->isExclusive())
			{
				m_activeInputClients.insert(j.key(), j.value());
				m_suspendedInputClients.remove(j.key());
				hasExclusive = true;
				break;
			}
		}
		if (!hasExclusive)
		{
			j.toFront();
			while (j.hasNext())
			{
				j.next();
				j.key()->resume();
				m_activeInputClients.insert(j.key(), j.value());
				m_suspendedInputClients.remove(j.key());
			}
		}
	}

	bool success = true;

	//FIXME SYSTEM WIDE RESUME!
	if (m_activeInputClients.isEmpty()) //don't need to record any longer
	{
		//then stop recording
		kDebug() << "No active clients available... Stopping recording";
		success = stopRecording();
		if (success)
			emit recordingFinished(); // destroy this sound input
	}

	return success;
}

void SimonSoundInput::slotInputStateChanged(QAudio::State state)
{
	kDebug() << "Input state changed: " << state;

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
	delete m_input;
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

