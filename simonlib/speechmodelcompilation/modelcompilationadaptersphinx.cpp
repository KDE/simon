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
    if(!removeContextAdditions())
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

  if (!(adaptionType & ModelCompilationAdapter::AdaptLanguageModel)) // I do not fully understand the meaning of this code snippet, so I just copied it:(
    return true;

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

  if(!storeDictionary(adaptionType, fetc+DICT_EXT, trainedVocabulary, definedVocabulary,
                      vocabulary))
  {
    emit error(i18n("Failed to store dictionary"));
    return false;
  }

  ADAPT_CHECKPOINT;

  kDebug()<<"Store filler";
  if(!storeFiller(adaptionType, fetc+".filler"))
  {
    emit error(i18n("Failed to store filler"));
    return false;
  }

  ADAPT_CHECKPOINT;

  kDebug()<<"Store phonelist";
  if(!storePhonesList(adaptionType, fetc+PHONE_EXT, vocabulary))
  {
    emit error(i18n("Failed to store phones"));
    return false;
  }

  ADAPT_CHECKPOINT;

  kDebug()<<"Store transcription & fields";
  if(!storeTranscriptionAndFields(adaptionType, promptsPathIn, fetc+TRAIN_TRANSCRIPTION, fetc+TRAIN_FIELDS,
                                  definedVocabulary, vocabulary) || !storeTranscriptionAndFields(adaptionType, promptsPathIn,
                                  fetc+TEST_TRANSCRIPTION, fetc+TEST_FIELDS, definedVocabulary, vocabulary))
  {
    emit error(i18n("Failed to store transcription and fields"));
    return false;
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

bool ModelCompilationAdapterSPHINX::storeDictionary(AdaptionType adaptionType, const QString &dictionaryPathOut, QStringList &trainedVocabulary,
                                                    QStringList &definedVocabulary, QSharedPointer<Vocabulary> vocabulary)
{
  QFile dictionaryFile(dictionaryPathOut);
  if (!dictionaryFile.open(QIODevice::WriteOnly))
  {
    emit error(i18n("Failed to adapt dictionary to \"%1\"", dictionaryPathOut));
    return false;
  }

  QTextStream dictionary(&dictionaryFile);
  dictionary.setCodec("UTF-8");

  m_pronunciationCount = 0;
  QList<Word*> words = vocabulary->getWords();

  QStringList added; //Think about better way to do it
  m_wordCount = 0;
  foreach (Word *word, words)
  {
    if (//(adaptionType & ModelCompilationAdapter::AdaptAcousticModel) &&
            (!(adaptionType == ModelCompilationAdapter::AdaptIndependently) &&
        !trainedVocabulary.contains(word->getLexiconWord())) || added.contains(word->getWord()))
    {
      kDebug() << "Skipping word " << word->getWord();
      continue;
    }

    ++m_pronunciationCount;
    dictionary << word->getWord() << QLatin1String("\t\t") <<
                  word->getPronunciation() << QLatin1String("\n");

    added.append(word->getWord());

    ++m_wordCount;
    if (//(adaptionType & ModelCompilationAdapter::AdaptAcousticModel) &&
        (!definedVocabulary.contains(word->getLexiconWord())))
      definedVocabulary << word->getLexiconWord();
  }

  dictionaryFile.close();

  return true;
}

bool ModelCompilationAdapterSPHINX::storeFiller(AdaptionType adaptionType, const QString &fillerPathOut)
{
  QFile fillerFile(fillerPathOut);
  if (!fillerFile.open(QIODevice::WriteOnly))
  {
    emit error(i18n("Failed to write filler to \"%1\"", fillerPathOut));
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

bool ModelCompilationAdapterSPHINX::storePhonesList(AdaptionType adaptionType, const QString &phonesListPathOut, QSharedPointer<Vocabulary> vocabulary)
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

bool ModelCompilationAdapterSPHINX::storeTranscriptionAndFields(AdaptionType adaptionType, const QString &promptsPathIn, const QString &transcriptionPathOut,
                                                                const QString &fieldsPathOut, QStringList &definedVocabulary, QSharedPointer<Vocabulary> vocabulary)
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
    QStringList words = line.mid(splitter+1).trimmed().split(' ');

    bool first(true);
    QString wordsString;
    foreach (const QString& word, words)
    {
      if (!definedVocabulary.contains(word))
      {
        allWordsInLexicon = false;
//        break;
      }
      if(!first)
        wordsString.append(" ");
      else
        first = false;

      QList<Word*> findWords= vocabulary->findWords(word, Vocabulary::ExactMatch);
      if(!findWords.isEmpty())
        wordsString.append(findWords.first()->getWord());
      else
        wordsString.append(word.toLower());//WARNING: error or not?
    }

    if (allWordsInLexicon || adaptionType == AdaptIndependently)
    {

      promptsOutFile.write("<s> "+wordsString.toUtf8() + " </s> (" + line.left(splitter).toUtf8() /*filename*/+ ")\n");
      fieldsOutFile.write(line.left(splitter).toUtf8() /*filename*/ + "\n");
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

  QTextStream grammarStream(&grammarFile);
  grammarStream.setCodec("UTF-8");

  grammarStream<<"#JSGF V1.0; \n\n"
                 <<"grammar generalGrammar;\n";
  grammarStream<< "public <structure> = ";

  QStringList grammarStructures = grammar->getStructures();
  foreach (const QString& structure, grammarStructures)
  {
    QStringList terminals = Grammar::getTerminalsForStructure(structure);
    if(terminals.isEmpty())
      continue;

//    if(structure != grammarStructures.first())
//    {
//     grammarStream<<"| ";
//    }

    QString buffer;

    foreach (const QString &terminal, terminals)
    {
      bool fword = true;
      QList<Word*> wordsForTerminal = vocabulary->findWordsByTerminal(terminal);

      if(wordsForTerminal.isEmpty())
        continue;



      buffer.append("( ");
      foreach (Word* word, wordsForTerminal)
      {
        if(!definedVocabulary.contains(word->getLexiconWord()) && adaptionType != AdaptIndependently)
          break;

        if(!fword)
          buffer.append(" | ");
        else
          fword = false;

        buffer.append(word->getWord());
      }

      buffer.append(" ) ");
    }

    if(!buffer.isEmpty())
    {
      if(structure != grammarStructures.first())
      {
       grammarStream << "| ";
      }

      grammarStream<< "(" << buffer <<") ";

    }

//    grammarStream<<") ";
//    if(structure != grammarStructures.last())
//    {
//      grammarStream<<"| ";
//    }

  }
  grammarStream<<";\n";
  return true;
}

