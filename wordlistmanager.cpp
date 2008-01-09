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
#include <QtGlobal>
#include <QTextStream>
#include "modelmanager.h"
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
	this->modelManager = new ModelManager();
	this->trainManager = trainManager;
	isTemp = false;

	this->trainManager->getPrompts();
	wordListLock.lock();
	mainDirty = false;
	this->wordlist = readWordList ( Settings::getS("Model/PathToLexicon"), Settings::getS("Model/PathToVocab"), Settings::getS("Model/PathToPrompts"), this->activeTerminals );
	wordListLock.unlock();

	start();
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
	shadowLock.lock();
	shadowDirty = false;
	this->shadowList = readWordList(Settings::getS("Model/PathToShadowLexicon"), Settings::getS("Model/PathToShadowVocab"), Settings::getS("Model/PathToPrompts"), this->shadowTerminals);
	shadowLock.unlock();
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
//FIXME: We can't show this directly!
//	If this method is called by a different thread (this WILL happen),
//	we would create the simonInfo object (which contains gui-widgets) in that
//	threads context
//	This will result in an async-error from xlib (you can't have different threads painting the gui)
//	and therefore result in a crash;
//	
// 		SimonInfo::showMessage(tr("Bitte warten Sie während das Schatten-Wörterbuch lädt..."),1000);
// 		QCoreApplication::processEvents();
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
		emit tempWarning();
	}

	Logger::log(QObject::tr("[INF] Speichere Wörterliste"));
	
	//save wordlist
	wordListLock.lock();
	if (mainDirty)	//we changed the wordlist
	{
		if (lexiconFilename.isEmpty()) lexiconFilename = Settings::getS("Model/PathToLexicon");
		if (vocabFilename.isEmpty()) vocabFilename = Settings::getS("Model/PathToVocab");
		saveWordList(this->getWordList(), lexiconFilename, vocabFilename);
		emit wordlistChanged();
		mainDirty=false;
	}
	wordListLock.unlock();
	
	//shadowlist
	shadowLock.lock();
	if (shadowDirty) //we changed the shadowDict
	{
		if (shadowLexiconFilename.isEmpty()) shadowLexiconFilename = Settings::getS("Model/PathToShadowLexicon");
		if (shadowVocabFilename.isEmpty()) shadowVocabFilename = Settings::getS("Model/PathToShadowVocab");
		saveWordList(this->getShadowList(), shadowLexiconFilename, shadowVocabFilename);
		emit shadowListChanged();
		shadowDirty=false;
	}
	shadowLock.unlock();
	
	
	return true;
}


/**
 * \brief Saves the given wordlist to the given files
 * \author Peter Grasch
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


	QFile *vocabFile = new QFile(vocabFilename);
	if (!vocabFile->open(QIODevice::WriteOnly)) {
		Logger::log(QObject::tr("[ERR] Fehler beim öffnen der Ausgabedatei %1").arg(vocabFilename));
		return false;
	}
	QTextStream vocab(vocabFile);
	vocab.setCodec("ISO 8859-15");

	//print internal sentence structure
	// 	% NS_B
	// 	<s>	sil
	// 
	// 	% NS_E
	// 	</s>	sil
	vocab << "% NS_B\n<s>\tsil\n";
	vocab << "% NS_E\n</s>\tsil\n";

	vocab << "% " << tr("Unbekannt") << "\n";

	QHash<QString /*terminalName*/, Word /*words*/> vocabulary;
	
	int i=0;
	int count = list->count();
	bool sentWritten = false; //performance
	while (i<count)
	{
		Word w = list->at(i);
		QString wordStr = w.getWord();
		QString upperWord = wordStr.toUpper();

		if (!sentWritten && (upperWord >= "SENT-END"))
		{
			outstream << "SENT-END\t\t[]\t\tsil\n";
			outstream << "SENT-START\t\t[]\t\tsil\n";
			sentWritten = true;
		}

		QString wordPron = w.getPronunciation();
		QString wordTerm = w.getTerminal();

		outstream << upperWord << "\t\t[" << wordStr << "]\t\t" <<
				wordPron << "\n";

		vocabulary.insertMulti(wordTerm, w);
		i++;
	}
	outfile->close();

	QStringList terminals = vocabulary.keys();
	QStringList distinctTerminals;
	while (terminals.count())
		if (!distinctTerminals.contains(terminals[0]))
			distinctTerminals << terminals.takeAt(0);
		else terminals.removeAt(0);

	
	for (int i=0; i < distinctTerminals.count(); i++)
	{
		vocab << "% " << distinctTerminals[i] << "\n";
		WordList currentWordList = vocabulary.values(distinctTerminals[i]);
		int wordCount = currentWordList.count();
		for (int j=0; j < wordCount; j++)
		{
			Word w = currentWordList.at(j);
			vocab << w.getWord() << "\t" << w.getPronunciation() << "\n";
		}
	}

	Logger::log(QObject::tr("[INF] Schießen der Ausgabedatei"));
	vocabFile->close();
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
 * \todo this is terribly slow with a large vocab file
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
// 	WordList *vocablist = readVocab(vocabpath);

	PromptsTable *promptsTable = trainManager->getPrompts();

	//opening
	Logger::log(QObject::tr("[INF] Öffnen des Lexikons von: %1").arg(lexiconpath));
	QFile *lexicon = new QFile ( lexiconpath );
	QFile vocab(vocabpath);
	if ( !lexicon->open ( QFile::ReadOnly ) || !vocab.open(QFile::ReadOnly) || !promptsTable) return false;
	

	QString line, term, word;
	QString pronunciation;
	int splitter;
	while (!vocab.atEnd())
	{
		line = QString(vocab.readLine(1024)).trimmed();
		if (line.startsWith("% "))
		{
			//its a new terminal!
			term = line.mid(2).trimmed();
			//strip multiple definitions
			if (!terminals.contains(term)) terminals.append(term);
		} else
		{
			//read the word
			splitter = line.indexOf("\t");
			word = line.left(splitter).trimmed();
			if (word.isEmpty()) continue;
			pronunciation = line.mid(splitter).trimmed();
			
			wordlist->append(Word(word, pronunciation, term, trainManager->getProbability(word.toUpper())));
		}
	}

/*
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
		term = (termtmp) ? *termtmp : tr("Unbekannt");
		if (!terminals.contains(term)) terminals.append(term);
		
		probability = this->trainManager->getProbability( name, promptsTable );

		//creates and appends the word to the wordlist
		wordlist->append(Word(output, pronunciation, term, probability));
	}*/

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
	wordListLock.lock();
	for (int i=0; i < wordlist->count(); i++)
	{
		if ((wordlist->at(i).getWord() == word) && (wordlist->at(i).getPronunciation() == pronunciation) && (wordlist->at(i).getTerminal() == terminal))
			return (Word*) &(wordlist->at(i));
	}
	wordListLock.unlock();

	isShadowed = true;
	shadowLock.lock();
	for (int i=0; i < shadowList->count(); i++)
	{
		if ((shadowList->at(i).getWord() == word) && (shadowList->at(i).getPronunciation() == pronunciation) && (shadowList->at(i).getTerminal() == terminal))
			return (Word*) &(shadowList->at(i));
	}
	shadowLock.unlock();
	return 0;
}

/**
 * \brief Moves the given word to the shadowlist
 * \author Peter Grasch
 * \todo THEORETICALLY this code (we must lock the shadowlist AND the main wordlist) might yield to a deadlock
 * @param w The given word
 * @return success (i.e. the file is not found)
 */
bool WordListManager::moveToShadow(Word *w)
{
	int i=0;
	this->trainManager->deleteWord(w);
	shadowLock.lock();
	while (i < wordlist->count())
	{
		if (&(wordlist->at(i)) == w)
		{
			wordListLock.lock();
			shadowList->append(wordlist->takeAt(i));
			wordListLock.unlock();
			shadowDirty = mainDirty = true;
			break;
		}
		i++;
	}
	shadowLock.unlock();
	return save();
}

bool WordListManager::deleteCompletely(Word *w, bool shadowed)
{
	//search for every sample that has the word in it and remove it
	//delete the entry in
	//	dict
	//	shadowdict
	//	voca
	//	prompts
	
	//TODO: Implement deletion


	
	if (!shadowed)
	{
		this->trainManager->deleteWord(w); //if the word is shadowed we can't have any
		wordListLock.lock();
		WordList *main = getWordList();
		for (int i=0; i < main->count(); i++)
			if (&(main->at(i)) == w)
			{
				main->removeAt(i);
				mainDirty=true;
				break; //is unique
			}

		wordListLock.unlock();
		//training samples
	}
	else 
	{
		shadowLock.lock();
		WordList *shadow = getShadowList();
		for (int i=0; i < shadow->count(); i++)
			if (&(shadow->at(i)) == w)
			{
				shadow->removeAt(i);
				shadowDirty=true;
				break; //is unique
			}
		shadowLock.unlock();
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
	wordListLock.lock();
	WordList *main = getWordList();
	int start = qrand()%main->count();
	int i=start;
	while (i<main->count())
	{
		if (main->at(i).getTerminal()==terminal)
		{
			wordListLock.unlock();
			return main->at(i).getWord();
		}
		i++;
	}
	//we didn't find anything till now
	//start again at the beginning and go till the point we started last time
	i=0;
	while (i<start)
	{
		if (main->at(i).getTerminal()==terminal)
		{
			wordListLock.unlock();
			return main->at(i).getWord();
		}
		i++;
	}
	wordListLock.unlock();
	

	shadowLock.lock();
	//still didn't find anything?
	//move on to the shadowlist
	WordList *shadowList = getShadowList();
	
	start = qrand()%shadowList->count();
	i=start;
	while (i<shadowList->count())
	{
		if (shadowList->at(i).getTerminal()==terminal)
		{
			shadowLock.unlock();
			return shadowList->at(i).getWord();
		}
		i++;
	}
	//we didn't find anything till now
	//start again at the beginning and go till the point we started last time
	i=0;
	while (i<start)
	{
		if (shadowList->at(i).getTerminal()==terminal)
		{
			shadowLock.unlock();
			return shadowList->at(i).getWord();
		}
		i++;
	}
	shadowLock.unlock();

	return terminal; //we couldn't find a word - return what we got
}

QStringList WordListManager::getTerminals(bool includeShadow)
{
	QStringList terminals;
	
	//main
	terminals << this->activeTerminals;
	if (includeShadow)
	{
		shadowLock.lock();
		getShadowList(); /* to wait if the thread is running */
		shadowLock.unlock();

		//merge the list
		for (int i=0; i < shadowTerminals.count(); i++)
			if (!terminals.contains(shadowTerminals.at(i)))
				terminals.append(shadowTerminals.at(i));
	}
	terminals.sort();


	return terminals;
}

WordList* WordListManager::getWords(QString word, bool includeShadow)
{
	WordList* found = new WordList();
	int i=0;
	QString toSearch = word.toUpper();

	wordListLock.lock();
    
	WordList* main = getWordList();
	//main
	if (main)
    {
		while (i<main->count())
		{
			if (main->at(i).getWord().toUpper()==toSearch)
				found->append(main->at(i));
			i++;
		}
    }
	wordListLock.unlock();
	if (!includeShadow)
		return found;
	
	i=0;
	shadowLock.lock();
	WordList* shadow = getShadowList();
	if (shadow)
		while (i<shadow->count())
		{
			if (shadow->at(i).getWord().toUpper()==toSearch)
			{
				found->append(shadow->at(i));
			}
			i++;
		}
	shadowLock.unlock();
	return found;
}

void WordListManager::renameTerminal(QString from, QString to, bool includeShadow)
{
	if (to == from) return;
	int i=0;
	wordListLock.lock();
	WordList *main = getWordList();
	while (i < main->count())
	{
		if (main->at(i).getTerminal()==from)
		{
			//discard the const-qualifier
			Word w = ((Word) main->at(i));
			w.setTerminal(to);
			main->replace(i, w);
			mainDirty = true;
		}
		i++;
	}
	wordListLock.unlock();

	if (!includeShadow) return;

	//shadow
	i=0;
	shadowLock.lock();
	WordList *shadow = getShadowList();
	while (i < shadow->count())
	{
		if (shadow->at(i).getTerminal()==from)
		{
			Word w = ((Word) shadow->at(i));
			w.setTerminal(to);
			shadow->replace(i, w);
			shadowDirty = true;
		}
		i++;
	}
	shadowLock.unlock();
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
	if (shadow)
	{
		shadowLock.lock();
		 target = getShadowList();
	} else {
		wordListLock.lock();
		target = getWordList();
	}
		

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

	if (shadow) {
		this->shadowList = main;
		shadowDirty=true;
		shadowLock.unlock();
	} else {
		this->wordlist = main;
		mainDirty=true;
		wordListLock.unlock();
	}

	Logger::log(QObject::tr("[INF] Die Wortliste beinhaltet jetzt %1 Wörter").arg(wordlist->count()));
	
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


	if (!vocab->atEnd()) 
	{
		line = vocab->readLine(1024);
	
		//skip till the next terminal definition
		while (!vocab->atEnd() && (!line.startsWith("% ")))
		{ line = vocab->readLine(1024);  }
		
		//set the terminal to this one if there is one (i.e. the file was not over)
		if (!vocab->atEnd())
			terminal = line.mid(2).trimmed();
	}
	


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
 * \brief adds the new samples to the prompts
 * \author Susanne Tschernegg
 */
 void WordListManager::writePrompts(QString line, QString promptspath)
{
    QFile *prompts = new QFile ( promptspath );
	prompts->open ( QIODevice::Append );
	//if ( !prompts->isWritable() ) return;
	
    QTextStream out(prompts);
    out << line << "\n";
    prompts->close();
    readPrompts();
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
 * \brief Destructor
 * \author Peter Grasch
 */
WordListManager::~WordListManager()
{}
