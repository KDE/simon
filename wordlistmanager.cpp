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
	readWordList ( path );
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
WordList* WordListManager::readWordList ( QString lexiconpath, QString vocabpath )
{
	wordlist->clear();
	//read the vocab

	//opening
	QFile *lexicon = new QFile ( lexiconpath );
	lexicon->open ( QFile::ReadOnly );
	if ( !lexicon->isReadable() ) return false;

	QFile *vocab = new QFile ( vocabpath );
	vocab->open ( QFile::ReadOnly );
	if ( !vocab->isReadable() ) return false;




	char buffer[1024]; //this will hold the current line
	qint64 length; //this stores the length of the read line - we can determine the end of the file that way
	QString line;

	QString name;
	QString output;
	QString pronunciation;
	QString category;

	length = lexicon->readLine ( buffer, sizeof ( buffer ) );
	//for each line
	while ( length != -1 )
	{
		line = QString ( buffer );
		
		//parsing the line
		name = line.left ( line.indexOf ( "\t" ) ).trimmed();
		output = line.mid ( line.indexOf ( "\t" ), line.lastIndexOf ( "\t" ) - line.indexOf ( "\t" ) ).trimmed();
		output = output.mid ( 1, output.length()-2 );
		pronunciation = line.right ( line.length() - line.lastIndexOf ( "\t" ) ).trimmed();
		
		
		//creates and appends the word to the wordlist
		wordlist->append ( new Word ( output, pronunciation, "undefined", 0 ) );

		//reading the next line
		length = lexicon->readLine ( buffer, sizeof ( buffer ) );
	}

	return this->wordlist;
}

WordListManager::~WordListManager()
{}
