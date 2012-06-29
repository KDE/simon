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
#include<KLocalizedString>
#include <QDir>
#include "simonutils/fileutils.h"

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

  if (!(adaptionType & ModelCompilationAdapter::AdaptLanguageModel)) // I do not fully understand the meaning of this code snippet, so I just copied it:(
    return true;

  ADAPT_CHECKPOINT;

//  FileUtils::removeDirRecursive(workingDirPath+"/"+mName);
//  FIXME: resolve linking problem and use function from simonutils module
  removeDirRecursive(workingDirPath+"/"+mName);

  //Creating a directory hierarchy, where model compilation will be executed
  QDir wDir(workingDirPath);
  if(!wDir.mkdir(mName) || !wDir.cd(mName) || !wDir.mkdir("etc") || !wDir.mkdir("wav"))
  {
    emit error(i18n("Failed to create directory hierarchy at \"%1\"", workingDirPath));
    return false;
  }

  QString fetc = workingDirPath+"/"+mName+"/etc/"+mName;

  if(!storeDictionary(adaptionType, fetc+".dic", trainedVocabulary, definedVocabulary,
                      vocabulary))
  {
    emit error(i18n("Failed to store dictionary"));
    return false;
  }

  if(!storeFiller(adaptionType, fetc+".filler"))
  {
    emit error(i18n("Failed to store filler"));
    return false;
  }

  if(!storePhonesList(adaptionType, fetc+".phone", vocabulary))
  {
    emit error(i18n("Failed to store phones"));
    return false;
  }

  if(!storeTranscriptionAndFields(adaptionType, promptsPathIn, fetc+"_train.transcription", fetc+"_train.fileids",
                                  definedVocabulary))
  {
    emit error(i18n("Failed to store transcription and fields"));
    return false;
  }

  ADAPT_CHECKPOINT;

  if(!storeGrammar(adaptionType, fetc+".jsjf", vocabulary, definedVocabulary, grammar))
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
  m_wordCount = 0;
  for(Word *word: words)
  {
    if (//(adaptionType & ModelCompilationAdapter::AdaptAcousticModel) &&
        //    !(adaptionType & ModelCompilationAdapter::AdaptIndependently) &&    //???
        !trainedVocabulary.contains(word->getLexiconWord()))
    {
      kDebug() << "Skipping word " << word->getWord();
      continue;
    }

    ++m_pronunciationCount;
    dictionary << word->getLexiconWord() << QLatin1String("\t\t") <<
                  word->getPronunciation() << QLatin1String("\n");

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
  //WARNING: Hardcode or not hardcode? this is a question.

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
  for(Word *word: vocabulary->getWords())
  {
    auto phoneList = word->getPronunciation().split(" ");
    for(const QString &tphone:phoneList)
    {
      uniquePhonemes.insert(tphone);
    }
  }

  for(const QString &wphone:uniquePhonemes)
  {
    phone<<wphone<<"\n";
  }

  phoneFile.close();

  return true;
}

bool ModelCompilationAdapterSPHINX::storeTranscriptionAndFields(AdaptionType adaptionType, const QString &promptsPathIn, const QString &transcriptionPathOut,
                                                                const QString &fieldsPathOut, QStringList &definedVocabulary)
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

    for(const QString& word: words)
    {
      if (!definedVocabulary.contains(word))
      {
        allWordsInLexicon = false;
        break;
      }
    }

    if (allWordsInLexicon)
    {
      promptsOutFile.write("<s> "+words.join(" ").toUtf8() + " </s> (" + line.left(splitter).toUtf8() /*filename*/+ ")\n");
      fieldsOutFile.write(line.left(splitter).toUtf8() /*filename*/ + ".wav\n"); //WARNING: is wav hardcode ?
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

  QStringList grammarStructures = grammar->getStructures();
//WARNING: is all ok there?:D
  int index(0);
  for(const QString& structure: grammarStructures)
  {
    grammarStream<< "public <structure"+ QString::number(index++) +"> = ";
    int splitter = structure.indexOf(" ");
    QStringList terminals = structure.mid(splitter+1).trimmed().split(' ');


    for(const QString &terminal: terminals)
    {
      grammarStream<<"( ";

      bool fword = true;
      QList<Word*> wordsForTerminal = vocabulary->findWordsByTerminal(terminal);
      for(Word* word: wordsForTerminal)
      {
        if(!definedVocabulary.contains(word->getLexiconWord()))
          break; //WARNING: Depends on adaptation type?

        if(!fword)
          grammarStream<<" | ";
        else
          fword = false;

        grammarStream<< word->getWord();
      }

      grammarStream<<" ) ";
    }

    grammarStream<<"\n";

  }
  return true;
}

//WARNING: remove and use proper variant
bool ModelCompilationAdapterSPHINX::removeDirRecursive(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDirRecursive(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}
