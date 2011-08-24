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

#include "sscsampledataprovider.h"
#include "sscconfig.h"
#include <sscdaccess/sscdaccess.h>
#include <sscdaccess/trainingsamplesdescriptor.h>
#include "deviceinformationpage.h"
#include "trainsamplepage.h"
#include <sscobjects/sample.h>
#include <sscobjects/microphone.h>
#include <sscobjects/soundcard.h>

#include <QFile>
#include <QSettings>

#include <KDebug>

SSCSampleDataProvider::SSCSampleDataProvider(qint32 userId, Sample::SampleType sampleType,
const QString& name) : AbstractSampleDataProvider(userId, sampleType, name),
m_infoPage(0)
{
}

void SSCSampleDataProvider::registerMicrophoneInfo(DeviceInformationPage* infoPage)
{
  m_infoPage = infoPage;
}


void SSCSampleDataProvider::registerDataProvider(TrainSamplePage* trainSamplePage)
{
  m_trainSamplePages << trainSamplePage;
}


QHash<QString, Microphone*> SSCSampleDataProvider::buildMicrophoneMappings(bool &ok)
{
  return m_infoPage->buildMicrophoneMappings(ok);
}


QHash<QString, SoundCard*> SSCSampleDataProvider::buildSoundCardMappings(bool &ok)
{
  return m_infoPage->buildSoundCardMappings(ok);
}

QList<TrainingSamplesDescriptor*> SSCSampleDataProvider::buildSampleDescriptors(bool &ok)
{
  QList<TrainingSamplesDescriptor*> sampleDescriptors;
  foreach (TrainSamplePage *page, m_trainSamplePages) {
    kDebug() << "Processing page";
    QStringList fileNames = page->getFileNames();
    QStringList devices = page->getDevices();
    QString prompt = page->getPrompt();
    sampleDescriptors << new TrainingSamplesDescriptor(prompt, fileNames, devices);
  }
  ok = true;
  return sampleDescriptors;
}

bool SSCSampleDataProvider::store()
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


SSCSampleDataProvider::~SSCSampleDataProvider()
{
}
