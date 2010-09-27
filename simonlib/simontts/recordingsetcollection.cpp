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

#include "recordingsetcollection.h"
#include "recordingset.h"
#include "ttsconfiguration.h"
#include <QFile>
#include <QDomDocument>

RecordingSetCollection::RecordingSetCollection()
{
}

void RecordingSetCollection::deleteAll()
{
  qDeleteAll(m_sets);
  m_sets.clear();
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

  QDomDocument doc;
  QDomElement rootElem = doc.createElement("ttssets");
  foreach (RecordingSet *set, m_sets)
    rootElem.appendChild(set->serialize(&doc));
  doc.appendChild(rootElem);
  
  f.write(doc.toString().toUtf8());

  f.close();
  return true;
}

bool RecordingSetCollection::canSay(const QString& text)
{
  return !getPath(text).isNull();
}

QString RecordingSetCollection::getPath(const QString& text)
{
  QList<RecordingSet*> activeSets = getActiveSets();
  QString path;
  foreach (RecordingSet *s, m_sets)
  {
    path = s->getPath(text);
    if (!path.isNull())
      return path;
  }
  
  return QString();
}

RecordingSet* RecordingSetCollection::getSet(int id)
{
  foreach (RecordingSet *s, m_sets)
    if (s->id() ==  id)
      return s;
  return 0;
}

QList<RecordingSet*> RecordingSetCollection::getActiveSets()
{
  int activeSetId = TTSConfiguration::activeSet();
  QList<RecordingSet*> activeSets;
  RecordingSet *activeSet = getSet(activeSetId);
  if (activeSet)
    activeSets << activeSet;

  if (TTSConfiguration::useRecordingsAcrossSets())
    foreach (RecordingSet *s, m_sets)
      if (s != activeSet)
        activeSets << s;

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
  if (!set) return false;
  m_sets.removeAll(set);
  delete set;
  return true;
}

RecordingSetCollection::~RecordingSetCollection()
{
  deleteAll();
}

