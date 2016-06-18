/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#include "modelcompilationadapter.h"
#include <simonscenarios/scenario.h>
#include <simonscenarios/vocabulary.h>
#include <simonscenarios/activevocabulary.h>
#include <simonscenarios/grammar.h>

#include <KAboutData>
#include <KLocale>
#include <QDebug>
#include <KGlobal>
#include <QString>
#include <QFile>
#include <QDir>



ModelCompilationAdapter::ModelCompilationAdapter(const QString& userName, QObject *parent) : QObject(parent), keepGoing(false), m_userName(userName)
{
}

void ModelCompilationAdapter::abort()
{
  keepGoing = false;
}

bool ModelCompilationAdapter::removeContextAdditions(ModelCompilationAdapter::AdaptionType adaptionType)
{
  if (!(adaptionType & AdaptAcousticModel))
    return true;

  QString realInPrompts = m_promptsPathIn;
  m_promptsPathIn = QDir::tempPath() + QLatin1Char('/') + KAboutData::applicationData().productName()+'/'+m_userName+"/compile/tmpprompts";
  QFile newPrompts(m_promptsPathIn);
  QFile oldPrompts(realInPrompts);
  if (!newPrompts.open(QIODevice::WriteOnly) || !oldPrompts.open(QIODevice::ReadOnly)) {
    emit error(i18n("Could not strip context of prompts"));
    return false;
  }
  while (!oldPrompts.atEnd()) {
    QByteArray line = oldPrompts.readLine();
    int firstIndex = line.indexOf('"');
    int nextIndex = line.indexOf('"', firstIndex+1);
    if ((firstIndex != -1) && (nextIndex != -1))
      line.remove(firstIndex, nextIndex - firstIndex + 2);
    newPrompts.write(line);
  }
  return true;
}

void ModelCompilationAdapter::mergeInputData(const QStringList &scenarioPaths, QSharedPointer<Vocabulary> mergedVocabulary, QSharedPointer<Grammar> mergedGrammar)
{
  //merging scenarios
  foreach (const QString& src, scenarioPaths)
  {
    qDebug() << "Serializing Scenario: " << src;
    QSharedPointer<Scenario> scenario (new Scenario(""));
    if (!scenario->readLanguageModel(src))
    {
      qDebug() << "Could not parse language model at " << src;
      continue;
    }

    Grammar *grammar = scenario->grammar();
    mergedGrammar->addStructures(grammar->getStructures(), false /* do not save */);

    Vocabulary *vocab = scenario->vocabulary();

    QList<Word*> words = vocab->getWords();
    vocab->clear();                               // make sure they are not removed from the scenario when we delete that
    QList<Word*> wordsTmp;
    foreach (Word* w, words)
      wordsTmp.append(w);

    //list will be deleted by addWords
    mergedVocabulary->addWords(wordsTmp);
  }
}

bool ModelCompilationAdapter::containsPoisonedPhoneme(const QString& pronunciation)
{
  if (m_poisonedPhonemes.isEmpty()) return false;

  QStringList phonemes = pronunciation.split(' ');
  foreach (const QString& phoneme, phonemes)
    if (m_poisonedPhonemes.contains(phoneme))
      return true;

  return false;
}

void ModelCompilationAdapter::removeWordsWithPoisonedPhonems(QSharedPointer<Vocabulary> vocabulary)
{
  QList<Word*> words = vocabulary->getWords();
  foreach (Word *word, words)
  {
    if (containsPoisonedPhoneme(word->getPronunciation()))
    {
      qDebug() << "Removing word containing poisoned phoneme: " << word->getWord();
      m_droppedTranscriptions << word->getPronunciation();
      vocabulary->removeWord(word);
    }
  }
}


bool ModelCompilationAdapter::readPrompts(ModelCompilationAdapter::AdaptionType adaptionType,
                                          QSharedPointer<Vocabulary> vocabulary, const QString &promptsPathIn,
                                          QStringList &trainedVocabulary)
{
  qDebug() << "Reading prompts from " <<promptsPathIn;
  ///// Prompts ///////////

  if (!m_poisonedPhonemes.isEmpty() && (adaptionType & ModelCompilationAdapter::AdaptLanguageModel))
  {
    removeWordsWithPoisonedPhonems(vocabulary);
  }

  ADAPT_CHECKPOINT;

  if (adaptionType & ModelCompilationAdapter::AdaptAcousticModel)
  {
    emit status(i18n("Adapting prompts..."), 1, 100);
    QFile promptsFile(promptsPathIn);

    if (!promptsFile.open(QIODevice::ReadOnly))
    {
      if (QFile::exists(promptsPathIn))
        emit error(i18nc("%1 is source file path", "Could not adapt prompts. Does the file \"%1\" exist?", promptsPathIn));
      else
      {
        qDebug() << "Aborting because we have no input prompts";
        emit adaptionAborted(ModelCompilation::InsufficientInput); //no input prompts
      }
      return false;
    }

    while (!promptsFile.atEnd())
    {
      QString line = QString::fromUtf8(promptsFile.readLine());
      int splitter = line.indexOf(" ");
      QStringList words;
      words = line.mid(splitter+1).trimmed().split(' ');

      foreach (const QString& word, words)
      {
        if (!vocabulary->containsWord(word))
        {
          qDebug() << "Word not defined in vocabulary: " << word;
          //allWordsDefined = false;
          break;
        }
        if (!trainedVocabulary.contains(word))
          trainedVocabulary.append(word);
      }
    }

    promptsFile.close();
  }
  ADAPT_CHECKPOINT;

  return true;
}
