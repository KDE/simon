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

#include "recordingsetcollection.h"
#include "recordingset.h"
#include "ttsconfiguration.h"
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <KArchive/KTar>

RecordingSetCollection::RecordingSetCollection()
{
}

void RecordingSetCollection::deleteAll()
{
  qDeleteAll(m_sets);
  m_sets.clear();
  qDeleteAll(m_setsScheduledForDeletion);
  m_setsScheduledForDeletion.clear();
}

bool RecordingSetCollection::init(const QString& path)
{
  deleteAll();

  QFile f(path);
  if (!f.open(QIODevice::ReadOnly))
    return false;
  
  QDomDocument doc;
  doc.setContent(QString::fromUtf8(f.readAll()));
  QDomElement rootElem = doc.documentElement();
  if (rootElem.isNull()) return false;
  QDomElement setElem = rootElem.firstChildElement("ttsset");
  while (!setElem.isNull())
  {
    RecordingSet *set = new RecordingSet;
    if (!set->deserialize(setElem))
      delete set;
    else
      m_sets << set;

    setElem = setElem.nextSiblingElement("ttsset");
  }
  return true;
}

bool RecordingSetCollection::save(const QString& path)
{
  QFile f(path);
  if (!f.open(QIODevice::WriteOnly)) return false;

  bool succ = true;
  succ = purgeSelectedSets();
  
  QDomDocument doc;
  QDomElement rootElem = doc.createElement("ttssets");
  foreach (RecordingSet *set, m_sets)
  {
    if (!set->applyTemp())
    {
      succ = false;
      continue;
    }

    QDomElement elem = set->serialize(&doc);
    if (elem.isNull())
    {
      succ = false;
      continue;
    }
    rootElem.appendChild(elem);
  }
  doc.appendChild(rootElem);
  
  f.write(doc.toString().toUtf8());

  f.close();
  return succ;
}

bool RecordingSetCollection::exportSet(const QString& path, int setId) const
{
  RecordingSet *set = getSet(setId);
  if (!set) return false;

  QDomDocument doc;
  QDomElement elem = set->serialize(&doc);
  if (elem.isNull())
    return false;

  QDomElement rootElem = doc.createElement("ttssets");
  rootElem.appendChild(elem);
  doc.appendChild(rootElem);

  QByteArray data = doc.toString().toUtf8();

  QString dataDir = QDir::tempPath() + QLatin1Char('/') +  "simontts/ttsrec/export/";

  if (!set->exportData(dataDir))
    return false;

  KTar tar(path, "application/x-gzip");
  if (!tar.open(QIODevice::WriteOnly))
  {
    qDebug() << "Couldn't open output archive" << path;
    return false;
  }

  bool succ = true;
  QString archiveDataDir = QString::number(setId)+'/';
  succ = tar.writeFile("set.xml", "", "", data.constData(), data.size()) && succ;
  succ = tar.writeDir(archiveDataDir, "", "") && succ;

  QDir d(dataDir);
  QStringList files = d.entryList(QDir::NoDotAndDotDot|QDir::Files);
  foreach (const QString& file, files)
  {
    QFile f(dataDir+file);
    if (!f.open(QIODevice::ReadOnly))
    {
      qDebug() << "Couldn't read: " << file;
      succ = false;
      continue;
    }
    QByteArray recordingData = f.readAll();
    f.close();
    f.remove();
    tar.writeFile(archiveDataDir+file, "", "", recordingData.constData(), recordingData.size());
  }
  tar.close();
  d.rmdir(dataDir);
  return true;
}

bool RecordingSetCollection::importSet(const QString& path)
{
  KTar tar(path, "application/x-gzip");
  if (!tar.open(QIODevice::ReadOnly))
  {
    qDebug() << "Couldn't open input archive: " << path;
    return false;
  }
  const KArchiveDirectory *directory = tar.directory();
  if (!directory) return false;
  QString importDirectory = QDir::tempPath() + QLatin1Char('/') +  "simontts/ttsrec/import/";
  directory->copyTo(importDirectory);


  QFile f(importDirectory+"set.xml");
  if (!f.open(QIODevice::ReadOnly))
    return false;
  QDomDocument doc;
  doc.setContent(QString::fromUtf8(f.readAll()));
  f.close();
  f.remove();

  QDomElement rootElem = doc.documentElement();
  if (rootElem.isNull()) return false;
  QDomElement setElem = rootElem.firstChildElement("ttsset");
  if (setElem.isNull()) return false;
  setElem.setAttribute("id", getFreeId());

  RecordingSet *set = new RecordingSet;
  QString setDataDir = importDirectory+QString::number(set->id())+'/';
  if (!set->deserialize(setElem) || 
      !set->importData(setDataDir))
  {
    set->clear();
    delete set;
    return false;
  }

  m_sets << set;
  QDir d;
  return d.rmdir(setDataDir) && d.rmdir(importDirectory);
}

bool RecordingSetCollection::canSay(const QString& text)
{
  return !getPath(text).isNull();
}

QString RecordingSetCollection::getPath(const QString& text)
{
  QList<RecordingSet*> activeSets = getActiveSets();
  QString path;
  foreach (RecordingSet *s, activeSets)
  {
    path = s->getPath(text);
    if (!path.isNull())
      return path;
  }
  
  return QString();
}

RecordingSet* RecordingSetCollection::getSet(int id) const
{
  foreach (RecordingSet *s, m_sets)
    if (s->id() ==  id)
      return s;
  return 0;
}

QList<RecordingSet*> RecordingSetCollection::getActiveSets()
{
  int activeSetId = TTSConfiguration::activeSet();
  qDebug() << "Active set: " << activeSetId;
  QList<RecordingSet*> activeSets;
  RecordingSet *activeSet = getSet(activeSetId);
  if (activeSet)
    activeSets << activeSet;

  if (TTSConfiguration::useRecordingsAcrossSets())
  {
    qDebug() << "Using recordings acorss sets";
    foreach (RecordingSet *s, m_sets)
      if (s != activeSet)
        activeSets << s;
  }

  return activeSets;
}

QList<int> RecordingSetCollection::getSets()
{
  QList<int> setIds;
  foreach (RecordingSet *s, m_sets)
    setIds << s->id();
  return setIds;
}

int RecordingSetCollection::getFreeId()
{
  QList<int> takenIds = getSets();
  int i=0;
  while (takenIds.contains(i)) i++;
  return i;
}


bool RecordingSetCollection::addSet(const QString& name)
{
  m_sets << new RecordingSet(getFreeId(), name);
  return true;
}

bool RecordingSetCollection::renameSet(int id, const QString& name)
{
  RecordingSet *s = getSet(id);
  if (!s) return false;

  return s->rename(name);
}

bool RecordingSetCollection::removeSet(int id)
{
  RecordingSet *set = getSet(id);
  if (!set || !set->clear()) return false;
  m_sets.removeAll(set);
  m_setsScheduledForDeletion << set;
  qDebug() << "Scheduling set for deletion";
  return true;
}

bool RecordingSetCollection::purgeSelectedSets()
{
  bool succ = true;
  foreach (RecordingSet* set, m_setsScheduledForDeletion)
  {
    qDebug() << "Purging set";
    succ = set->applyTemp() && succ;
    m_setsScheduledForDeletion.removeAll(set);
    delete set;
  }
  return succ;
}

bool RecordingSetCollection::addRecording(int id, const QString& text, const QString& path)
{
  RecordingSet *set = getSet(id);
  if (!set) return false;
  return set->addRecording(text, path);
}

bool RecordingSetCollection::editRecording(int id, const QString& text, const QString& path)
{
  RecordingSet *set = getSet(id);
  if (!set) return false;
  return set->editRecording(text, path);
}

bool RecordingSetCollection::removeRecording(int id, const QString& text)
{
  RecordingSet *set = getSet(id);
  if (!set) return false;
  return set->removeRecording(text);
}

RecordingSetCollection::~RecordingSetCollection()
{
  deleteAll();
}

