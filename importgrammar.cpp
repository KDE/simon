//
// C++ Implementation: importgrammar
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importgrammar.h"
#include "wordlistmanager.h"

ImportGrammar::ImportGrammar(WordListManager *wordListManager, QObject* parent): QThread(parent)
{
	this->wordListManager = wordListManager;
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
	emit status(tr("Öffne Datei..."));
	QStringList structures;
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) return structures;
	

	emit status(tr("Lese Datei..."));
	
	//matches for example the following:
	// this is a test.
	// this is a test...
	// this is a test!
	// this is a test?
	// this is a test!?
	// this is a test?!...!
	// this is a test - or is it (is recognised as two seperate sentences: this is a test; or is it)
	// he said: Test
	QRegExp sentenceStoppers = QRegExp("((\\.|\\?|\\!|:)(\\.|\\?|\\!)*| )-*( |$|\\n)");

	QString leftOvers;
	QString currentSentence;
	while (!file.atEnd())
	{
		QStringList realSentences;
		
		QString sentence=leftOvers;
		leftOvers="";
		
		while (!file.atEnd() && (!sentence.contains(sentenceStoppers)))
			sentence += file.readLine(4000)+"\n";
		
		QStringList sentences = sentence.split(sentenceStoppers, QString::SkipEmptyParts);
		for (int i=0; i < sentences.count()-1;i++)
		{
			currentSentence = sentences[i].trimmed();
			if (currentSentence.trimmed().isEmpty()) continue;
			
			realSentences << currentSentence;
		}
		if (!sentences.at(sentences.count()-1).contains(sentenceStoppers))
			leftOvers = sentences.at(sentences.count()-1);
		else realSentences << leftOvers;
		
		structures << realSentences;
	}
	return structures;
}

QStringList ImportGrammar::importFile(QString path)
{
	emit fileProgress(0, 0); //waiting
	
	QStringList out;

	QStringList structures = readFile(path);
	
	emit status(tr("Verarbeite..."));
	emit fileProgress(0, structures.count());

	WordList* lookupResult;
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
		QStringList words = currentSentence.split(" ");
		
		QString terminal;
		bool everyWordSure=true;
		for (int j=0; (j < words.count()) && everyWordSure; j++)
		{
			lookupResult = wordListManager->getWords(words[j],false /*first - quick lookup*/);
			
			QStringList wordTerminals=terminals(lookupResult);
			if (wordTerminals.count()==0)
			{
				lookupResult = wordListManager->getWords(words[j],true /*extensive lookup*/);
				wordTerminals = terminals(lookupResult);
			}
			
			if (wordTerminals.count()!=1)
				everyWordSure = false;
			else
				words.replace(j, wordTerminals[0]);
				
			delete lookupResult;
		}
		if (everyWordSure)
		{
			//add to output
			out << words.join(" ");
		}
		emit fileProgress(++progress, max);
	}
	
	emit status(tr("Fertig"));
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
	if (!includeUnknown) terminals.removeAll(tr("Unbekannt"));
	return terminals;
}

ImportGrammar::~ImportGrammar()
{
}


