/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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
#include <KDebug>

PromptsTable::PromptsTable(QString filePath, QObject *parent) :
    QObject(parent),
    m_filePath(filePath)
{
    init();
}

bool PromptsTable::init()
{
    QFile *prompts = new QFile ( m_filePath );
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
            kDebug() << "prompts line: '" << line << "' is using new format!";
            label = splitLine.at(0).trimmed();
            group = splitLine.at(1);
            prompt = splitLine.at(2).mid(1).trimmed();
        }
        else
        {
            kDebug() << "prompts line: '" << line << "' is using old format.  attempting old format parsing...";
            group = "default";
            if (line.trimmed().isEmpty()) continue;
            index = line.indexOf ( ' ' );
            label = line.left ( index );
            prompt = line.mid ( index ).trimmed();
        }

        m_wordBySample.insert (label, prompt);
        m_groupBySample.insert(label, group);
    }
    prompts->close();
    prompts->deleteLater();

    return true;
}

bool PromptsTable::save()
{
    QFile promptsFile ( m_filePath );
    if ( !promptsFile.open ( QIODevice::WriteOnly ) ) return false;

    QStringList samples = m_wordBySample.keys();

    foreach (QString sample, samples)
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

    //removes the sample
    found &= QFile::remove(path.toUtf8());

    return found;
}

bool PromptsTable::deleteWord(Word *w)
{
    QString wordToDelete = w->getLexiconWord();

    QStringList sampleFileNames = keys();
    bool succ = true;
    foreach(QString sample, sampleFileNames)
    {
        QStringList promptWords = m_wordBySample.value(sample).split(' ');
        if (promptWords.contains(wordToDelete))
        {
            if (!deletePrompt(sample)) succ = false;
        }
    }
    return (save() && succ);
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
    m_wordBySample.insert(sample, word);
    m_groupBySample.insert(sample, sampleGroup);
}

bool PromptsTable::clear()
{
    QStringList paths = keys();

    foreach (const QString& path, paths)
    {
      kDebug() << "Path: " << path;
      deletePrompt(path);
    }

    m_wordBySample.clear();
    m_groupBySample.clear();

    return save();
}

bool PromptsTable::contains(const QString &key)
{
    return keys().contains(key);
}

int PromptsTable::remove(const QString &key)
{
    m_wordBySample.remove(key);
    return m_groupBySample.remove(key);
}

int PromptsTable::count()
{
    return keys().count();
}

QString PromptsTable::value(const QString& key)
{
    return m_wordBySample.value(key);
}

QString PromptsTable::sampleGroup(const QString& key)
{
    return m_groupBySample.value(key);
}

void PromptsTable::setFileName(QString fileName)
{
    m_filePath = fileName;
}
