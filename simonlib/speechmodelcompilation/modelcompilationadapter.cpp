/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include <QTimer>
#include <KLocale>
#include <KDebug>
#include <KGlobal>
#include <QString>
#include <QFile>
#include <KStandardDirs>
#include <KAboutData>
#include <KComponentData>
#include <simonscenarios/scenario.h>
#include <simonscenarios/vocabulary.h>
#include <simonscenarios/activevocabulary.h>
#include <simonscenarios/grammar.h>

ModelCompilationAdapter::ModelCompilationAdapter(const QString& userName, QObject *parent) : QObject(parent), m_userName(userName)
{
}

void ModelCompilationAdapter::abort()
{
  kDebug() << "Aborting current adaption";
  keepGoing = false;
}

bool ModelCompilationAdapter::removeContextAdditions()
{
    QString realInPrompts = m_promptsPathIn;
    m_promptsPathIn = KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName()+'/'+m_userName+"/compile/tmpprompts");
    QFile newPrompts(m_promptsPathIn);
    QFile oldPrompts(realInPrompts);
    if (!newPrompts.open(QIODevice::WriteOnly) || !oldPrompts.open(QIODevice::ReadOnly)) {
        emit error(i18n("Couldn't strip context of prompts"));
        return false;
    }
    while (!oldPrompts.atEnd()) {
        QByteArray line = oldPrompts.readLine();
        int firstIndex = line.indexOf('"');
        line.remove(firstIndex, line.indexOf('"', firstIndex+1) - firstIndex + 1);
        newPrompts.write(line);
    }
    return true;
}

void ModelCompilationAdapter::mergeInputData(const QStringList &scenarioPaths, QSharedPointer<Vocabulary> mergedVocabulary, QSharedPointer<Grammar> mergedGrammar)
{
    //merging scenarios
    for(const QString& src: scenarioPaths)
    {
        kDebug() << "Serializing Scenario: " << src;
        QSharedPointer<Scenario> scenario (new Scenario(""));
        if (!scenario->readLanguageModel(src))
        {
            kDebug() << "Could not parse language model at " << src;
            continue;
        }

        Grammar *grammar = scenario->grammar();
        mergedGrammar->addStructures(grammar->getStructures(), false /* do not save */);

        Vocabulary *vocab = scenario->vocabulary();

        QList<Word*> words = vocab->getWords();
        vocab->clear();                               // make sure they are not removed from the scenario when we delete that
        QList<Word*> wordsTmp;
        for(Word* w: words)
            wordsTmp.append(w);

        //list will be deleted by addWords
        mergedVocabulary->addWords(wordsTmp);
    }
}

bool ModelCompilationAdapter::containsPoisonedPhoneme(const QString& pronunciation)
{
    if (poisonedPhonemes.isEmpty()) return false;

    QStringList phonemes = pronunciation.split(' ');
    for(const QString& phoneme: phonemes)
        if (poisonedPhonemes.contains(phoneme))
            return true;

    return false;
}

void ModelCompilationAdapter::removeWordsWithPoisonedPhonems(QSharedPointer<Vocabulary> vocabulary)
{
    QList<Word*> words = vocabulary->getWords();
    for(Word *word: words)
    {
        if (containsPoisonedPhoneme(word->getPronunciation()))
        {
            kDebug() << "Removing word containing poisoned phoneme: " << word->getWord();
            vocabulary->removeWord(word);
        }
    }
}


bool ModelCompilationAdapter::readPrompts(ModelCompilationAdapter::AdaptionType adaptionType,
                                              QSharedPointer<Vocabulary> vocabulary, const QString &promptsPathIn,
                                              QStringList &trainedVocabulary)
{
    ///// Prompts ///////////

    if (!poisonedPhonemes.isEmpty() && (adaptionType & ModelCompilationAdapter::AdaptLanguageModel))
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
                kDebug() << "Aborting because we have no input prompts";
                emit adaptionAborted(); //no input prompts
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
                    kDebug() << "Word not defined in vocabulary: " << word;
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

