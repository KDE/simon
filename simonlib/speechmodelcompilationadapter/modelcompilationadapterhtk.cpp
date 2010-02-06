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

#include "modelcompilationadapterhtk.h"

#include <simonlogging/logger.h>

#include <simonscenarios/scenario.h>
#include <simonscenarios/vocabulary.h>
#include <simonscenarios/activevocabulary.h>
#include <simonscenarios/grammar.h>

#include <KDebug>
#include <QFile>



ModelCompilationAdapterHTK::ModelCompilationAdapterHTK(const QString& userName, QObject *parent) : ModelCompilationAdapter(userName, parent)
{
}

bool ModelCompilationAdapterHTK::adaptModel(ModelCompilationAdapter::AdaptionType adaptionType,
		const QStringList& scenarioPaths, const QString& promptsPathIn, 
			const QString& lexiconPathOut, const QString& grammarPathOut, 
			const QString& simpleVocabPathOut, const QString& promptsPathOut)
{
	Vocabulary *mergedVocabulary = new Vocabulary();
	Grammar *mergedGrammar = new Grammar();

	//merging scenarios
	foreach (const QString& src, scenarioPaths) {
		kDebug() << "Serializing Szenario: " << src;
		Scenario *s = new Scenario("");
		if (!s->readLanguageModel(src))  {
			kDebug() << "Couldn't parse language model at " << src;
			continue;
		}

		Grammar *grammar = s->grammar();
		mergedGrammar->addStructures(grammar->getStructures(), false /* don't save */);

		Vocabulary *vocab = s->vocabulary();

		QList<Word*> words = vocab->getWords();
		vocab->clear(); // make sure they are not removed from the scenario when we delete that
		QList<Word*> *wordsTmp = new QList<Word*>();
		foreach (Word* w, words)
			wordsTmp->append(w);

		//list will be deleted by addWords
		mergedVocabulary->addWords(wordsTmp);
		delete s;
	}

	if (!storeModel(adaptionType, lexiconPathOut, simpleVocabPathOut, grammarPathOut, 
				promptsPathOut, mergedVocabulary, mergedGrammar, promptsPathIn))
		return false;

	delete mergedVocabulary;
	delete mergedGrammar;
	return true;
}

QString ModelCompilationAdapterHTK::htkify(const QString& in)
{
	return QString::fromUtf8(htkify(in.toUtf8()));
}

QByteArray ModelCompilationAdapterHTK::htkify(const QByteArray& in)
{
	QByteArray out = in;
	out.replace("0", "0ZERO");
	out.replace("1", "1ONE");
	out.replace("2", "2TWO");
	out.replace("3", "3THREE");
	out.replace("4", "4FOUR");
	out.replace("5", "5FIVE");
	out.replace("6", "6SIX");
	out.replace("7", "7SEVEN");
	out.replace("8", "8EIGHT");
	out.replace("9", "9NINE");
	return out;
}


bool ModelCompilationAdapterHTK::storeModel(ModelCompilationAdapter::AdaptionType adaptionType,
		const QString& lexiconPathOut, const QString& simpleVocabPathOut, const QString& grammarPathOut, 
			const QString& promptsPathOut, Vocabulary* vocab, Grammar *grammar, const QString& promptsPathIn)
{
	///// Prompts ///////////
	QList<QByteArray> trainedVocabulary; // words where prompts exist
	QList<QString> definedVocabulary; // words that are in the dictionary

	if (adaptionType & ModelCompilationAdapter::AdaptAcousticModel) {
		QFile promptsFile(promptsPathIn);
		QFile promptsFileOut(promptsPathOut);

		if (!promptsFile.open(QIODevice::ReadOnly) || 
				(!(adaptionType & ModelCompilationAdapter::AdaptLanguageModel) && 
				 !promptsFileOut.open(QIODevice::WriteOnly)))
			return false;

		while (!promptsFile.atEnd())
		{
			QByteArray line = promptsFile.readLine();
			int splitter = line.indexOf(" ");
			if (!(adaptionType & ModelCompilationAdapter::AdaptLanguageModel))
				promptsFileOut.write(line.left(splitter) /*filename*/ + htkify(line.mid(splitter)));

			QList<QByteArray> words = line.mid(splitter+1).trimmed().split(' ');
			foreach (const QByteArray& word, words) {
				if (!trainedVocabulary.contains(word))
					trainedVocabulary.append(word);
			}
		}

		promptsFile.close();
		if (!(adaptionType & ModelCompilationAdapter::AdaptLanguageModel))
			promptsFileOut.close();
	}

	if (!(adaptionType & ModelCompilationAdapter::AdaptLanguageModel))
		return true;

	/////  Lexicon  ////////////////
	QFile lexiconFile(lexiconPathOut);
	QFile simpleVocabFile(simpleVocabPathOut);
	if (!lexiconFile.open(QIODevice::WriteOnly) ||
			(!simpleVocabFile.open(QIODevice::WriteOnly)))
		return false;

	QTextStream lexicon(&lexiconFile);
	lexicon.setCodec("UTF-8");

	bool sentWritten = false;
	QList<Word*> words = vocab->getWords();
	QString htkIfiedWord;
	foreach (Word *w, words) {
		if ((adaptionType & ModelCompilationAdapter::AdaptAcousticModel) &&
				!trainedVocabulary.contains(w->getLexiconWord().toUtf8())) {
			continue;
		}
		htkIfiedWord = htkify(w->getLexiconWord());

		if (!sentWritten && (htkIfiedWord >= "SENT-END")) {
			lexicon << "SENT-END\t\t[]\t\tsil\n";
			lexicon << "SENT-START\t\t[]\t\tsil\n";
			sentWritten=true;
		}
		lexicon << htkIfiedWord << "\t\t[" << w->getWord() << "]\t\t" <<
				w->getPronunciation() << "\n";

		if (!definedVocabulary.contains(htkIfiedWord))
			definedVocabulary << htkIfiedWord;
	}
	if (!sentWritten) {
		lexicon << "SENT-END\t\t[]\t\tsil\n";
		lexicon << "SENT-START\t\t[]\t\tsil\n";
	}

	lexiconFile.close();

	/////  Vocabulary  /////////////
	

	// find out which words are referenced by training data
	// find out which terminals are referenced by grammar
	// find o
	QTextStream vocabStream(&simpleVocabFile);
	vocabStream.setCodec("UTF-8");

	//print internal sentence structure
	vocabStream << "% NS_B\n<s>\tsil\n";
	vocabStream << "% NS_E\n</s>\tsil\n";

	QStringList grammarTerminals = grammar->getTerminals();

	QStringList structures = grammar->getStructures();
	bool everythingChanged = true;

	while (everythingChanged) {
		everythingChanged = false;
		foreach (const QString& terminal, grammarTerminals) {
			//if there are no words for this terminal, remove it from the list
			QList<Word*> wordsForTerminal = vocab->findWordsByTerminal(terminal);

			bool hasAssociatedWord = false;
			foreach (Word *w, wordsForTerminal) {
				if ((!(adaptionType & ModelCompilationAdapter::AdaptAcousticModel)) ||
						trainedVocabulary.contains(w->getLexiconWord().toUtf8()))
					hasAssociatedWord = true;
				else vocab->removeWord(w, true /* delete word*/);
			}
			if ((adaptionType & ModelCompilationAdapter::AdaptAcousticModel) && !hasAssociatedWord) {
				kDebug() << "Terminal has no associated words: " << terminal;
				grammarTerminals.removeAll(terminal);

				for (int i=0; i < structures.count(); i++)
				{
					if (structures[i].contains(terminal))
					{
						//delete all words of all terminals in this structure
						QStringList structureElements = structures[i].split(' ');
						foreach (const QString& structureTerminal, structureElements) {
							QList<Word*> wordsToDelete = vocab->findWordsByTerminal(structureTerminal);
							foreach (Word *w, wordsToDelete)
								vocab->removeWord(w, true);
						}

						grammar->deleteStructure(i--);
						structures = grammar->getStructures();
					}
				}
				everythingChanged = true;
				break;
			}
		}
	}

	
	foreach (const QString& terminal, grammarTerminals) {
		//only store vocabulary that is referenced by the grammar
		QList<Word*> wordsForTerminal = vocab->findWordsByTerminal(terminal);

		vocabStream << "% " << terminal << "\n";
		foreach (const Word *w, wordsForTerminal)
			vocabStream << w->getWord() << "\t" << w->getPronunciation() << "\n";
	}
	simpleVocabFile.close();


	///// Grammar ///////////
	QFile grammarFile(grammarPathOut);
	if (!grammarFile.open(QIODevice::WriteOnly)) return false;

	foreach (const QString& structure, structures) 
		grammarFile.write("S:NS_B "+structure.toUtf8()+" NS_E\n");
	grammarFile.close();

	///// Prompts (2) //////
	if (adaptionType & ModelCompilationAdapter::AdaptAcousticModel) {
		QFile promptsFile(promptsPathIn);
		QFile promptsFileOut(promptsPathOut);

		if (!promptsFile.open(QIODevice::ReadOnly) || !promptsFileOut.open(QIODevice::WriteOnly))
			return false;

		while (!promptsFile.atEnd())
		{
			QByteArray line = promptsFile.readLine();
			int splitter = line.indexOf(" ");
			bool allWordsInLexicon = true;

			QList<QByteArray> words = line.mid(splitter+1).trimmed().split(' ');
			foreach (const QByteArray& word, words) {
				if (!definedVocabulary.contains(word)) {
					allWordsInLexicon = false;
					break;
				}
			}
			if (allWordsInLexicon)
				promptsFileOut.write(line.left(splitter) /*filename*/ + htkify(line.mid(splitter)));
		}

		promptsFile.close();
		promptsFileOut.close();
	}
	return true;
}


ModelCompilationAdapterHTK::~ModelCompilationAdapterHTK()
{
}

