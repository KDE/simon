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

#include <QDebug>

/**
 * @brief Constructor
 *
 * Reads the vocab and initializes the wordlist (member) with it
 *
 * @author Peter Grasch
 * @param QString path
 * Sets the path (member) to the given string
 */
WordListManager::WordListManager ( TrainingManager *trainManager ) : QThread()
{
	this->wordlist = readWordList ( Settings::getS("Model/PathToLexicon"), Settings::getS("Model/PathToVocab"), Settings::getS("Model/PathToPrompts"), this->activeTerminals );
	mainDirty = false;
	this->modelManager = new ModelManager();
	this->trainManager = trainManager;
	start();
	isTemp = false;
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
	shadowDirty = false;
	this->shadowList = readWordList(Settings::getS("Model/PathToShadowLexicon"), Settings::getS("Model/PathToShadowVocab"), Settings::getS("Model/PathToPrompts"), this->shadowTerminals);
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
bool WordListManager::save ( QString lexiconFilename, QString vocabFilename,
			     QString shadowLexiconFilename, QString shadowVocabFilename )
{
	if (isTemp){
		QMessageBox::warning(0, tr("Temporäre Änderungen"), tr("Sie verändern eine temporäre Wordliste.\n\nDie Änderungen werden nicht gespeichert. Bitte konfigurieren Sie einen korrekten Pfad in den Einstellungen und starten Sie simon neu um eine dauerhafte Wortliste zu benutzen."));
	}

	Logger::log(QObject::tr("[INF] Speichere Wörterliste"));
	
	//save wordlist
	if (mainDirty)	//we changed the wordlist
	{
		if (lexiconFilename.isEmpty()) lexiconFilename = Settings::getS("Model/PathToLexicon");
		if (vocabFilename.isEmpty()) vocabFilename = Settings::getS("Model/PathToVocab");
		saveWordList(this->getWordList(), lexiconFilename, vocabFilename);
		emit wordlistChanged();
		mainDirty=false;
	}
	
	//shadowlist
	if (shadowDirty) //we changed the shadowDict
	{
		if (shadowLexiconFilename.isEmpty()) shadowLexiconFilename = Settings::getS("Model/PathToShadowLexicon");
		if (shadowVocabFilename.isEmpty()) shadowVocabFilename = Settings::getS("Model/PathToShadowVocab");
		saveWordList(this->getShadowList(), shadowLexiconFilename, shadowVocabFilename);
		emit shadowListChanged();
		shadowDirty=false;
	}
	
	
	return true;
}


/**
 * \brief Saves the given wordlist to the given files
 * \author Peter Grasch
 * \todo implement vocab-saving 
 * @param list The list to save
 * @param lexiconFilename The lexicon to write to
 * @param vocabFilename The vocabfile to write to
 * @return Success
 */
bool WordListManager::saveWordList(WordList *list, QString lexiconFilename, QString vocabFilename)
{
	Logger::log(QObject::tr("[INF] Öffnen der Ausgabedatei: %1").arg(lexiconFilename));
	
	QFile *outfile = new QFile(lexiconFilename);
	if (!outfile->open(QIODevice::WriteOnly)) {
		Logger::log(QObject::tr("[ERR] Fehler beim öffnen der Ausgabedatei %1").arg(lexiconFilename));
		return false;
	}
	QTextStream outstream(outfile);
	outstream.setCodec("ISO 8859-15");
	
	int i=0;
	int count = list->count();
	while ((i<count)&&(list->at(i).getWord().toUpper() < "SENT-END"))
	{
		outstream << QString(list->at(i).getWord().toUpper() 
				+ "\t\t[" + list->at(i).getWord() + "]\t\t" +
				list->at(i).getPronunciation()) << "\n";
		i++;
	}
	outstream << "SENT-END\t\t[]\t\tsil\n";
	outstream << "SENT-START\t\t[]\t\tsil\n";
	while (i<count)
	{
		outstream << QString(list->at(i).getWord().toUpper() 
				+ "\t\t[" + list->at(i).getWord() + "]\t\t" +
				list->at(i).getPronunciation()) << "\n";
		i++;
	}

	Logger::log(QObject::tr("[INF] Schießen der Ausgabedatei"));
	outfile->close();
	return true;
}

/**
 * \brief Inits the lists to empty lists
 * \author Peter Grasch
 */
void WordListManager::safelyInit()
{
	this->wordlist = new WordList();
	this->shadowList = new WordList();
	isTemp = true;
}

/**
 * @brief Reads the Vocab and returns the read Data
 *
 * This method parses the given File and creates a WordList out of it
 *
 * @author Peter Grasch
 *
 * @param lexiconpath
 * Path to the lexicon
 * \param vocabpath
 * Path to the vocabulary
 * \param promptspath
 * Path to the prompts file
 * \param terminals
 * Pointer to the terminallist to fill
 * @return  The parsed WordList
 */
WordList* WordListManager::readWordList ( QString lexiconpath, QString vocabpath, QString promptspath, QStringList &terminals )
{
	Logger::log (QObject::tr("[INF] Lesen der Wörterliste bestehend aus "));
	Logger::log(QObject::tr("[INF] \t\tLexikon: %1,").arg(lexiconpath));
	Logger::log(QObject::tr("[INF] \t\tVocabular: %1,").arg(vocabpath));
	Logger::log(QObject::tr("[INF] \t\tPrompts: %1").arg(promptspath));

	WordList *wordlist = new WordList();
	//read the vocab
	WordList *vocablist = readVocab(vocabpath);
	//read the prompts
// 	TrainingManager *trainManager = new TrainingManager();
	PromptsTable *promptsTable = /*& (trainManager->promptsList); */ trainManager->getPrompts();
// 	PromptsTable *

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
		if (!terminals.contains(term)) terminals.append(term);
		
		probability = this->trainManager->getProbability( name, promptsTable );

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

Word* WordListManager::getWord(QString word, QString pronunciation, QString terminal, bool &isShadowed)
{
	isShadowed = false;
	for (int i=0; i < wordlist->count(); i++)
	{
		if ((wordlist->at(i).getWord() == word) && (wordlist->at(i).getPronunciation() == pronunciation) && (wordlist->at(i).getTerminal() == terminal))
			return (Word*) &(wordlist->at(i));
	}
	isShadowed = true;
	for (int i=0; i < shadowList->count(); i++)
	{
		if ((shadowList->at(i).getWord() == word) && (shadowList->at(i).getPronunciation() == pronunciation) && (shadowList->at(i).getTerminal() == terminal))
			return (Word*) &(shadowList->at(i));
	}
	return 0;
}

bool WordListManager::moveToShadow(Word *w)
{
	int i=0;
	this->trainManager->deleteWord(w);
	while (i < wordlist->count())
	{
		if (&(wordlist->at(i)) == w)
		{
			shadowList->append(wordlist->takeAt(i));
			shadowDirty = mainDirty = true;
			break;
		}
		i++;
	}
	return save();
}

bool WordListManager::deleteCompletely(Word *w, bool shadowed)
{
	//search for every sample that has the word in it and remove it
	//delete the entry in
	//	dict--------------------
	//	shadowdict--------------
	//	voca
	//	prompts-----------------
	
	//TODO: Implement deletion

	if (!shadowed)
	{
		this->trainManager->deleteWord(w); //if the word is shadowed we can't have any
		WordList *main = getWordList();
		for (int i=0; i < main->count(); i++)
			if (&(main->at(i)) == w)
			{
				main->removeAt(i);
				mainDirty=true;
				break; //is unique
			}
		//training samples
	}
	else 
	{
		WordList *shadow = getShadowList();
		for (int i=0; i < shadow->count(); i++)
			if (&(shadow->at(i)) == w)
			{
				shadow->removeAt(i);
				shadowDirty=true;
				break; //is unique
			}
	}
	
	return save();
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
	{ i++; }
	if ((i<wordcount) && (wlist->at(i).getWord().toUpper() == uppername) && 
			( wlist->at( i ).getPronunciation() == pronunciation))
	{
		//we found the word!
		return  new QString(((wlist->at( i ).getTerminal())));
	}
	// there was no result
	return NULL;
}

/**
 * \brief Returns a random wordname using this terminal
 * @param terminal The terminal of the word
 * @author Peter Grasch
 * @return Wordname
 */
QString WordListManager::getRandomWord(QString terminal)
{
	WordList *main = getWordList();
	int start = qrand()%main->count();
	int i=start;
	while (i<main->count())
	{
		if (main->at(i).getTerminal()==terminal)
			return main->at(i).getWord();
		i++;
	}
	//we didn't find anything till now
	//start again at the beginning and go till the point we started last time
	i=0;
	while (i<start)
	{
		if (main->at(i).getTerminal()==terminal)
			return main->at(i).getWord();
		i++;
	}
	
	//still didn't find anything?
	//move on to the shadowlist
	WordList *shadowList = getShadowList();
	
	start = qrand()%shadowList->count();
	i=start;
	while (i<shadowList->count())
	{
		if (shadowList->at(i).getTerminal()==terminal)
			return shadowList->at(i).getWord();
		i++;
	}
	//we didn't find anything till now
	//start again at the beginning and go till the point we started last time
	i=0;
	while (i<start)
	{
		if (shadowList->at(i).getTerminal()==terminal)
			return shadowList->at(i).getWord();
		i++;
	}
	return terminal; //we couldn't find a word - return what we got
}

QStringList WordListManager::getTerminals(bool includeShadow)
{
	QStringList terminals;
	
	//main
	terminals << this->activeTerminals;
	if (includeShadow && (getShadowList() /* to wait if the thread is running */))
	{
		for (int i=0; i < shadowTerminals.count(); i++)
			if (!terminals.contains(shadowTerminals.at(i)))
				terminals.append(shadowTerminals.at(i));
	}
	terminals.sort();
	return terminals;
}

WordList* WordListManager::getWords(QString word, bool includeShadow)
{
	WordList* main = getWordList();
	WordList* found = new WordList();
	int i=0;
	QString toSearch = word.toUpper();
	//main
	if (main)
		while (i<main->count())
		{
			if (main->at(i).getWord().toUpper()==toSearch)
				found->append(main->at(i));
			i++;
		}
	if (!includeShadow)
		return found;
	WordList* shadow = getShadowList();
// 	//shadow
	i=0;
	if (shadow)
		while (i<shadow->count())
		{
			if (shadow->at(i).getWord().toUpper()==toSearch)
			{
				found->append(shadow->at(i));
			}
			i++;
		}
	return found;
}

/**
 * \brief Adds the words to the local wordlist
 * Inserts the words into the current list (alphabetically)
 * Tries to omit duplicates...
 * \author Peter Grasch
 * \param WordList *list
 * List of words to add (DANGER: this pointer may be invalid after calling this function!)
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

	if (list->count() < target->count())
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
		} else newList->removeAt(0); //remove the double
	}

	delete newList;

	if (shadow) this->shadowList = main;
	else this->wordlist = main;

	Logger::log(QObject::tr("[INF] Die Wortliste beinhaltet jetzt %1 Wörter").arg(wordlist->count()));
	
	if (shadow) shadowDirty=true;
	else mainDirty=true;
	
	this->save();
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
		if (line.trimmed().isEmpty()) continue;
		
		if (line.startsWith("%")) 
		{
			//The Line is the Definition of the terminal
			terminal = line.mid(2).trimmed();
			continue;
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
