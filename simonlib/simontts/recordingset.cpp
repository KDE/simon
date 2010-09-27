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

#include "recordingset.h"
#include <QStringList>
#include <KDebug>

RecordingSet::RecordingSet() : 
  m_isNull(true)
{

}

RecordingSet::RecordingSet(int id, const QString& name) :
  m_isNull(false),
  m_id(id),
  m_name(name)
{
}

bool RecordingSet::deserialize(const QDomElement& elem)
{
  if (elem.isNull()) return false;

  bool ok = true;
  m_id = elem.attribute("id").toInt(&ok);
  if (!ok) return false;

  QDomElement nameElem = elem.firstChildElement("name");
  m_name = nameElem.text();

  QDomElement recordingsElem = elem.firstChildElement("recordings");
  QDomElement recordingElem = elem.firstChildElement("recording");
  while (!recordingElem.isNull())
  {
    QDomElement textElem = recordingElem.firstChildElement("text");
    QDomElement pathElem = recordingElem.firstChildElement("path");

    kDebug() << "Provides recording: " << textElem.text() << pathElem.text();
    m_recordings.insert(RecordingSetText(textElem.text()), pathElem.text());

    recordingElem = recordingElem.nextSiblingElement("recording");
  }

  m_isNull = false;
  return true;
}

QDomElement RecordingSet::serialize(QDomDocument *doc)
{
  if (m_isNull) return QDomElement();

  QDomElement recordingSetElem = doc->createElement("ttsset");
  recordingSetElem.setAttribute("id", QString::number(m_id));

  QDomElement nameElem = doc->createElement("name");
  nameElem.appendChild(doc->createTextNode(m_name));
  QDomElement recordingsElem = doc->createElement("recordings");
  QList<RecordingSetText> texts = m_recordings.keys();

  kDebug() << "Available texts:";
  kDebug() << texts;

  foreach (const RecordingSetText& text, texts)
  {
    QDomElement recordingElem = doc->createElement("recording");
    QDomElement textElem = doc->createElement("text");
    QDomElement pathElem = doc->createElement("path");
    textElem.appendChild(doc->createTextNode(text));
    pathElem.appendChild(doc->createTextNode(m_recordings.value(text)));
    recordingElem.appendChild(textElem);
    recordingElem.appendChild(pathElem);
    recordingsElem.appendChild(recordingElem);
  }

  recordingSetElem.appendChild(nameElem);
  recordingSetElem.appendChild(recordingsElem);
  return recordingSetElem;
}

QString RecordingSet::getPath(const QString& text)
{
  if (m_isNull) return QString();

  return m_recordings.value(text.trimmed());
}

bool RecordingSet::rename(const QString& newName)
{
  m_name = newName;
  return true;
}

