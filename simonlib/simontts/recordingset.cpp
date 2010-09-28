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
#include <QFile>
#include <QDir>
#include <KLocalizedString>
#include <KStandardDirs>
#include <KDebug>

RecordingSet::RecordingSet() : 
  m_isNull(true)
{
  clearTemp();
}

RecordingSet::RecordingSet(int id, const QString& name) :
  m_isNull(false),
  m_id(id),
  m_name(name)
{
}

RecordingSet::~RecordingSet()
{
  clearTemp();
  //try to remove temp directory; This will of course fail if there are files in there
  //so it will only clean the directory if it is not needed anymore;
  QDir().rmdir(getBaseDirectory());
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
  QDomElement recordingElem = recordingsElem.firstChildElement("recording");
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
  if (m_isNull || !applyTemp()) return QDomElement();

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

QString RecordingSet::getBaseDirectory() const
{
  return KStandardDirs::locateLocal("appdata", QString("ttsrec/%1/").arg(m_id));
}

QString RecordingSet::getTempDirectory() const
{
  return KStandardDirs::locateLocal("tmp", QString("simontts/ttsrec/%1/").arg(m_id));
}

QString RecordingSet::getSamplePath(int sampleId, bool absolute, bool temporary) const
{
  QString file = QString("%1.wav").arg(sampleId);
  if (!absolute)
    return file;

  if (temporary)
    return getTempDirectory()+file;
  else
    return getBaseDirectory()+file;
}

int RecordingSet::getNextId() const
{
  int id = m_recordings.count();
  while (QFile::exists(getSamplePath(id, true /*absolute*/, true /*temporary*/)) ||
        (QFile::exists(getSamplePath(id, true /*absolute*/, false /*temporary*/))) )
    id++;

  return id;
}

QString RecordingSet::getPath(const QString& text, bool forceTemp) const
{
  if (m_isNull) return QString();
  
  QString file = m_recordings.value(text.trimmed());
  QString tempFile = getTempDirectory()+file;

  if (QFile::exists(tempFile))
    return tempFile;

  if (forceTemp) return QString();

  return getBaseDirectory()+file;
}

bool RecordingSet::rename(const QString& newName)
{
  m_name = newName;
  return true;
}

bool RecordingSet::keepFile(const QString& file)
{
  kDebug() << "Keeping file: " << file;
  QFile toRemove(getTempDirectory()+"torm");
  QStringList lines;
  if (toRemove.open(QIODevice::ReadOnly))
  {
    while (!toRemove.atEnd())
    {
      QString path = QString::fromUtf8(toRemove.readLine()).trimmed();
      kDebug() << "To rm: " << path;
      if (path != file)
        lines << path;
      else
        kDebug() << "Removing: " << path;
    }
    toRemove.seek(0);
    toRemove.close();
    if (!toRemove.open(QIODevice::WriteOnly|QIODevice::Truncate))
      return false;
    foreach (const QString& l, lines)
      toRemove.write(l.toUtf8()+"\n");
    toRemove.close();
  }
  return true;
}

bool RecordingSet::addRecording(const QString& text, const QString& path)
{
  QString relativeNewPath;
  QString absoluteNewPath;
  
  //if the text is already in this set, we will overwrite the current entry
  //with this path so use the same id;
  QList<RecordingSetText> texts = m_recordings.keys();
  if (texts.contains(text))
  {
    relativeNewPath = m_recordings.value(text);
    absoluteNewPath = getTempDirectory()+relativeNewPath;
  }
  else
  {
    int id = getNextId();
    absoluteNewPath = getSamplePath(id, true, true /*temp*/);
    relativeNewPath = getSamplePath(id, false, true /*temp*/);
  }

  removeTemporarySample(text);
  if (!QFile::copy(path, absoluteNewPath))
  {
    kDebug() << "Copy failed: " << path << absoluteNewPath;
    return false;
  }

  keepFile(relativeNewPath);
  m_recordings.insert(text, relativeNewPath);
  reset();
  return true;
}

bool RecordingSet::editRecording(const QString& text, const QString& path)
{
  bool succ;
  succ = addRecording(text, path);

  reset();
  return succ;
}

bool RecordingSet::removeTemporarySample(const QString& text)
{
  QString tempPath = getPath(text, true);
  if (!tempPath.isNull())
    if (!QFile::remove(tempPath))
      return false;
  return true;
}

bool RecordingSet::removeRecording(const QString& text)
{
  kDebug() << "Removing recording for text: " << text;
  bool succ = true;
  removeTemporarySample(text);

  //add this to the list of samples to remove
  QString file = m_recordings.value(text.trimmed());
  kDebug() << "Removing recording: " << file;
  if (!file.isNull() && QFile::exists(getBaseDirectory()+file))
  {
    kDebug() << "Adding to torm";
    QFile f(getTempDirectory()+"torm");
    kDebug() << "Torm file:" << getTempDirectory()+"torm";
    if (f.open(QIODevice::WriteOnly))
    {
      kDebug() << "Wrote to file" << file.toUtf8()+"\n";
      f.write(file.toUtf8()+"\n");
    }
    else succ = false;
    f.close();
  }

  m_recordings.remove(text);
  reset();
  return succ;
}

bool RecordingSet::clearTemp()
{
  kDebug() << "Clearing temp";
  bool succ = true;

  QString tempDir = getTempDirectory();
  QDir d(tempDir);
  QStringList files = d.entryList(QDir::Files|QDir::NoDotAndDotDot);
  foreach (const QString& f, files)
    succ = QFile::remove(tempDir+f) && succ;

  return succ && d.rmdir(tempDir);
}

bool RecordingSet::applyTemp()
{
  bool succ = true;

  //move samples over existing samples
  kDebug() << "Copying all files";
  QString tempDir = getTempDirectory();
  QString baseDir = getBaseDirectory();
  QDir d(tempDir);
  kDebug() << "Temp dir: " << tempDir << d.exists();
  d.setNameFilters(QStringList() << "*.wav");
  QStringList samples = d.entryList(QDir::Files|QDir::NoDotAndDotDot);
  kDebug() << "Samples: " << samples;
  foreach (const QString& f, samples)
  {
    if (QFile::exists(baseDir+f) && !QFile::remove(baseDir+f))
    {
      kDebug() << "Failed to remove existing: " << baseDir+f;
      succ = false;
      continue;
    }

    succ = QFile::copy(tempDir+f, baseDir+f) &&
           QFile::remove(tempDir+f);

    if (!succ)
      kDebug() << "Failed to copy: " << tempDir+f << " to " << baseDir+f;
  }
  kDebug() << "Copied all files: " << succ;

  //remove samples in torm list
  QFile toRemove(tempDir+"torm");
  if (toRemove.open(QIODevice::ReadOnly))
  {
    while (!toRemove.atEnd())
    {
      QString path = QString::fromUtf8(toRemove.readLine()).trimmed();
      kDebug() << "Removing: " << path << baseDir+path;
      succ = QFile::remove(baseDir+path) && succ;
    }
    kDebug() << "Removed all scheduled files: " << succ;
    toRemove.close();
    succ = QFile::remove(tempDir+"torm") && succ;
    kDebug() << "Removed file list: " << succ;
  }

  return d.rmdir(tempDir) && succ;
}

bool RecordingSet::clear()
{
  bool succ = true;
  QList<RecordingSetText> texts = m_recordings.keys();
  foreach (const RecordingSetText& t, texts)
    succ = removeRecording(t) && succ;

  kDebug() << "Cleared";
  //succ = clearTemp() && succ;
  return succ;
}

QVariant RecordingSet::headerData(int /*section*/, Qt::Orientation orientation, int role) const
{
  if ((orientation == Qt::Vertical) || (role != Qt::DisplayRole))
    return QVariant();
  
  return i18n("Text");
}

QVariant RecordingSet::data (const QModelIndex& index, int role) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      return m_recordings.keys().at(index.row());
    case Qt::UserRole:
      QString file = m_recordings.value(m_recordings.keys().at(index.row()));
      QString baseFile = getBaseDirectory()+file;
      QString tempFile = getTempDirectory()+file;
      if (QFile::exists(tempFile)) return tempFile;
      if (QFile::exists(baseFile)) return baseFile;
      break;
  }
  return QVariant();
}

QModelIndex RecordingSet::index(int row, int column, const QModelIndex& /*parent*/) const
{
  return createIndex(row, column, 0);
}

QModelIndex RecordingSet::parent(const QModelIndex& /*index*/) const
{
  return QModelIndex();
}

