/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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


#include "simonsampledataprovider.h"
#include <sscobjects/microphone.h>
#include <sscobjects/soundcard.h>
#include <sscdaccess/trainingsamplesdescriptor.h>
#include <simonscenarios/trainingmanager.h>
#include <KDebug>

SimonSampleDataProvider::SimonSampleDataProvider(qint32 userId, Microphone *microphone, 
			    SoundCard *soundCard, Sample::SampleType sampleType, 
			    const QString& name) :
  AbstractSampleDataProvider(userId, sampleType, name, true),
  m_microphone(microphone), m_soundCard(soundCard)
{
}

SimonSampleDataProvider::~SimonSampleDataProvider()
{
  delete m_soundCard;
  delete m_microphone;
}

bool SimonSampleDataProvider::store()
{
  kWarning() << "Not implemented";
  return false;
}

QHash< QString, SoundCard* > SimonSampleDataProvider::buildSoundCardMappings(bool& ok)
{
  ok = true;
  QHash< QString, SoundCard* > soundCards;
  soundCards.insert("simon", new SoundCard(*m_soundCard));
  return soundCards;
}

QHash< QString, Microphone* > SimonSampleDataProvider::buildMicrophoneMappings(bool& ok)
{
  ok = true;
  QHash< QString, Microphone* > microphones;
  microphones.insert("simon", new Microphone(*m_microphone));
  return microphones;
}

QList< TrainingSamplesDescriptor* > SimonSampleDataProvider::buildSampleDescriptors(bool& ok)
{
  ok = true;
  QList< TrainingSamplesDescriptor* > sampleDescriptors;
  
  PromptsTable *prompts = TrainingManager::getInstance()->getPrompts();
  QString dir = TrainingManager::getInstance()->getTrainingDir();
  for (QHash<QString, QString>::const_iterator i = prompts->samples().constBegin();
       i != prompts->samples().constEnd(); ++i) {
    sampleDescriptors << new TrainingSamplesDescriptor(i.value(),
						       QStringList() << dir+i.key()+".wav",
						       QStringList() << "simon");
  }
  return sampleDescriptors;
}
