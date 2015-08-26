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

#include "modelcompilationadapterhtk.h"

#include <simonlogging/logger.h>

#include <simonscenarios/scenario.h>
#include <simonscenarios/vocabulary.h>
#include <simonscenarios/activevocabulary.h>
#include <simonscenarios/grammar.h>

#include <QFile>
#include <QDebug>


ModelCompilationAdapterHTK::ModelCompilationAdapterHTK(const QString& userName, QObject *parent) : ModelCompilationAdapter(userName, parent)
{
}

bool ModelCompilationAdapterHTK::startAdaption(AdaptionType adaptionType, const QStringList& scenarioPathsIn,
                                               const QString& promptsIn, const QHash<QString, QString>& args)
{
  abort();

  keepGoing=true;

  m_adaptionType = adaptionType;
  m_scenarioPathsIn = scenarioPathsIn;
  m_promptsPathIn = promptsIn;

  m_lexiconPathOut = args.value("lexicon");
  m_grammarPathOut = args.value("grammar");
  m_simpleVocabPathOut = args.value("simpleVocab");
  m_promptsPathOut = args.value("prompts");
  m_baseTiedListPathIn = args.value("base/tiedlist");

  m_droppedTranscriptions.clear();

  if (args.value("stripContext") == "true")
  {
    //remove context additions for prompts file
    if(!removeContextAdditions(adaptionType))
      return false;
  }

  emit  status(i18n("Adapting model..."), 0, 100);

  if (!adaptModel(m_adaptionType, m_scenarioPathsIn, m_promptsPathIn, m_baseTiedListPathIn,
                  m_lexiconPathOut, m_grammarPathOut, m_simpleVocabPathOut, m_promptsPathOut))
  {
    return false;
  }

  emit  status(i18n("Model adaption complete"), 100, 100);
  emit adaptionComplete();

  qDebug() <<"Adaptation complete";

  keepGoing=false;

  return true;
}


bool ModelCompilationAdapterHTK::checkTriphones(const QString& baseTiedListPathIn, QSharedPointer<Vocabulary> vocabulary)
{
  QFile baseTiedList(baseTiedListPathIn);
  if (!baseTiedList.open(QIODevice::ReadOnly))
    return true;

  QSet<QByteArray> allowedTriphones;

  //parse tiedlist and find triphones
  //int minusIndex;
  //int plusIndex;
  //minusIndex = plusIndex = -1;
  int i;

  while (!baseTiedList.atEnd()) {
    QByteArray triphone = baseTiedList.readLine().trimmed();
    if (triphone.contains(' ')) {
      // only one space
      int separator = triphone.indexOf(' ');
      allowedTriphones << triphone.mid(0, separator);
      allowedTriphones << triphone.mid(separator+1);
    } else
      allowedTriphones << triphone;
  }
  QList<Word*> words = vocabulary->getWords();
  foreach (Word *w, words) {
    QByteArray pronunciation = w->getPronunciation().toAscii();
    int start = 0;
    int nextStart = -1;
    int afterNextStart = -1;
    bool broken = false;
    for (i = 0; i < pronunciation.count(); i++) {
      if (pronunciation[i] == ' ') {
        if (afterNextStart != -1) {
          // check X-Y+Z
          QByteArray triphone = pronunciation.mid(start, i - start);
          triphone[nextStart - start - 1] = '-';
          triphone[afterNextStart - start - 1] = '+';
          if (!supportedTranscription(allowedTriphones, triphone))
            break;

          start = nextStart;
          nextStart = afterNextStart;
          afterNextStart = i + 1;
        } else if (nextStart == -1) {
          nextStart = i + 1;
        } else if (afterNextStart == -1) {
          afterNextStart = i + 1;

          // check *-X+Y
          QByteArray triphone = pronunciation.mid(start, i - start);
          triphone[nextStart - 1] = '+';
          if (!supportedTranscription(allowedTriphones, triphone))
            break;
        }
      }
    }
    broken = (i != pronunciation.count());
    if (!broken) {
      if (nextStart == -1) {
        // single phoneme pronunciation
        broken = !supportedTranscription(allowedTriphones, pronunciation);
      } else if (afterNextStart == -1) {
        // biphone pronunciation
        pronunciation[nextStart - 1] = '-';
        broken = !supportedTranscription(allowedTriphones, pronunciation);
        pronunciation[nextStart - 1] = '+';
        broken |= !supportedTranscription(allowedTriphones, pronunciation);
      } else {
        // full triphone left over
        QByteArray triphone = pronunciation.mid(start);
        triphone[nextStart - start - 1] = '-';
        triphone[afterNextStart - start - 1] = '+';
        broken = !supportedTranscription(allowedTriphones, triphone);

        triphone = pronunciation.mid(nextStart);
        triphone[afterNextStart - nextStart - 1] = '-';
        broken |= !supportedTranscription(allowedTriphones, triphone);
      }
    }

    if (broken) {
      // forbid word
      qDebug() << "Forbidding word " << w->getWord();
      m_droppedTranscriptions << w->getPronunciation();
      vocabulary->removeWord(w);
    }
  }
  return true;
}

inline bool ModelCompilationAdapterHTK::supportedTranscription(const QSet<QByteArray>& allowedTriphones, const QByteArray& triphone)
{
  bool supported = allowedTriphones.contains(triphone);

  if (!supported)
    qDebug() << "Not allowed: " << triphone;
  //else
    //qDebug() << "Allowed: " << triphone;
  return supported;
}
bool ModelCompilationAdapterHTK::adaptModel(ModelCompilationAdapter::AdaptionType adaptionType,
                                            const QStringList& scenarioPaths, const QString& promptsPathIn,
					    const QString& baseTiedListPathIn,
                                            const QString& lexiconPathOut, const QString& grammarPathOut,
                                            const QString& simpleVocabPathOut, const QString& promptsPathOut)
{
  qDebug() << "Adapting model";
  QSharedPointer<Vocabulary> mergedVocabulary(new Vocabulary());
  QSharedPointer<Grammar> mergedGrammar(new Grammar());

  //merging scenarios
  mergeInputData(scenarioPaths, mergedVocabulary, mergedGrammar);

  ADAPT_CHECKPOINT;
  if (!checkTriphones(baseTiedListPathIn, mergedVocabulary)) {
    qWarning() << "Triphone optimization failed";
  }

  if (!storeModel(adaptionType, lexiconPathOut, simpleVocabPathOut, grammarPathOut,
                  promptsPathOut, mergedVocabulary, mergedGrammar, promptsPathIn))
  {
    qWarning() << "Adaption failed";
    return false;
  }

  return true;
}


QString ModelCompilationAdapterHTK::htkify(const QString& in)
{
  return QString::fromUtf8(htkify(in.toUtf8()));
}


QByteArray ModelCompilationAdapterHTK::htkify(const QByteArray& in)
{
  QByteArray out = in;
  out.replace('0', "0ZERO");
  out.replace('1', "1ONE");
  out.replace('2', "2TWO");
  out.replace('3', "3THREE");
  out.replace('4', "4FOUR");
  out.replace('5', "5FIVE");
  out.replace('6', "6SIX");
  out.replace('7', "7SEVEN");
  out.replace('8', "8EIGHT");
  out.replace('9', "9NINE");
  return out;
}

bool ModelCompilationAdapterHTK::storeLexicon(ModelCompilationAdapter::AdaptionType adaptionType,
                                              const QString &lexiconPathOut,
                                              QSharedPointer<Vocabulary> vocabulary,
                                              QStringList &trainedVocabulary, QStringList &definedVocabulary)
{
  /////  Lexicon  ////////////////

  qDebug() << "Store lexicon";

  emit status(i18n("Adapting lexicon..."), 15, 100);
  QFile lexiconFile(lexiconPathOut);
  if (!lexiconFile.open(QIODevice::WriteOnly))
  {
    emit error(i18n("Failed to adapt lexicon to \"%1\"", lexiconPathOut));
    return false;
  }
  ADAPT_CHECKPOINT;

  QTextStream lexicon(&lexiconFile);
  lexicon.setCodec("UTF-8");

  bool sentWritten = false;
  m_pronunciationCount = 0;
  QList<Word*> words = vocabulary->getWords();
  QString htkIfiedWord;
  m_wordCount = 0;
  foreach (Word *word, words)
  {
    if ((adaptionType & ModelCompilationAdapter::AdaptAcousticModel) &&
        !(adaptionType & ModelCompilationAdapter::AdaptIndependently) &&
        !trainedVocabulary.contains(word->getLexiconWord()))
    {
      qDebug() << "Skipping word " << word->getLexiconWord();
      continue;
    }
    htkIfiedWord = htkify(word->getLexiconWord());

    if (!sentWritten && (htkIfiedWord >= QLatin1String("SENT-END")))
    {
      lexicon << "SENT-END\t\t[]\t\tsil\n";
      lexicon << "SENT-START\t\t[]\t\tsil\n";
      sentWritten=true;
    }
    ++m_pronunciationCount;
    lexicon << htkIfiedWord << QLatin1String("\t\t[") << word->getWord() << QLatin1String("]\t\t") <<
               word->getPronunciation() << QLatin1String("\n");

    ++m_wordCount;
    if ((adaptionType & ModelCompilationAdapter::AdaptAcousticModel) &&
        (!definedVocabulary.contains(htkIfiedWord)))
      definedVocabulary << htkIfiedWord;
  }
  if (!sentWritten) {
    lexicon << "SENT-END\t\t[]\t\tsil\n";
    lexicon << "SENT-START\t\t[]\t\tsil\n";
  }
  lexiconFile.close();

  ADAPT_CHECKPOINT;

  return true;
}

bool ModelCompilationAdapterHTK::storeVocabulary(ModelCompilationAdapter::AdaptionType adaptionType,
                                                 const QString& simpleVocabPathOut, QSharedPointer<Vocabulary> vocabulary,
                                                 QSharedPointer<Grammar> grammar, QStringList &trainedVocabulary,
                                                 QStringList &structures)
{
  /////  Vocabulary  /////////////
  qDebug() << "Store vocabulary";
  emit status(i18n("Adapting vocabulary..."), 35, 100);

  // find out which words are referenced by training data
  // find out which categories are referenced by grammar
  QFile simpleVocabFile(simpleVocabPathOut);

  if (!simpleVocabFile.open(QIODevice::WriteOnly))
  {
    emit error(i18n("Failed to adapt lexicon to \"%1\"", simpleVocabPathOut));
    return false;
  }

  QTextStream vocabStream(&simpleVocabFile);
  vocabStream.setCodec("UTF-8");

  //print internal sentence structure
  vocabStream << "% NS_B\n<s>\tsil\n";
  vocabStream << "% NS_E\n</s>\tsil\n";

  QStringList grammarCategories = grammar->getCategories();

  bool everythingChanged = true;

  while (everythingChanged)
  {
    everythingChanged = false;
    foreach (const QString& category, grammarCategories)
    {
      //if there are no words for this category, remove it from the list
      QList<Word*> wordsForCategory = vocabulary->findWordsByCategory(category);

      bool hasAssociatedWord = false;
      foreach (Word *word, wordsForCategory)
      {
        if ((!(adaptionType & ModelCompilationAdapter::AdaptAcousticModel)) ||
            trainedVocabulary.contains(word->getLexiconWord()))
          hasAssociatedWord = true;
        else vocabulary->removeWord(word, true /* delete word*/);
      }
      if ((adaptionType & ModelCompilationAdapter::AdaptAcousticModel)
          && !(adaptionType & ModelCompilationAdapter::AdaptIndependently)
          && !hasAssociatedWord) {
        grammarCategories.removeAll(category);

        for (int i=0; i < structures.count(); i++) {
          if (structures[i].contains(QRegExp("\\b"+category+"\\b")))
          {
            //             This appears to be a bit arbitrary and shouldn't technically be necessary. Lets
            //             disable it for now, keep it around for a while and look for bugs.
            //
            //             //delete all words of all categories in this structure
            //             QStringList structureElements = structures[i].split(' ');
            //             foreach (const QString& structureCategory, structureElements) {
            //               QList<Word*> wordsToDelete = vocab->findWordsByCategory(structureCategory);
            //               foreach (Word *w, wordsToDelete)
            //                 vocab->removeWord(w, true);
            //             }
            grammar->deleteStructure(i--);
            structures = grammar->getStructures();
          }
        }
        everythingChanged = true;
        break;
      }
    }
  }
  if (vocabulary->getWords().isEmpty()) {
    emit adaptionAborted(ModelCompilation::InsufficientInput); //no vocabulary
    return false;
  }

  ADAPT_CHECKPOINT;

  foreach (const QString& category, grammarCategories)
  {
    //only store vocabulary that is referenced by the grammar
    QList<Word*> wordsForCategory = vocabulary->findWordsByCategory(category);

    vocabStream << "% " << category << "\n";
    foreach (const Word *w, wordsForCategory)
      vocabStream << w->getWord() << "\t" << w->getPronunciation() << "\n";
  }
  simpleVocabFile.close();

  ADAPT_CHECKPOINT;

  return true;
}

bool ModelCompilationAdapterHTK::storeGrammar(const QString& grammarPathOut, QStringList &structures)
{
  emit status(i18n("Adapting grammar..."), 75, 100);
  qDebug() << "Store grammar";

  QFile grammarFile(grammarPathOut);
  if (!grammarFile.open(QIODevice::WriteOnly))
  {
    emit error(i18n("Failed to adapt grammar to \"%1\"", grammarPathOut));
    return false;
  }

  if (structures.isEmpty()) {
    emit adaptionAborted(ModelCompilation::InsufficientInput); //no grammar
    return false;
  }

  foreach (const QString& structure, structures)
    grammarFile.write("S:NS_B "+structure.toUtf8()+" NS_E\n");
  grammarFile.close();

  ADAPT_CHECKPOINT;

  return true;
}

bool ModelCompilationAdapterHTK::storePrompts(ModelCompilationAdapter::AdaptionType adaptionType,
                                              const QString& promptsPathOut, const QString& promptsPathIn,
                                              QStringList &definedVocabulary) //wtf?
{
  emit status(i18n("Adapting prompts..."), 90, 100);
  qDebug() << "Store prompts";
  if (adaptionType & ModelCompilationAdapter::AdaptAcousticModel)
  {
    QFile promptsFile(promptsPathIn);
    QFile promptsFileOut(promptsPathOut);
    m_sampleCount = 0;

    if (!promptsFile.open(QIODevice::ReadOnly) || !promptsFileOut.open(QIODevice::WriteOnly))
    {
      emit error(i18n("Failed to adapt prompts from \"%1\" to \"%2\"", promptsPathIn, promptsPathOut));
      return false;
    }

    while (!promptsFile.atEnd())
    {
      QString line = QString::fromUtf8(promptsFile.readLine());
      int splitter = line.indexOf(" ");
      bool allWordsInLexicon = true;
      QStringList words = line.mid(splitter+1).trimmed().split(' ');

      if ((adaptionType & ModelCompilationAdapter::AdaptLanguageModel) && !(adaptionType & ModelCompilationAdapter::AdaptIndependently)) {
        foreach (const QString& word, words)
        {
          if (!definedVocabulary.contains(word))
          {
            allWordsInLexicon = false;
            break;
          }
        }
      }
      if (allWordsInLexicon)
      {
        promptsFileOut.write(line.left(splitter).toUtf8() /*filename*/ + ' ' + htkify(words.join(" ").toUtf8()) + '\n');
        ++m_sampleCount;
      }
    }

    promptsFile.close();
    promptsFileOut.close();
  }
  return true;
}

bool ModelCompilationAdapterHTK::storeModel(ModelCompilationAdapter::AdaptionType adaptionType,
                                            const QString& lexiconPathOut, const QString& simpleVocabPathOut, const QString& grammarPathOut,
                                            const QString& promptsPathOut, QSharedPointer<Vocabulary> vocabulary, QSharedPointer<Grammar> grammar, const QString& promptsPathIn)
{
  qDebug() << "Output prompts: " << promptsPathOut;
  qDebug() << "Input prompts: " << promptsPathIn;

  //    ///// Prompts ///////////
  QStringList trainedVocabulary;                  // words where prompts exist
  QStringList definedVocabulary;                  // words that are in the dictionary

  if(!readPrompts(adaptionType, vocabulary, promptsPathIn, trainedVocabulary))
    return false;

  if (adaptionType & ModelCompilationAdapter::AdaptLanguageModel) {
    /////  Lexicon  ////////////////

    if(!storeLexicon(adaptionType, lexiconPathOut, vocabulary, trainedVocabulary, definedVocabulary))
      return false;

    //    /////  Vocabulary  /////////////
    QStringList structures = grammar->getStructures();

    if(!storeVocabulary(adaptionType, simpleVocabPathOut, vocabulary, grammar, trainedVocabulary, structures))
      return false;

    ///// Grammar ///////////
    if(!storeGrammar(grammarPathOut, structures))
      return false;
  }

  ///// Prompts (2) //////
  if(!storePrompts(adaptionType, promptsPathOut, promptsPathIn, definedVocabulary))
    return false;

  return true;
}

ModelCompilationAdapterHTK::~ModelCompilationAdapterHTK()
{
}
