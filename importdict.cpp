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
#include "bompdict.h"

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
void ImportDict::parseWordList(QString pathToDict, int type)
{
	this->pathToDict = pathToDict;
	this->type = type;
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
	Logger::log(tr("Öffne lexikon")+" \""+pathToDict+"\"");
	emit status(tr("Öffne Wörterbuch..."));
	
	emit progress(10);
	Dict *dict;
	if (type == WIKTIONARY)
		dict = new WiktionaryDict(pathToDict);
	else dict = new BOMPDict(pathToDict);
	
	connect(dict, SIGNAL(loaded()), this, SLOT(openingFinished()));
	connect(dict, SIGNAL(progress(int)), this, SLOT(loadProgress(int)));
	
	emit status(tr("Verarbeite Wörterbuch..."));
	
	dict->load(pathToDict);
	emit status(tr("Erstelle Liste..."));
	QStringList words = dict->getWords();
	QStringList terminals = dict->getTerminals();
	QStringList pronunciations = dict->getPronuncations();
	
	WordList* vocablist = new WordList();
	
	for (int i=0; i<words.count(); i++)
	{
		vocablist->append ( Word(QString(words.at(i)), 
				    QString(pronunciations.at(i)), 
					QString(terminals.at(i)), 0 ) );
		emit progress((int) ((((double) i)/((double)words.count())) *40+800));
	}
	emit status(tr("Sortiere Wörterbuch..."));
	qSort(vocablist->begin(), vocablist->end());
	
	
	emit progress(1000);
	emit status(tr("Wörterbuch wird verteilt..."));
	
	Logger::log(QString::number(words.count())+" "+tr("Wörter aus dem lexikon")+" \""+pathToDict+"\""+tr(" importiert"));
	emit finished(vocablist);
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
	int globalProg =(int)  ((((double)prog)/1000)*800+10);
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


