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


#include "importgrammar.h"
#include "wordlistmanager.h"
#include <KLocalizedString>
#include <QFile>

ImportGrammar::ImportGrammar(QObject* parent): QThread(parent)
{
}

void ImportGrammar::run()
{
	emit allProgress(0, files.count());
	QStringList sentences;
	for (int i=0; i < files.count(); i++)	//for every file
	{
		QStringList newSentences=importFile(files[i]);  //import the file
		for (int j=0; j<newSentences.count(); j++)	//remove the doubles
		{
			if (!sentences.contains(newSentences[j]))
				sentences << newSentences[j]; 	//add them to the list
		}
		emit allProgress(i+1, files.count());
	}
	emit grammarCreated(sentences);
}



QStringList ImportGrammar::readFile(QString path)
{
	emit status(i18n("Opening File..."));
	QStringList structures;
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) return structures;
	

	emit status(i18n("Reading File..."));
	
	//matches for example the following:
	// this is a test.
	// this is a test...
	// this is a test!
	// this is a test?
	// this is a test!?
	// this is a test?!...!
	// this is a test - or is it? (is recognised as two separate sentences: this is a test; or is it)
	// he said: Test
	QRegExp sentenceStoppers = QRegExp("((\\.|\\?|\\!|:)(\\.|\\?|\\!)*| )-*( |$|\\n)");

	QString leftOvers;
	QString currentSentence;
	while (!file.atEnd())
	{
		QStringList realSentences;
		
		QString sentence;//=leftOvers;
// 		leftOvers="";
		
		while (!file.atEnd() && (!sentence.contains(sentenceStoppers)))
			sentence += file.readLine(4000)+"\n";
		
		QStringList sentences = sentence.split(sentenceStoppers, QString::SkipEmptyParts);
		for (int i=0; i < sentences.count();i++)
		{
			currentSentence = sentences[i].trimmed();
			if (currentSentence.trimmed().isEmpty()) continue;
			
			realSentences << currentSentence;
		}

		//if (!sentences.at(sentences.count()-1).contains(sentenceStoppers))
		//	leftOvers = sentences.at(sentences.count()-1);
		//else realSentences << leftOvers;
		
		structures << realSentences;
	}
	return structures;
}

QStringList ImportGrammar::importFile(QString path)
{
	emit fileProgress(0, 0); //waiting
	
	QStringList out;

	QStringList structures = readFile(path);
	
	emit status(i18n("Processing..."));
	emit fileProgress(0, structures.count());

	WordList* lookupResult;
	WordListManager *wordListManager = WordListManager::getInstance();
	QString currentSentence;
	int progress=0;
	int max=structures.count();
	while (!structures.isEmpty())
	{
		currentSentence = structures.takeAt(0);
		currentSentence.remove(","); //cleaning up
		currentSentence.remove(".");
		currentSentence.remove("!");
		currentSentence.remove("?");
		currentSentence.remove("(");
		currentSentence.remove(")");
		currentSentence.remove(":");
		currentSentence.remove("-");
		currentSentence.remove("\"");
		QStringList words = currentSentence.split(" ",QString::SkipEmptyParts);
		
		QString terminal;
		bool everyWordSure=true;
		for (int j=0; (j < words.count()) && everyWordSure; j++)
		{
			lookupResult = wordListManager->getMainstreamWords(words[j] /*first - quick lookup*/);
			
			QStringList wordTerminals=terminals(lookupResult);
			if (wordTerminals.count()==0)
			{
				lookupResult = wordListManager->getShadowedWords(words[j] /*extensive lookup*/);
				wordTerminals = terminals(lookupResult);
			}
			
			if (wordTerminals.count() != 1 /*change this to include ambigous terminals */)
			{
				if (includeUnknown)
					words.replace(j, i18n("Unknown"));
				else 
					everyWordSure = false;
			} else 
					words.replace(j, wordTerminals[0]);
			/*
			if (includeUnknown)
			{
				if (wordTerminals.count() != 1)
					words.replace(j, i18n("Unknown"));
				else 
					words.replace(j, wordTerminals[0]);
			} else {
				if (wordTerminals.count() != 1)
					everyWordSure = false;
				else 
					words.replace(j, wordTerminals[0]);
			}*/

				
			delete lookupResult;
		}
		if (everyWordSure)
		{
			//add to output
			out << words.join(" ");
		}
		emit fileProgress(++progress, max);
	}
	
	emit status(i18n("Finished"));
	return out;
}

QStringList ImportGrammar::terminals(WordList *in)
{
	QStringList terminals;
	QString terminal;
	for (int i=0; i < in->count(); i++)
	{
		terminal = in->at(i).getTerminal();
		if (!terminals.contains(terminal)) terminals << terminal;
	}
	if (!includeUnknown) terminals.removeAll(i18n("Unknown"));
	return terminals;
}

ImportGrammar::~ImportGrammar()
{
}


