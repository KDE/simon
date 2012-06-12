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


#include "ModelCompilationAdapterSPHINX.h"
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

    //Creating a directory hierarchy, where model compilation will be executed
    QDir wDir(workingDirPath);
    if(!wDir.mkdir(mName) || !wDir.cd(mName) || !wDir.mkdir("/etc") || !wDir.mkdir("/wav"))
    {
        emit error(i18n("Failed to create directory hierarchy at \"%1\"", workingDirPath));
        return false;
    }

    return true;
}

bool ModelCompilationAdapterSPHINX::storeDictionary(const QString &dictionaryPathOut, QStringList &trainedVocabulary,
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
