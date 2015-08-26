/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "abstractsampledataprovider.h"

#include <sscdaccess/sscdaccess.h>
#include <sscdaccess/trainingsamplesdescriptor.h>
#include <sscobjects/sample.h>
#include <sscobjects/microphone.h>
#include <sscobjects/soundcard.h>

#include <QStringList>
#include <QFile>

#include <QDebug>
#include <qvarlengtharray.h>

AbstractSampleDataProvider::AbstractSampleDataProvider(qint32 userId, Sample::SampleType sampleType,
const QString& name, bool keepSamples) :
m_userId(userId), m_sampleType(sampleType), m_name(name), m_keepSamples(keepSamples)
{
}


bool AbstractSampleDataProvider::startTransmission()
{
  //build m_samplesToTransmit
  bool ok;

  qDebug() << "Building mappings...";
  fprintf(stderr, "Building mappings\n");
  QHash<QString, Microphone*> microphones = buildMicrophoneMappings(ok);
  if (!ok) {
    fprintf(stderr, "Mapping building failed.\n");
    return false;
  }
  qDebug() << "Found Microphones: " << microphones.keys();

  qDebug() << "Microphone done...";
  fprintf(stderr, "Building soundcards mappings\n");
  QHash<QString, SoundCard*> soundCards = buildSoundCardMappings(ok);
  if (!ok) {
    fprintf(stderr, "Building mappings for soundcards failed\n");
    return false;
  }
  qDebug() << "Found Soundcards: " << soundCards.keys();

  QList<TrainingSamplesDescriptor*> sampleDescriptors = buildSampleDescriptors(ok);
  if (!ok) {
    fprintf(stderr, "Building sample descriptors failed\n");
    return false;
  }

  fprintf(stderr, "Starting transmission\n");
  qDebug() << "Starting transmission";

  foreach (TrainingSamplesDescriptor *sample, sampleDescriptors) {
    qDebug() << "Processing page";
    QStringList fileNames = sample->getFileNames();
    QStringList devices = sample->getDevices();
    QString prompt = sample->getPrompt();

    qDebug() << "Page contains these files: " << fileNames;
    qDebug() << "Recorded with these devices: " << devices;
    Q_ASSERT(fileNames.count() == devices.count());

    for (int i=0; i < fileNames.count(); i++) {
      QString fullPath = fileNames[i];
      QFile d(fullPath);
      if (!d.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file: " << fullPath << "; Skipping";
        continue;
      }

      Microphone *mic = microphones.value(devices[i]);
      SoundCard *soundCard = soundCards.value(devices[i]);

      qDebug() << fileNames[i] << " was recorded with " << mic->id() << mic->model() << mic->type();
      qDebug() << "                " << soundCard->id() << soundCard->model() << soundCard->type();

      m_samplesToTransmit << new Sample(-1, m_sampleType, m_userId,
        mic->id(), soundCard->id(),
        fullPath, prompt,
        d.readAll());
    }
  }
  qDeleteAll(microphones);
  qDeleteAll(soundCards);
  return true;
}


void AbstractSampleDataProvider::stopTransmission()
{
  qDebug() << "Calling stop transmission!";
  qDeleteAll(m_samplesToTransmit);
  m_samplesToTransmit.clear();
}


Sample* AbstractSampleDataProvider::getSample()
{
  return m_samplesToTransmit.at(0);
}


void AbstractSampleDataProvider::sampleTransmitted()
{
  Sample *s = m_samplesToTransmit.takeAt(0);
  if (!m_keepSamples) {
    qDebug() << "Deleting file: " << s->path();
    bool succ = s->deleteFile();
    qDebug() << "Deleted file: " << succ;
  }
  delete s;
}


void AbstractSampleDataProvider::skipSample()
{
  delete m_samplesToTransmit.takeAt(0);
}


AbstractSampleDataProvider::~AbstractSampleDataProvider()
{
  qDebug() << "Deleting abstract sample data provider";
  qDeleteAll(m_samplesToTransmit);
}
