/*
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


#include "modelcompilationadaptersphinx.h"
#include "simonutils/fileutils.h"

#include<KLocalizedString>
#include <QDir>
#include <QSet>

ModelCompilationAdapterSPHINX::ModelCompilationAdapterSPHINX(const QString &userName, QObject *parent)
  : ModelCompilationAdapter(userName, parent)
{
}

bool ModelCompilationAdapterSPHINX::startAdaption(AdaptionType adaptionType, const QStringList& scenarioPathsIn,
                                                  const QString& promptsIn, const QHash<QString, QString>& args)
{
  abort();

  m_adaptionType = adaptionType;
  m_scenarioPathsIn = scenarioPathsIn;
  m_promptsPathIn = promptsIn;

  m_workingDir = args.value("workingDir");
  m_modelName = args.value("modelName");
  kDebug()<<"Input prompts: " <<promptsIn;
  kDebug()<<"Working directory: " <<m_workingDir;



  if (args.value("stripContext") == "true")
  {
    //remove context additions for prompts file
    if(!removeContextAdditions(adaptionType))
      return false;
  }

  keepGoing=true;

  emit  status(i18n("Adapting model..."), 0, 100);

  kDebug() << "Adapting model";

  QSharedPointer<Vocabulary> mergedVocabulary(new Vocabulary());
  QSharedPointer<Grammar> mergedGrammar(new Grammar());

  kDebug()<<"Scenarios input path: "<<scenarioPathsIn;
  //merging scenarios
  mergeInputData(scenarioPathsIn, mergedVocabulary, mergedGrammar);

  if ((adaptionType & ModelCompilationAdapter::AdaptLanguageModel) &&
      (mergedVocabulary->empty() || (mergedGrammar->structureCount() == 0)))
  {
    kDebug() << "Empty vocabulary or grammar; aborting adaptation";
    emit adaptionAborted(ModelCompilation::InsufficientInput);
    return false;
  }

  ADAPT_CHECKPOINT;

  kDebug() << "ADAPTING model for real";

  if(!storeModel(adaptionType, m_promptsPathIn, m_workingDir, m_modelName, mergedVocabulary, mergedGrammar))
  {
    kDebug()<< "Adaptation failed";
    return false;
  }

  emit  status(i18n("Model adaption complete"), 100, 100);
  emit adaptionComplete();

  kDebug() <<"Adaptation complete";

  return true;
}

bool ModelCompilationAdapterSPHINX::storeModel(AdaptionType adaptionType, const QString& promptsPathIn,
                                               const QString &workingDirPath, const QString &mName,
                                               QSharedPointer<Vocabulary> vocabulary, QSharedPointer<Grammar> grammar)
{
  //    ///// Prompts ///////////
  QStringList trainedVocabulary;                  // words where prompts exist
  QStringList definedVocabulary;                  // words that are in the dictionary

  if(!readPrompts(adaptionType, vocabulary, promptsPathIn, trainedVocabulary))
    return false;

  ADAPT_CHECKPOINT;

  FileUtils::removeDirRecursive(workingDirPath+"/"+mName);

  //Creating a directory hierarchy, where model compilation will be executed
  QDir wDir(workingDirPath);
  if(!wDir.mkdir(mName) || !wDir.cd(mName) || !wDir.mkdir("etc") || !wDir.mkdir("wav"))
  {
    emit error(i18n("Failed to create directory hierarchy at \"%1\"", workingDirPath));
    return false;
  }

  QString fetc = workingDirPath+"/"+mName+"/etc/"+mName;

  kDebug()<<"Store dictionary";

  bool errorStoringDictionary = !storeDictionary(adaptionType, fetc+DICT_EXT, fetc+QLatin1String("_ship")+DICT_EXT,
                                                 trainedVocabulary, definedVocabulary,
                                                 vocabulary);

  if(!purgeUnusedVocabulary(vocabulary, grammar) || errorStoringDictionary)
  {
    emit error(i18n("Failed to store dictionary"));
    return false;
  }

  ADAPT_CHECKPOINT;

  kDebug()<<"Store filler";
  if(!storeFiller(adaptionType, fetc+".filler"))
  {
    emit error(i18nc("Please keep \"filler\" in English as it refers to a file that is usually named that",
                     "Failed to store filler"));
    return false;
  }

  ADAPT_CHECKPOINT;

  kDebug()<<"Store phonelist";
  if(!(adaptionType == ModelCompilationAdapter::AdaptLanguageModel) &&
     !storePhonesList(adaptionType, fetc+PHONE_EXT, vocabulary, trainedVocabulary))
  {
    emit error(i18n("Failed to store phones"));
    return false;
  }

  ADAPT_CHECKPOINT;

  if (adaptionType & ModelCompilationAdapter::AdaptAcousticModel) {
    kDebug()<<"Store transcription & fields";
    bool err = storeTranscriptionAndFields(adaptionType, promptsPathIn,
                                                            fetc+TRAIN_TRANSCRIPTION,
                                                            fetc+TRAIN_FIELDS,
                                                            definedVocabulary);

    err = err && storeTranscriptionAndFields(adaptionType, promptsPathIn,
                                                                fetc+TEST_TRANSCRIPTION,
                                                                fetc+TEST_FIELDS,
                                                                definedVocabulary);
    if (!err) {
        kDebug() << "Returning here; store transcription and fields failed...?";
        return false; // error reporting done by the function itself
    }
  }

  ADAPT_CHECKPOINT;

  kDebug()<<"Store grammar";
  if(!storeGrammar(adaptionType, fetc+GRAMMAR_EXT, vocabulary, definedVocabulary, grammar))
  {
    emit error(i18n("Failed to store grammar"));
    return false;
  }

  return true;
}

bool ModelCompilationAdapterSPHINX::purgeUnusedVocabulary(QSharedPointer<Vocabulary> vocabulary, QSharedPointer<Grammar> grammar)
{
  QSet<QString> categories;
  foreach (const QString& structure, grammar->getStructures())
    foreach (const QString& category, structure.split(" ", QString::SkipEmptyParts))
      categories.insert(category);
  QList<Word*> words = vocabulary->getWords();
  foreach (Word* w, words)
    if (!categories.contains(w->getCategory()))
      vocabulary->removeWord(w);
  return true;
}

bool ModelCompilationAdapterSPHINX::storeDictionary(AdaptionType adaptionType, const QString &dictionaryPathTrainOut, const QString &dictionaryPathOut, QStringList &trainedVocabulary,
                                                    QStringList &definedVocabulary, QSharedPointer<Vocabulary> vocabulary)
{
  QFile dictionaryFile(dictionaryPathOut);
  QFile dictionaryTrainFile(dictionaryPathTrainOut);
  if (!dictionaryFile.open(QIODevice::WriteOnly) || !dictionaryTrainFile.open(QIODevice::WriteOnly) )
  {
    //FIXME: After string freeze, add dictioanryPathTrainOut
    emit error(i18n("Failed to adapt dictionary to \"%1\"", dictionaryPathOut));
    return false;
  }

  QTextStream dictionary(&dictionaryFile);
  QTextStream dictionaryTrain(&dictionaryTrainFile);
  dictionary.setCodec("UTF-8");
  dictionaryTrain.setCodec("UTF-8");

  m_pronunciationCount = 0;
  QList<Word*> words = vocabulary->getWords();

  m_wordCount = 0;
  QHash<QString, int> wordCounts;
  QHash<QString, int> wordCountsShip;
  foreach (Word *word, words)
  {
    if ((adaptionType & ModelCompilationAdapter::AdaptAcousticModel) &&
            (!(adaptionType == ModelCompilationAdapter::AdaptIndependently) &&
        (!trainedVocabulary.contains(word->getLexiconWord()))))
    {
      kDebug() << "Skipping word " << word->getWord();
      continue;
    }

    ++m_pronunciationCount;

    QString wordName = word->getWord();
    QString wordNameShip = word->getLexiconWord();
    if (wordCounts.contains(wordName)) {
      int currentCount = wordCounts.value(wordName) + 1;
      wordCounts.insert(wordName, currentCount);
      wordName += '(' + QByteArray::number(currentCount) + ')';
    } else
      wordCounts.insert(wordName, 1);
    if (wordCountsShip.contains(wordNameShip)) {
      int currentCount = wordCountsShip.value(wordNameShip) + 1;
      wordCountsShip.insert(wordNameShip, currentCount);
      wordNameShip += '(' + QByteArray::number(currentCount) + ')';
    } else
      wordCountsShip.insert(wordNameShip, 1);
    dictionary << wordName << QLatin1String("\t\t") <<
                  word->getPronunciation() << QLatin1String("\n");
    dictionaryTrain << wordNameShip << QLatin1String("\t\t") <<
                  word->getPronunciation() << QLatin1String("\n");

    ++m_wordCount;
    if (//(adaptionType == ModelCompilationAdapter::AdaptAcousticModel) &&
        (!definedVocabulary.contains(word->getLexiconWord())))
      definedVocabulary << word->getLexiconWord();
  }

  dictionaryFile.close();

  return true;
}

bool ModelCompilationAdapterSPHINX::storeFiller(AdaptionType adaptionType, const QString &fillerPathOut)
{
  Q_UNUSED(adaptionType);
  QFile fillerFile(fillerPathOut);
  if (!fillerFile.open(QIODevice::WriteOnly))
  {
    emit error(i18nc("Please keep \"filler\" in English as it refers to a file that is usually named that",
                     "Failed to write filler to \"%1\"", fillerPathOut));
    return false;
  }

  QTextStream filler(&fillerFile);
  filler.setCodec("UTF-8");

  filler << "<s> \t\t SIL\n"
         << "</s> \t\t SIL\n"
         << "<sil> \t\t SIL";

  fillerFile.close();

  return true;
}

bool ModelCompilationAdapterSPHINX::storePhonesList(AdaptionType adaptionType, const QString &phonesListPathOut,
                                                    QSharedPointer<Vocabulary> vocabulary,
                                                    const QStringList &trainedVocabulary)
{
  QFile phoneFile(phonesListPathOut);
  if (!phoneFile.open(QIODevice::WriteOnly))
  {
    emit error(i18n("Failed to store phones list to \"%1\"", phonesListPathOut));
    return false;
  }

  QTextStream phone(&phoneFile);
  phone.setCodec("UTF-8");

  QSet<QString> uniquePhonemes;
  foreach (Word *word, vocabulary->getWords())
  {
    if((adaptionType & ModelCompilationAdapter::AdaptAcousticModel) &&
       (adaptionType != AdaptIndependently) && !trainedVocabulary.contains(word->getLexiconWord()))
    {
      kDebug() << "Skipping phones for word " << word->getWord();
      continue;
    }

    QStringList phoneList = word->getPronunciation().split(" ");
    foreach (const QString &tphone, phoneList)
    {
      uniquePhonemes.insert(tphone);
    }
  }

  foreach (const QString &wphone, uniquePhonemes)
  {
    phone<<wphone<<"\n";
  }

  phone<<"SIL\n";

  phoneFile.close();

  return true;
}

bool ModelCompilationAdapterSPHINX::storeTranscriptionAndFields(AdaptionType adaptionType,
                                                                const QString &promptsPathIn,
                                                                const QString &transcriptionPathOut,
                                                                const QString &fieldsPathOut,
                                                                QStringList &definedVocabulary)
{
  QFile promptsInFile(promptsPathIn);
  QFile promptsOutFile(transcriptionPathOut);
  QFile fieldsOutFile(fieldsPathOut);
  if (!promptsInFile.open(QIODevice::ReadOnly) || !promptsOutFile.open(QIODevice::WriteOnly) ||
      !fieldsOutFile.open(QIODevice::WriteOnly))
  {
    emit error(i18n("Failed to store transcription to \"%1\" from \"%2\" or write fields to \"%3\"", transcriptionPathOut, promptsPathIn, fieldsPathOut));
    return false;
  }

  m_sampleCount = 0;

  while (!promptsInFile.atEnd())
  {
    QString line = QString::fromUtf8(promptsInFile.readLine());
    int splitter = line.indexOf(" ");
    bool allWordsInLexicon = true;
    QString sentence = line.mid(splitter+1).trimmed();
    QStringList words = sentence.split(' ');

    foreach (const QString& word, words)
    {
      if (!definedVocabulary.contains(word))
      {
        allWordsInLexicon = false;
        break;
      }
    }

    if (allWordsInLexicon || !(adaptionType & AdaptLanguageModel) || (adaptionType & AdaptIndependently))
    {
      QByteArray filename;
#ifdef Q_OS_WIN
      filename = line.left(splitter).toLocal8Bit();
#else
      filename = line.left(splitter).toUtf8();
#endif

      promptsOutFile.write("<s> "+ sentence.toUtf8() + " </s> (" + filename + ")\n");
      fieldsOutFile.write(filename + "\n");
      ++m_sampleCount;
    }
  }

  promptsInFile.close();
  promptsOutFile.close();
  fieldsOutFile.close();

  return true;
}

bool ModelCompilationAdapterSPHINX::storeGrammar(ModelCompilationAdapter::AdaptionType adaptionType,
                                                 const QString &grammarPathOut, QSharedPointer<Vocabulary> vocabulary,
                                                 QStringList &definedVocabulary, QSharedPointer<Grammar> grammar)
{
  QFile grammarFile(grammarPathOut);
  if (!grammarFile.open(QIODevice::WriteOnly))
  {
    emit error(i18n("Failed to store grammar to \"%1\"", grammarPathOut));
    return false;
  }

  QStringList grammarStructures = grammar->getStructures();

  if (grammarStructures.isEmpty())
  {
    emit adaptionAborted(ModelCompilation::InsufficientInput); //no grammar
    return false;
  }

  QTextStream grammarStream(&grammarFile);
  grammarStream.setCodec("UTF-8");

  grammarStream<<"#JSGF V1.0; \n\n"
                 <<"grammar generalGrammar;\n";
  grammarStream<< "public <structure> = ";

  kDebug()<<"Structures count:"<< grammarStructures.size();

  foreach (const QString& structure, grammarStructures)
  {
    QStringList categories = Grammar::getCategoriesForStructure(structure);
    if(categories.isEmpty())
      continue;

    QString gramBuffer;

    kDebug()<<"Categories count for structure "<<structure<<": "<<categories.size();
    foreach (const QString &category, categories)
    {
      bool fword = true;
      QList<Word*> wordsForCategory = vocabulary->findWordsByCategory(category);

      kDebug()<<"Words for category "<<category<<":"<<wordsForCategory.size();

      if(wordsForCategory.isEmpty())
        continue;

      QString termBuffer;

      foreach (Word* word, wordsForCategory)
      {
        if(!definedVocabulary.contains(word->getLexiconWord()) && adaptionType != AdaptIndependently)//WARNING: talk about adapt independently becouse of magic
          continue;

        if(!fword)
          termBuffer.append(" | ");
        else
          fword = false;

        termBuffer.append(word->getWord());
      }

      if(!termBuffer.isEmpty())
      {
        gramBuffer.append("( ");
        gramBuffer.append(termBuffer);
        gramBuffer.append(" ) ");
      }
    }

    if(!gramBuffer.isEmpty() && !(gramBuffer == "(  ) "))
    {
      if(structure != grammarStructures.first())
      {
       grammarStream << "| ";
      }

      grammarStream<< "(" << gramBuffer <<") ";

    }
  }
  grammarStream<<";\n";
  return true;
}

