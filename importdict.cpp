//
// C++ Implementation: importdict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importdict.h"
#include "logger.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 */
ImportDict::ImportDict(QObject *parent) : QThread(parent)
{
	
}

/**
 * \brief Parses the wordlist of an existing wiktioray wordlist
 * \author Peter Grasch
 */
void ImportDict::parseWordList(QString pathToDict)
{
	this->pathToDict = pathToDict;
	start(QThread::IdlePriority);
}

/**
 * \brief The main execution loop
 * Does the real world.
 * Uses the members pathToDict to get the location and writes the finished wordlist to the member wordList
 * \author Peter Grasch
 */
void ImportDict::run()
{
	Logger::log("Opening (wiktionary-) lexicon at \""+pathToDict+"\"");
	emit status("Öffne Wörterbuch...");
	
	emit progress(10);
	WiktionaryDict *wdict = new WiktionaryDict(pathToDict);
	connect(wdict, SIGNAL(loaded()), this, SLOT(openingFinished()));
	connect(wdict, SIGNAL(progress(int)), this, SLOT(loadProgress(int)));
	
	emit status("Verarbeite Wörterbuch...");
	
	wdict->load(pathToDict);
	emit status("Erstelle Liste...");
	QStringList words = wdict->getWords();
	QStringList terminals = wdict->getTerminals();
	QStringList pronunciations = wdict->getPronuncations();
	
	WordList* vocablist = new WordList();
	
	for (int i=0; i<words.count(); i++)
	{
		vocablist->append ( Word(QString(words.at(i)), 
				    QString(pronunciations.at(i)), 
					QString(terminals.at(i)), 0 ) );
		emit progress((((double) i)/((double)words.count())) *40+960);
	}
	
	emit progress(1000);
	emit status("Fertig");
	
	Logger::log("Imported "+QString::number(words.count())+" words from the wiktionary-lexicon \""+pathToDict+"\"");
	wordList = vocablist;
	emit finished();
}

/**
 * \brief Translates the progress of the loading of the file to the global importing progress
 * emits the progress(int) signal with the calculated prog.
 * \author Peter Grasch
 * \param int prog
 * The progress of the loading
 */
void ImportDict::loadProgress(int prog)
{
	int globalProg = (((double)prog)/1000)*950+10;
	emit progress(globalProg);
}

/**
 * \brief Finished opening the file
 * Emits opened()
 * \author Peter Grasch
 */
void ImportDict::openingFinished()
{
	emit opened();
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDict::~ImportDict()
{
}


