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
	m_output(NULL)
{
	open(QIODevice::ReadWrite);
}


qint64 SimonSoundOutput::readData(char *toRead, qint64 maxLen)
{
	Q_UNUSED(toRead);
	Q_UNUSED(maxLen);
	//TODO
	/*
	if (!currentOutputClient)
	{
		kDebug() << "No current output client";
		return -1;
	}

	qint64 read = currentOutputClient->getDataProvider()->read(toRead, maxLen);

	if (read <= 0)
		deRegisterOutputClient(currentOutputClient);

	return read;
	*/
	return 0;
}


qint64 SimonSoundOutput::writeData(const char *toWrite, qint64 len)
{
	Q_UNUSED(toWrite);
	Q_UNUSED(len);
	//FIXME: split this
	/*QByteArray data;
	data.append(toWrite, len);

	//length is in ms
	qint64 length = byteSizeToLength(data.count());

	//pass data on to all registered, active clients
	
	QList<SoundInputClient*> active = inputs.activeInputClients.keys();
	foreach (SoundInputClient *c, active)
	{
		qint64 streamTime = activeInputClients.value(c)+length;
		c->process(data, streamTime);
		//update time stamp
		activeInputClients.insert(c, streamTime);
	}
	return len;*/
	return 0;
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

