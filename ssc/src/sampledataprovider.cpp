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
#include <QSettings>

#include <KDebug>

SampleDataProvider::SampleDataProvider(qint32 userId, TrainingsWizard::TrainingsType sampleType,
const QString& name) :
m_userId(userId), m_sampleType(sampleType), m_name(name), m_infoPage(0)
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


bool SampleDataProvider::store()
{
  QString directory = KStandardDirs::locateLocal("appdata", QString("stored/%1/%2/").arg(m_userId).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd.hh-mm-ss-zzz")));
  fprintf(stderr, "Created directory: %s\n", directory.toAscii().constData());
  QSettings ini(directory+"/profile.ini", QSettings::IniFormat);
  kDebug() << "Profiles: " << directory+"/profile.ini";

  ini.setValue("Type", m_sampleType);
  ini.setValue("Name", m_name);

  bool succ = true;
  succ = m_infoPage->serializeToStorage(ini) && succ;

  ini.beginGroup("Samples");
  ini.beginWriteArray("Sample");
  int i=0;
  foreach (TrainSamplePage *page, m_trainSamplePages) {
    ini.setArrayIndex(i);
    succ = page->serializeToStorage(ini, directory) && succ;
    i++;
  }
  ini.endArray();
  ini.endGroup();

  return succ;
}


bool SampleDataProvider::startTransmission()
{
  //build m_samplesToTransmit
  bool ok;

  kDebug() << "Building mappings...";
  fprintf(stderr, "Building mappings\n");
  QHash<QString, Microphone*> microphones = buildMicrophoneMappings(ok);
  if (!ok) {
    fprintf(stderr, "Mapping building failed.\n");
    return false;
  }

  kDebug() << "Microphone done...";
  fprintf(stderr, "Building soundcards mappings\n");
  QHash<QString, SoundCard*> soundCards = buildSoundCardMappings(ok);
  if (!ok) {
    fprintf(stderr, "Building mappings for soundcards failed\n");
    return false;
  }

  fprintf(stderr, "Starting transmission\n");
  kDebug() << "Starting transmission";
  foreach (TrainSamplePage *page, m_trainSamplePages) {
    kDebug() << "Processing page";
    QStringList fileNames = page->getFileNames();
    QStringList devices = page->getDevices();

    Q_ASSERT(fileNames.count() == devices.count());

    for (int i=0; i < fileNames.count(); i++) {
      QString fullPath = /*SSCConfig::sampleDirectory()+*/fileNames[i]/*+".wav"*/;
      QFile d(fullPath);
      if (!d.open(QIODevice::ReadOnly)) {
        kWarning() << "Could not open file: " << fullPath << "; Skipping";
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


void SampleDataProvider::skipSample()
{
  delete m_samplesToTransmit.takeAt(0);
}


SampleDataProvider::~SampleDataProvider()
{
}
