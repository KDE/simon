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
#include "logger.h"
#include <QObject>
#include <QList>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QtGlobal>
#include <QTextStream>
#include "modelmanager.h"
#include "simoninfo.h"
#include "settings.h"

/**
 * @brief Constructor
 *
 * Reads the vocab and initializes the wordlist (member) with it
 *
 * @author Peter Grasch
 * @param QString path
 * Sets the path (member) to the given string
 */
WordListManager::WordListManager ( ) : QThread()
{
	this->wordlist = readWordList ( Settings::getS("Model/PathToLexicon"), Settings::getS("Model/PathToVocab"), Settings::getS("Model/PathToPrompts") );
	start();
	this->modelManager = new ModelManager();
}

bool WordListManager::compileModel()
{
	return this->modelManager->compileModel();
}

/**
 * \brief Starts the importing of the shadow-Model in a new thread
 * \author Peter Grasch
 */
void WordListManager::run()
{
	this->shadowList = readWordList(Settings::getS("Model/PathToShadowLexicon"), Settings::getS("Model/PathToShadowVocab"), Settings::getS("Model/PathToPrompts"));
}


/**
 * \brief Sorts the given List of words
 * \author Peter Grasch
 * \param WordList* list
 * The list of words to sort
 * \return WordList*
 * The sorted list of words
 */
WordList* WordListManager::sortList(WordList* list)
{
	Logger::log(QObject::tr("[INF] Sortiere eine Liste mit %1 Wörtern").arg(list->count()));
	qSort(list->begin(), list->end());
	return list;
}


WordList* WordListManager::getShadowList()
{
	//if the thread is still running we are obviously not ready to give out the shadowdict
	//wait till we are finished
	if (isRunning())
	{
		SimonInfo::showMessage(tr("Bitte warten Sie während das Schatten-Wörterbuch lädt..."),1000);
		QCoreApplication::processEvents();
		//this is actually bug-using as we just set any kind of timeout (the gui thread will be
		//blocked until the importing is done anyway...
		wait();
	}
	
	return shadowList;
}

/**
 * \brief Saves the current WordList (member)
 * \author Peter Grasch
 * \param QString filename
 * If not provided we default on the filename we used to open the file
 * \return bool
 * Saving successful?
 */
bool WordListManager::save ( QString lexiconFilename, QString vocabFilename )
{
	Logger::log(QObject::tr("[INF] Speichere Wörterliste"));
	
	WordList *saving = this->wordlist;
	
	saving->append(Word("SENT-START", "sil", "NS_B", 0));
	saving->append(Word("SENT-END", "sil", "NS_E", 0));
	
	if (lexiconFilename.isEmpty()) lexiconFilename = Settings::getS("Model/PathToLexicon");
	if (vocabFilename.isEmpty()) vocabFilename = Settings::getS("Model/PathToVocab");
	
	Logger::log(QObject::tr("[INF] Öffnen der Ausgabedatei: %1").arg(lexiconFilename));
	
	QFile *outfile = new QFile(lexiconFilename);
	if (!outfile->open(QIODevice::WriteOnly)) {
		Logger::log(QObject::tr("[ERR] Fehler beim öffnen der Ausgabedatei %1").arg(lexiconFilename));
		return false;
	}
	QTextStream outstream(outfile);
	outstream.setCodec("ISO 8859-15");
	
	for (int i=0; i< saving->count(); i++)
	{
		outstream << QString(saving->at(i).getWord().trimmed().toUpper() 
				+ "\t\t[" + saving->at(i).getWord().trimmed() + "]\t\t" +
				saving->at(i).getPronunciation()).trimmed() << "\n";
	}

	Logger::log(QObject::tr("[INF] Schießen der Ausgabedatei"));
	outfile->close();
	
	return true;
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
	Logger::log (QObject::tr("[INF] Lesen der Wörterliste bestehend aus "));
	Logger::log(QObject::tr("[INF] \t\tLexikon: %1,").arg(lexiconpath));
	Logger::log(QObject::tr("[INF] \t\tVocabular: %1,").arg(vocabpath));
	Logger::log(QObject::tr("[INF] \t\tPrompts: %1").arg(promptspath));

	WordList *wordlist = new WordList();
	//read the vocab
	WordList *vocablist = readVocab(vocabpath);
	//read the prompts
	PromptsTable *promptsTable = readPrompts(promptspath);

	//opening
	Logger::log(QObject::tr("[INF] Öffnen des Lexikons von: %1").arg(lexiconpath));
	QFile *lexicon = new QFile ( lexiconpath );
	
	if ( !lexicon->open ( QFile::ReadOnly ) || !vocablist || !promptsTable) return false;
	
	QString line, name, output, pronunciation, term, *termtmp;
	int wordend, outend, probability;
	while (!lexicon->atEnd())
	{
		line = lexicon->readLine(1024);
		wordend = line.indexOf("[");
		outend = line.indexOf("]");
		name = line.left(wordend).trimmed();
		output = line.mid(wordend+1, outend-wordend-1);
		pronunciation = line.mid(outend+1).trimmed();
		
		termtmp = getTerminal( name, pronunciation, vocablist );
		term = (termtmp) ? *termtmp : "Unbekannt";
		
		probability = getProbability( name, promptsTable );

		//creates and appends the word to the wordlist
		wordlist->append(Word(output, pronunciation, term, probability));
		
	}


	Logger::log(QObject::tr("[INF] Wörterliste erstellt"));
	return wordlist;
}


WordList* WordListManager::removeDoubles(WordList *in)
{
	if (!in) return NULL;
	
	Logger::log(QObject::tr("[INF] Entfernen der doppelten Einträge"));
	
	for (int i=0; i < in->count(); i++)
	{
		for (int j=i; j < in->count(); j++)
		{
			if (in->at(i).getWord() == in->at(j).getWord())
			{
				in->removeAt(j);
			}
		}
	}
	return in;
}


/**
 * \brief Returns the Terminal of the name & pronunciation (it is pulled out of the wlist)
 * \author Peter Grasch
 * \param QString name
 * Name of the word
 * \param QString pronunciation
 * Pronunciation of the word
 * \param WordList *wlist
 * The list of words where we look for the Terminal
 * \return QString*
 * The found terminal (or NULL if none matched)
 */
QString* WordListManager::getTerminal(QString name, QString pronunciation, WordList *wlist)
{
	int i=0, wordcount = wlist->count();
	QString uppername = name.toUpper();
	while ((i < wordcount) && (wlist->at( i ).getWord().toUpper() != uppername) && 
			( wlist->at( i ).getPronunciation() != pronunciation))
	{ i++; };
	if ((i<wordcount) && (wlist->at(i).getWord().toUpper() == uppername) && 
			( wlist->at( i ).getPronunciation() != pronunciation))
	{
		//Because vocabs have just one pronunciation for each entry
		return  new QString(((wlist->at( i ).getTerminal())));
	}
	// there was no result
	return NULL;
}

/**
 * \brief Adds the words to the local wordlist
 * Inserts the words into the current list (alphabetically)
 * Tries to omit duplicates...
 * \author Peter Grasch
 * \param WordList *list
 * List of words to add (DANGER: this pointer is invalid after calling this function!)
 */
void WordListManager::addWords(WordList *list, bool isSorted, bool shadow)
{
	if (!isSorted)
		list = sortList(list);

	WordList *target;
	if (shadow) target = getShadowList();
	else target = this->wordlist;

	Logger::log(QObject::tr("[INF] Hinzufügen von %1 Wörtern in die Wörterliste").arg(list->count()));

	int i=0;
	WordList *main, *newList;

	if (list->count() < wordlist->count())
	{
		main=target;
		newList = list;
	} else
	{
		main= list;
		newList = target;
	}
	int wordcount = main->count();
	
	
	while (newList->count() >0)
	{
		Word tmp=newList->at(0);
		while ((i<wordcount-1) && (main->at(i) < tmp))
		{ i++; }
		if (main->at(i) != tmp) //no doubles
		{
			main->insert(i, tmp);
			wordcount++;
		}
		newList->removeAt(0); //remove the double
	}
	
	target = main;
	delete newList;
// 	if (list) delete list;

	Logger::log(QObject::tr("[INF] Die Wortliste beinhaltet jetzt %1 Wörter").arg(wordlist->count()));
	this->save();

	emit wordlistChanged();
}

/**
 * \brief Returns the probability of the name (it is pulled out of the promptsTable)
 * \author Peter Grasch
 * \param QString wordname
 * Name of the word
 * \param PromptsTable
 * Promptsfile
 * \return int
 * Probability to recognize; The higher the more likly simon will recognize this word correctly
 */
int WordListManager::getProbability(QString wordname, PromptsTable *promptsTable)
{
	QStringList prompts = promptsTable->values();
	int prob=0;
	int i=0;
	QString line;
	while (i<prompts.count())
	{
		prob += prompts.at(i).count(wordname);
		i++;
	}
	return prob;
}

/**
 * \brief Builds and returns the promptstable by parsing the file at the given path
 * \author Peter Grasch
 * \param QString promptspath
 * The path to the prompts file
 * \return PromptsTable*
 * The table
 */
PromptsTable* WordListManager::readPrompts(QString promptspath)
{
	Logger::log(QObject::tr("[INF] Parse Promptsdatei von %1").arg(promptspath));
	PromptsTable *promptsTable = new PromptsTable();
	
	QFile *prompts = new QFile ( promptspath );
	prompts->open ( QFile::ReadOnly );
	if ( !prompts->isReadable() ) return false;
	
	QString label;
	QString prompt;
	QString line;
	int labelend;
	while ( !prompts->atEnd() ) //for each line that was successfully read
	{
		line = prompts->readLine(1024);
		labelend = line.indexOf(" ");
		label = line.left(labelend);
		prompt = line.mid(labelend).trimmed();

		promptsTable->insert( label, prompt );
	}
	
	return promptsTable;
}

/**
 * \brief Reads the vocab of the given file and parses it into the WordList which is returned
 * \author Peter Grasch
 * \param QString vocabpath
 * Path to the vocabs
 * \return WordList*
 * The list of parsed vocab
 */
WordList* WordListManager::readVocab(QString vocabpath)
{
	Logger::log(QObject::tr("[INF] Lese Vokabular von %1").arg(vocabpath));
	WordList *vocablist = new WordList();
	
	QFile *vocab = new QFile ( vocabpath );
	vocab->open ( QFile::ReadOnly );
	if ( !vocab->isReadable() ) return false;
	
	QString line;
	QString name;
	QString pronunciation;
	QString terminal;
	int foundPos;

	while ( !vocab->atEnd() ) //for each line that was successfully read
	{
		line =vocab->readLine(1024);
		
		if (line.startsWith("%")) 
		{
			//The Line is the Definition of the terminal
			terminal = line.mid(2).trimmed();
		}
		
		//parsing the line
		foundPos = line.indexOf ( "\t" );
		name = line.left ( foundPos ).trimmed();
		pronunciation = line.mid ( foundPos ).trimmed();
		
		//creates and appends the word to the wordlist
		vocablist->append ( Word(name, pronunciation, terminal, 0 ) );
	}
	
	return vocablist;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
WordListManager::~WordListManager()
{}
