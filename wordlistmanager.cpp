//
// C++ Implementation: wordlistmanager
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wordlistmanager.h"

/**
 * @brief Constructor
 *
 * Reads the vocab and initializes the wordlist (member) with it
 *
 * @author Peter Grasch
 *
 */
WordListManager::WordListManager ( QString path )
{
	this->wordlist = new WordList();
	this->wordlist = readWordList ( path );
}

/**
 * @brief Reads the Vocab and returns the read Data
 *
 * This method parses the given File and creates a WordList out of it
 *
 * @author Peter Grasch
 *
 * @param QString path
 * Path to the lexicon
 * @return Wordlist*
 * The parsed WordList
 */
WordList* WordListManager::readWordList ( QString lexiconpath, QString vocabpath, QString promptspath )
{
	WordList *wordlist = new WordList();
	//read the vocab
	WordList *vocablist = readVocab(vocabpath);
	//read the prompts
	PromptsTable *promptsTable = readPrompts(promptspath);

	//opening
	QFile *lexicon = new QFile ( lexiconpath );
	lexicon->open ( QFile::ReadOnly );
	
	if ( !lexicon->isReadable() || !vocablist || !promptsTable) return false;

	char buffer[1024]; //this will hold the current line
	qint64 length; //this stores the read length - we can determine the end of the file that way
	QString line;
	QString name;
	QString output;
	QString pronunciation;
	QString category;
	int probability;

	length = lexicon->readLine ( buffer, sizeof ( buffer ) );
	
	while ( length != -1 )	//for each line that was successfully read
	{
		line = QString ( buffer );
		
		//parsing the line
		name = line.left ( line.indexOf ( "\t" ) ).trimmed();
		output = line.mid ( line.indexOf ( "\t" ), line.lastIndexOf ( "\t" ) - line.indexOf ( "\t" ) ).trimmed();
		output = output.mid ( 1, output.length()-2 );
		pronunciation = line.right ( line.length() - line.lastIndexOf ( "\t" ) ).trimmed();
		
		category = getTerminal( name, pronunciation, vocablist );
		probability = getProbability( name, promptsTable );
		
		//creates and appends the word to the wordlist
		if (output != "")
			wordlist->append ( new Word ( output, pronunciation, category, probability ) );

		//reading the next line
		length = lexicon->readLine ( buffer, sizeof ( buffer ) );
	}
	return wordlist;
}

QString WordListManager::getTerminal(QString name, QString pronunciation, WordList *wlist)
{
	int i=0;
	QString terminal("");
	while (i < wlist->count())
	{
		//Because vocabs have just one pronunciation for each entry
		if ((wlist->at( i )->getWord() == name) && 
				   ( (* wlist->at( i )->getPronunciation(0)) == pronunciation))
			
			terminal += (terminal.isEmpty()) ? wlist->at( i )->getTerminal() :
					", " + wlist->at( i )->getTerminal();
		i++;
	}
	// there was no result
	return (terminal.isEmpty()) ? "Unbekannt" : terminal;
}

int WordListManager::getProbability(QString wordname, PromptsTable *promptsTable)
{
	int i=0;
	int probability = 0;
	int foundpos;
	QList<QString> promptsList = promptsTable->values();
	while (i < promptsList.count())
	{
		foundpos = promptsList.at(i).indexOf( wordname );
		while (foundpos != -1)
		{
			probability++;
			promptsList.replace( i, promptsList.at(i).mid(
					     promptsList.at(i).indexOf( wordname )+ wordname.length()));
			foundpos = promptsList.at(i).indexOf( wordname );
		}
		i++;
	}
	
	return probability;
}

PromptsTable* WordListManager::readPrompts(QString promptspath)
{
	PromptsTable *promptsTable = new PromptsTable();
	
	QFile *prompts = new QFile ( promptspath );
	prompts->open ( QFile::ReadOnly );
	if ( !prompts->isReadable() ) return false;
	
	char buffer[1024]; //this will hold the current line
	qint64 length; //this stores the read length - we can determine the end of the file that way
	QString label;
	QString prompt;
	QString line;
	length = prompts->readLine ( buffer, sizeof ( buffer ) );
	while ( length != -1 ) //for each line that was successfully read
	{
		line = QString ( buffer );
		
		label = line.left(line.indexOf(" ")).trimmed();
		prompt = line.right( line.length() - line.indexOf(" ") ).trimmed();

		promptsTable->insert( label, prompt );
		//reading the next line
		length = prompts->readLine ( buffer, sizeof ( buffer ) );
	}
	
	return promptsTable;
}

WordList* WordListManager::readVocab(QString vocabpath)
{
	WordList *vocablist = new WordList();
	
	QFile *vocab = new QFile ( vocabpath );
	vocab->open ( QFile::ReadOnly );
	if ( !vocab->isReadable() ) return false;
	
	char buffer[1024]; //this will hold the current line
	qint64 length; //this stores the read length - we can determine the end of the file that way
	QString line;
	QString name;
	QString pronunciation;
	QString terminal;

	length = vocab->readLine ( buffer, sizeof ( buffer ) );
	while ( length != -1 ) //for each line that was successfully read
	{
		line = QString ( buffer );
		
		if (line.indexOf("%") == 0) 
		{
			//The Line is the Definition of the terminal
			terminal = line.mid(2).trimmed();
		}
		
		//parsing the line
		name = line.left ( line.indexOf ( "\t" ) ).trimmed();
		pronunciation = line.mid ( line.indexOf ( "\t" ) ).trimmed();
		
		//creates and appends the word to the wordlist
		vocablist->append ( new Word(name, pronunciation, terminal, 0 ) );

		//reading the next line
		length = vocab->readLine ( buffer, sizeof ( buffer ) );
	}
	
	return vocablist;
}

WordListManager::~WordListManager()
{}
