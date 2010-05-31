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

#include "sampledataprovider.h"
#include "sscconfig.h"
#include "sscdaccess.h"
#include "deviceinformationpage.h"
#include "trainsamplepage.h"
#include <sscobjects/sample.h>
#include <sscobjects/microphone.h>
#include <sscobjects/soundcard.h>

#include <QFile>

SampleDataProvider::SampleDataProvider(qint32 userId, TrainingsWizard::TrainingsType sampleType) :
	m_userId(userId), m_sampleType(sampleType), m_infoPage(NULL)
{
}

void SampleDataProvider::registerMicrophoneInfo(DeviceInformationPage* infoPage)
{
	m_infoPage = infoPage;
}

void SampleDataProvider::registerDataProvider(TrainSamplePage* trainSamplePage)
{
	m_trainSamplePages << trainSamplePage;
}

QHash<QString, Microphone*> SampleDataProvider::buildMicrophoneMappings(bool &ok)
{
	return m_infoPage->buildMicrophoneMappings(ok);
}

QHash<QString, SoundCard*> SampleDataProvider::buildSoundCardMappings(bool &ok)
{
	return m_infoPage->buildSoundCardMappings(ok);
}


bool SampleDataProvider::startTransmission()
{
	//build m_samplesToTransmit
	bool ok;
	
	kDebug() << "Building mappings...";
	QHash<QString, Microphone*> microphones = buildMicrophoneMappings(ok);
	if (!ok) return false;

	kDebug() << "Microphone done...";
	QHash<QString, SoundCard*> soundCards = buildSoundCardMappings(ok);
	if (!ok) return false;

	kDebug() << "Starting transmission";
	foreach (TrainSamplePage *page, m_trainSamplePages)
	{
		kDebug() << "Processing page";
		QStringList fileNames = page->getFileNames();
		QStringList devices = page->getDevices();

		Q_ASSERT(fileNames.count() == devices.count());

		for (int i=0; i < fileNames.count(); i++)
		{
			QString fullPath = SSCConfig::sampleDirectory()+fileNames[i]+".wav";
			QFile d(fullPath);
			if (!d.open(QIODevice::ReadOnly))
			{
				kWarning() << "Couldn't open file: " << fullPath << "; Skipping";
				continue;
			}

			Microphone *mic = microphones.value(devices[i]);
			SoundCard *soundCard = soundCards.value(devices[i]);

			kDebug() << fileNames[i] << " was recorded with " << mic->id() << mic->model() << mic->type();
			kDebug() << "                " << soundCard->id() << soundCard->model() << soundCard->type();

			m_samplesToTransmit << new Sample(-1, m_sampleType, m_userId,
					mic->id(), soundCard->id(), 
					fullPath, page->getPrompt(),
					d.readAll());
		}
	}
	qDeleteAll(microphones);
	qDeleteAll(soundCards);
	return true;
}

void SampleDataProvider::stopTransmission()
{
	qDeleteAll(m_samplesToTransmit);
	m_samplesToTransmit.clear();
}

Sample* SampleDataProvider::getSample()
{
	return m_samplesToTransmit.at(0);
}

void SampleDataProvider::sampleTransmitted()
{
	Sample *s = m_samplesToTransmit.takeAt(0);
  kDebug() << "Deleting file: " << s->path();
  bool succ = s->deleteFile();
  kDebug() << "Deleted file: " << succ;
  delete s;
}


SampleDataProvider::~SampleDataProvider()
{
}

