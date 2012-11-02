/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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

#include "promptstable.h"
#include "word.h"
#include "speechmodelmanagementconfiguration.h"
#include <QFile>
#include <KDebug>

PromptsTable::PromptsTable() : m_model(0)
{
}

PromptsTableModel* PromptsTable::getModel()
{
  if (!m_model)
    m_model = new PromptsTableModel(this);
  return m_model;
}

void PromptsTable::updateModel()
{
  if (m_model) m_model->reset();
}

bool PromptsTable::init(const QString& path)
{
  QFile *prompts = new QFile ( path );
  prompts->open ( QFile::ReadOnly );
  if ( !prompts->isReadable() ) return false;

  QString label;
  QString group;
  QString prompt;
  QString line;
  int index;
  while ( !prompts->atEnd() ) {                   //for each line that was successfully read
    QByteArray rawLine = prompts->readLine ( 1024 );
    line = QString::fromUtf8(rawLine);

    QStringList splitLine = line.split('"');

    if (splitLine.count() == 3)
    {
//             kDebug() << "prompts line: '" << line << "' is using new format!";
      label = splitLine.at(0).trimmed();
      group = splitLine.at(1);
      prompt = splitLine.at(2).mid(1).trimmed();
    }
    else
    {
//             kDebug() << "prompts line: '" << line << "' is using old format.  attempting old format parsing...";
      group = "default";
      if (line.trimmed().isEmpty()) continue;
      index = line.indexOf ( ' ' );
      label = line.left ( index );
      prompt = line.mid ( index ).trimmed();
    }

    m_samples << label;
    m_wordBySample.insert (label, prompt);
    m_groupBySample.insert(label, group);
  }
  prompts->close();
  prompts->deleteLater();

  updateModel();
  return true;
}

bool PromptsTable::save(const QString& path)
{
  kDebug() << "Opening prompts table at output path: " << path;
  QFile promptsFile ( path );
  if ( !promptsFile.open ( QIODevice::WriteOnly ) ) {
    kDebug() << "Open failed";
    return false;
  }

  foreach (const QString& sample, m_samples)
    promptsFile.write(sample.toUtf8() + " \""
                        + m_groupBySample.value(sample).toUtf8() + "\" "
                        + m_wordBySample.value(sample).toUtf8() + '\n');
  promptsFile.close();

  return true;
}

bool PromptsTable::deletePrompt(QString key)
{
  QString path = SpeechModelManagementConfiguration::modelTrainingsDataPath().toLocalFile()+'/'+key+".wav";

  bool found = true;
  found &= m_groupBySample.contains(key);
  found &= m_wordBySample.contains(key);

  m_groupBySample.remove (key);
  m_wordBySample.remove (key);
  m_samples.removeAll(key);

  updateModel();

  //removes the sample
  QFile::remove(path.toUtf8());

  return found;
}

bool PromptsTable::deleteWord(Word *w)
{
  QString wordToDelete = w->getLexiconWord();

  QStringList sampleFileNames = keys();
  bool succ = true;
  foreach(const QString& sample, sampleFileNames)
  {
    QStringList promptWords = m_wordBySample.value(sample).split(' ');
    if (promptWords.contains(wordToDelete))
    {
      if (!deletePrompt(sample)) succ = false;
    }
  }
  return succ;
}

bool PromptsTable::deleteWord(const QString & word)
{
  Word *w = new Word(word, "", "");
  bool succ = deleteWord(w);
  delete w;
  return succ;
}

void PromptsTable::insert(const QString &sample, const QString &sampleGroup, const QString &word)
{
  m_samples << sample;
  m_wordBySample.insert(sample, word);
  m_groupBySample.insert(sample, sampleGroup);
  updateModel();
}

bool PromptsTable::merge(const PromptsTable& other)
{
  foreach (const QString& sample, other.m_samples)
    if (!m_samples.contains(sample))
      m_samples << sample;
  m_wordBySample.unite(other.m_wordBySample);
  m_groupBySample.unite(other.m_groupBySample);
  updateModel();
  return true;
}

bool PromptsTable::clear()
{
  QStringList paths = keys();

  foreach (const QString& path, paths)
  {
    kDebug() << "Path: " << path;
    deletePrompt(path);
  }

  m_samples.clear();
  m_wordBySample.clear();
  m_groupBySample.clear();
  updateModel();

  return true;
}

bool PromptsTable::contains(const QString &key)
{
  return m_samples.contains(key);
}

int PromptsTable::remove(const QString &key)
{
  m_samples.removeAll(key);
  m_wordBySample.remove(key);
  int removed = m_groupBySample.remove(key);
  updateModel();
  return removed;
}

int PromptsTable::count()
{
  return m_samples.count();
}

QString PromptsTable::value(const QString& key)
{
  return m_wordBySample.value(key);
}

QString PromptsTable::sampleGroup(const QString& key)
{
  return m_groupBySample.value(key);
}
