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
#include <QDebug>
#include <QObject>

/**
 * @brief Constructor
 *
 * Reads the vocab and initializes the wordlist (member) with it
 *
 * @author Peter Grasch
 * @param QString path
 * Sets the path (member) to the given string
 */
WordListManager::WordListManager ( QString lexiconPath, QString vocabPath )
{
	this->wordlist = new WordList();
	this->lexiconPath = lexiconPath;
	this->vocabPath = vocabPath;
// 	this->extralist = new WordList();
	this->wordlist = readWordList ( lexiconPath, vocabPath );
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
	Logger::log(QObject::tr("[INF] Erstelle Wörterliste unter Verwendung des Vokabulars von %1 und der derzeitig Wörterliste").arg(vocabFilename));
	
	WordList *saving = this->wordlist;
// 	for (int i=0; i < extralist->count(); i++)
// 		saving->append(this->extralist->at(i));
	
	saving->append(Word("SENT-START", QStringList("sil"), "NS_B", 0));
	saving->append(Word("SENT-END", QStringList("sil"), "NS_E", 0));
	
	
	if (lexiconFilename.isEmpty()) lexiconFilename = this->lexiconPath;
	if (vocabFilename.isEmpty()) vocabFilename = this->vocabPath;
	
	Logger::log(QObject::tr("[INF] Opening output file: ").arg(lexiconPath));
	
	QFile *outfile = new QFile(lexiconPath);
	if (!outfile->open(QIODevice::WriteOnly)) {
		Logger::log(QObject::tr("[ERR] Fehler beim öffnen der Ausgabedatei %1").arg(lexiconPath));
		return false;
	}
	QTextStream outstream(outfile);
	outstream.setCodec("ISO 8859-15");
	
	for (int i=0; i< saving->count(); i++)
	{
		for (int j=0; j<saving->at(i).getPronunciations().count(); j++)
			outstream << QString(saving->at(i).getWord().trimmed().toUpper() 
				+ "\t\t[" + saving->at(i).getWord().trimmed() + "]\t\t" +
				saving->at(i).getPronunciations().at(j)).trimmed() << "\n";
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
	Logger::log(QObject::tr("[INF] \t\tLexikon: %1,").arg(lexiconPath));
	Logger::log(QObject::tr("[INF] \t\tVocabular: %1,").arg(vocabPath));
	Logger::log(QObject::tr("[INF] \t\tPrompts: %1").arg(promptspath));
	
	WordList *wordlist = new WordList();
	//read the vocab
	WordList *vocablist = readVocab(vocabpath);
	//read the prompts
	PromptsTable *promptsTable = readPrompts(promptspath);

	//opening
	Logger::log(QObject::tr("[INF] Öffnen des Lexikons von: ").arg(lexiconPath));
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
	
	Logger::log(QObject::tr("[INF] Leeren der Wörterliste"));
	
	for (int i=0; i < in->count(); i++)
	{
		for (int j=i; j < in->count(); j++)
		{
			if (in->at(i).getWord() == in->at(j).getWord())
			{
				for (int k=0; k < in->at(i).getPronunciations().count(); k++)
				{
					Word w = in->at(i);
					w.delPronunciation(w.getPronunciations().at(k));
				}
				if (in->at(j).getPronunciations().count() == 0)
				{
					in->removeAt(j);
				}
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
			( (* wlist->at( i ).getPronunciation(0)) != pronunciation))
	{ i++; };
	if ((i<wordcount) && (wlist->at(i).getWord().toUpper() == uppername) && 
			( (* wlist->at( i ).getPronunciation(0)) != pronunciation))
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
 * List of words to add (DANGER: this pointer might be invalid after calling this function!)
 */
void WordListManager::addWords(WordList *list, bool isSorted)
{
	if (!isSorted)
		list = sortList(list);
// 
	Logger::log(QObject::tr("[INF] Hinzufügen von %1 Wörtern in die Wörterliste").arg(list->count()));

	int i=0;
	WordList *main, *newList;

	if (list->count() < wordlist->count())
	{
		main=this->wordlist;
		newList = list;
	} else
	{
		main= list;
		newList = this->wordlist;
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
	
	this->wordlist = main;
	delete newList;

	Logger::log(QObject::tr("[INF] Die Wortliste beinhaltet jetzt %1 Wörter").arg(wordlist->count()));
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
	Logger::log(QObject::tr("[INF] Parse Promptsdatei von ").arg(promptspath));
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
	Logger::log(QObject::tr("[INF] Lese Vokabular von ").arg(vocabpath));
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
