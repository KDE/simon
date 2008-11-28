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


#include "wordlistmanager.h"
#include "trainingmanager.h"
#include "grammarmanager.h"
#include <simonlogging/logger.h>

#include <simonprogresstracking/operation.h>
#include <simonprogresstracking/statusmanager.h>

#include <QObject>
#include <QList>
#include <QFile>
#include <QByteArray>
#include <QtGlobal>
#include <QTextStream>
#include <QDateTime>

#include <KConfig>
#include <KConfigGroup>
#include <KMessageBox>
#include <KLocalizedString>
#include <KStandardDirs>
#include <KFilterDev>
#include <KMimeType>
#include <KDebug>
#include <KLocale>

WordListManager* WordListManager::instance;

/**
 * @brief Constructor
 *
 * Reads the vocab and initializes the wordlist (member) with it
 *
 * @author Peter Grasch
 * @param QString path
 * Sets the path (member) to the given string
 */
WordListManager::WordListManager () : QThread(), wordListLock(QMutex::Recursive), shadowLock(QMutex::Recursive)
{
	KLocale::setMainCatalog("simonlib");
	isTemp = false;
	connect(this, SIGNAL(wordListCouldntBeLoaded()), this, SLOT(complainAboutPaths()));
	connect(this, SIGNAL(shadowListCouldntBeLoaded()), this, SLOT(complainAboutPaths()));
	connect(this, SIGNAL(tempWarning()), this, SLOT(warnAboutTempWordList()));
	connect(TrainingManager::getInstance(), SIGNAL(trainingDataChanged()), this, SLOT(updateWordProbability()));

	initWordList();
	initShadowList();
}

bool WordListManager::initShadowList()
{
	start(QThread::IdlePriority);
	return true;
}

bool WordListManager::initWordList()
{
	QMutexLocker lock(&wordListLock);
	mainDirty = false;
	this->wordlist = readWordList ( KStandardDirs::locate("appdata", "model/lexicon"), 
					KStandardDirs::locate("appdata", "model/model.voca"),
					this->activeTerminals );

	if (!wordlist) {
		this->wordlist = new WordList();
		emit wordListCouldntBeLoaded();
		return false;
	}
	
	return true;
}

/**
 * \brief Updates the word probabilyt of the active lexicon
 * \author Peter Grasch
 * \note Usually triggered when the training data has changed
 */
void WordListManager::updateWordProbability()
{
	wordListLock.lock();
	WordList::iterator i= wordlist->begin();
	TrainingManager *trainMan = TrainingManager::getInstance();
	WordList::iterator end = wordlist->end();
	for (; i != end; i++)
	{
		(*i).setProbability(trainMan->getProbability((*i).getWord()));
	}

	wordListLock.unlock();
}

/**
 * \brief Returns the whole list of words using the specified terminal
 * @param terminal The terminal to look for
 * @param includeShadow Should we also look in the shadowdict?
 * @return The wordlist
 */
WordList* WordListManager::getWordsByTerminal(const QString& terminal, bool includeShadow)
{
	wordListLock.lock();
	WordList *list = getWordList();
	WordList *out = new WordList();
	
	WordList::const_iterator i = list->constBegin(); 
	WordList::const_iterator end = list->constEnd();
	for (; i < end; i++)
	{
		if ((*i).getTerminal()==terminal)
			out->append((*i));
	}
	wordListLock.unlock();
	if (!includeShadow) return out;
	
	shadowLock.lock();
	list = getShadowList();
	end = list->constEnd();
	for (i=list->constBegin(); i < end; i++)
	{
		if ((*i).getTerminal()==terminal)
			out->append((*i));
	}
	shadowLock.unlock();
	
	return out;
}

/**
 * \brief Returns a copy of the active dict which only contains words with a terminal that is used in the grammar
 * \author Peter Grasch
 */
WordList* WordListManager::getSimpleVocab()
{
	QStringList usedTerminals = GrammarManager::getInstance()->getTerminals();

	WordList *simpleList = new WordList();
	for (int i=0; i < usedTerminals.count(); i++)
	{
		WordList *tempList = getWordsByTerminal(usedTerminals[i]);
		foreach (Word w, *tempList)
			simpleList->append(w);
		delete tempList;
	}
	
	return simpleList;
}

/**
 * \brief Warns about changing a temporary wordlist (when the path is not correctly configured)
 * \author Peter Grasch
 */
void WordListManager::warnAboutTempWordList()
{
	KMessageBox::information(0, i18n("You are changing a temporary wordlist.\n\nYour changes will NOT be saved!"), i18n("Temporary changes"));
}

void WordListManager::complainAboutPaths()
{
	KMessageBox::error(0, i18n("Couldn't read all the nescessairy files for the wordlist.\n\nA temporary wordlist will be created. Your changes will NOT be saved!"), i18n("Read Error"));
}

WordListManager* WordListManager::getInstance()
{
	if (!instance)
	{
		instance = new WordListManager();
	}
	return instance;
}

/**
 * \brief Starts the importing of the shadow-Model in a new thread
 * \author Peter Grasch
 */
void WordListManager::run()
{
	Operation op(thread(), i18n("Loading Shadow List"), i18n("Parsing..."), 0,0, true);
	shadowLock.lock();
	shadowDirty = false;
	this->shadowList = readWordList(KStandardDirs::locate("appdata", "model/shadowlexicon"),
					KStandardDirs::locate("appdata", "model/shadow.voca"),
					this->shadowTerminals, true);
	if (!shadowList)
	{
		this->shadowList = new WordList();
		emit shadowListCouldntBeLoaded();
		op.canceled();
	} else
	{
		op.finished();
	}
	
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
	Logger::log(i18n("[INF] Sorting a list of %1 words", list->count()));
	qSort(list->begin(), list->end());
	return list;
}


inline WordList* WordListManager::getShadowList()
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
// 		SimonInfo::showMessage(i18n("Bitte warten Sie während das Schatten-Wörterbuch lädt..."),1000);
//		this is actually bug-using as we just set any kind of timeout (the gui thread will be
//		blocked until the importing is done anyway...
// 		QCoreApplication::processEvents();
		wait();
	}
	
	return shadowList;
}

/**
 * \brief Saves the current WordList (member)
 * \author Peter Grasch
 * \param QString filename
 * If not provided we default on the filename we used to open the file
 * \warning If both lists are changed, we will ONLY emit the wordlistChanged signal
 * \return bool
 * Saving successful?
 */
bool WordListManager::save()
{
	bool wlistChanged = false, slistChanged = false;
	if (isTemp){
		emit tempWarning();
	}

	Logger::log(i18n("[INF] Saving wordlist(s)"));
	
	//save wordlist
	wordListLock.lock();
	if (mainDirty)	//we changed the wordlist
	{
		saveWordList(this->getWordList(), 
			      KStandardDirs::locateLocal("appdata", "model/model.voca"),
			      KStandardDirs::locateLocal("appdata", "model/lexicon"));
		wlistChanged = true;
		mainDirty=false;
	}
	wordListLock.unlock();
	
	//shadowlist
	shadowLock.lock();
	if (shadowDirty) //we changed the shadowDict
	{
		saveWordList(this->getShadowList(), 
			      KStandardDirs::locateLocal("appdata", "model/shadow.voca"));
		slistChanged = true;
		shadowDirty=false;
	}
	shadowLock.unlock();
	
	if (wlistChanged) {
		emit wordlistChanged();
		KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
		KConfigGroup cGroup(&config, "");
		cGroup.writeEntry("WordListDate", QDateTime::currentDateTime());
		config.sync();
		
	} else if (slistChanged) {
		emit shadowListChanged();
		KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
		KConfigGroup cGroup(&config, "");
		cGroup.writeEntry("LanguageDescriptionDate", QDateTime::currentDateTime());
		config.sync();
	}
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
bool WordListManager::saveWordList(WordList *list, const QString& vocabFilename, const QString& lexiconFilename)
{
	Logger::log(i18n("[INF] Opening output file: %1", lexiconFilename));
	QIODevice *outfile=NULL;
	QTextStream *outstream=NULL;

	if (!lexiconFilename.isEmpty())
	{
		outfile = KFilterDev::deviceForFile(lexiconFilename,
							KMimeType::findByFileContent(lexiconFilename)->name());
		if (!outfile->open(QIODevice::WriteOnly)) {
			Logger::log(i18n("[ERR] Could not open output file %1", lexiconFilename));
			outfile->deleteLater();
			return false;
		}
		outstream = new QTextStream(outfile);
		//TODO Test encoding
		outstream->setCodec("UTF-8");
	}


	QIODevice *vocabFile = KFilterDev::deviceForFile(vocabFilename,
						KMimeType::findByFileContent(vocabFilename)->name());
	if (!vocabFile->open(QIODevice::WriteOnly)) {
		Logger::log(i18n("[ERR] Could not open output file %1", vocabFilename));
		outfile->close();
		outfile->deleteLater();
		vocabFile->deleteLater();
		return false;
	}
	QTextStream vocab(vocabFile);
	//TODO Test encoding
	vocab.setCodec("UTF-8");

	//print internal sentence structure
	// 	% NS_B
	// 	<s>	sil
	// 
	// 	% NS_E
	// 	</s>	sil
	vocab << "% NS_B\n<s>\tsil\n";
	vocab << "% NS_E\n</s>\tsil\n";

	QHash<QString /*terminalName*/, Word /*words*/> vocabulary;
	
	bool sentWritten = false;
	WordList::const_iterator end = list->constEnd();
	WordList::const_iterator i = list->constBegin();
	QStringList distinctTerminals;
	while (i<end)
	{
		Word w = (*i);
		QString wordStr = w.getWord();
		QString wordTerm = w.getTerminal();
		//TODO: Test naming
		if (!lexiconFilename.isEmpty()) {
			QString upperWord = wordStr.toUpper();
			QString wordPron = w.getPronunciation();
	
			if (!sentWritten && (upperWord >= "SENT-END"))
			{
				*outstream << "SENT-END\t\t[]\t\tsil\n";
				*outstream << "SENT-START\t\t[]\t\tsil\n";
				sentWritten=true;
			}
			*outstream << upperWord /*wordStr*/ << "\t\t[" << wordStr << "]\t\t" <<
					wordPron << "\n";
		}

		vocabulary.insertMulti(wordTerm, w);
		if (!distinctTerminals.contains(wordTerm)) distinctTerminals.append(wordTerm);
		i++;
	}
	if (!lexiconFilename.isEmpty())
	{
		if (!sentWritten)
		{
			*outstream << "SENT-END\t\t[]\t\tsil\n";
			*outstream << "SENT-START\t\t[]\t\tsil\n";
		}
		outstream->flush();
		outfile->close();
		outfile->deleteLater();
		delete outstream;
	}
	
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

	Logger::log(i18n("[INF] Closing output file"));
	vocabFile->close();
	vocabFile->deleteLater();
	return true;
}

/**
 * \brief Binary search for finding the word
 * \author Peter Grasch
 * \param list The list to search
 * \param found Did we find something?
 * \param word The name of the word
 * \param pronunciation SAMPA pronunciation
 * \param terminal The terminal of the word
 * \note This is incredibly fast :)
 * \return The index of the found word; this is set to the nearest hit if the word is not found (see parameter: found)
 */
int WordListManager::getWordIndex(WordList *list, bool &found, const QString& word, const QString& pronunciation, const QString& terminal)
{
	if (!list || (list->count()==0))
	{
		found = false;
		return 0;
	}
	
	QString realWord = word.toUpper();
	
	int currentSearchStart = list->count()/2; //make use of integer division
	//if count() == 1, currentSearchStart = 0,5 = 0 instead of 1 when using round
	//(which would be out of bounds)
	
	int currentMinValue = 0;
	int currentMaxValue = list->count();
	Word *currentWord;
	QString currentWordName, currentWordPronunciation, currentWordTerminal;
	int modificator=0;
	while (true)
	{
		currentWord = (Word*) &(list->at(currentSearchStart));
		currentWordName = currentWord->getWord().toUpper();
		currentWordPronunciation = currentWord->getPronunciation();
		currentWordTerminal = currentWord->getTerminal();

		
		if ((currentWordName==realWord)
			&& ((pronunciation.isEmpty() || currentWordPronunciation == pronunciation)
			&& (terminal.isEmpty() || currentWordTerminal == terminal)))
		{
			//we found the exact word
			found = true;
			return currentSearchStart;
		} else if ((currentWordName < realWord) || 
			((currentWordName == word) && ((!pronunciation.isEmpty() && currentWordPronunciation < pronunciation)
			|| (!terminal.isEmpty() && currentWordTerminal < terminal))))
		{
			currentMinValue = currentSearchStart;
			modificator = (currentMaxValue - currentMinValue)/2;
		} else if ((currentWordName > realWord) || 
			((currentWordName == word) && ((!pronunciation.isEmpty() && currentWordPronunciation > pronunciation)
			|| (!terminal.isEmpty() && currentWordTerminal > terminal))))
		{
			currentMaxValue = currentSearchStart;
			modificator = (currentMaxValue - currentMinValue)/(-2);
		}
		
		
		if (modificator == 0) {
			//stagnating search
			//do a incremental search over the left over items
			int i=currentMinValue;
			while ((i < currentMaxValue) && ((currentWordName < realWord) || 
						     ((currentWordName == word) && ((!pronunciation.isEmpty() && currentWordPronunciation < pronunciation)
						     || (!terminal.isEmpty() && currentWordTerminal < terminal)))))
			{
				i++;
			}
			if ((i<list->count()) && (list->at(i).getWord().toUpper()==realWord)
							&& ((pronunciation.isEmpty() || list->at(i).getPronunciation() == pronunciation)
							&& (terminal.isEmpty() || list->at(i).getTerminal() == terminal)))
			{
				found = true;
			} else found = false;
			return i;
		}
		currentSearchStart += modificator;
	}
	found = false;
	return currentSearchStart;
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
 * \param terminals
 * Pointer to the terminallist to fill
 * \param isShadowlist
 * Points out if this is a shadowlist - then we skip the recognition-check (it will always return 0)
 * @return  The parsed WordList
 */
WordList* WordListManager::readWordList ( const QString& lexiconpath, const QString& vocabpath, QStringList &terminals, bool isShadowlist )
{
	Logger::log (i18n("[INF] Reading wordlist consisting of"));
	Logger::log(i18n("[INF] \t\tLexicon: %1,", lexiconpath));
	Logger::log(i18n("[INF \t\tVocabulary: %1", vocabpath));

	WordList *wordlist = new WordList();
	//read the vocab
	TrainingManager *trainManager = TrainingManager::getInstance();	

	QIODevice *vocab = KFilterDev::deviceForFile(vocabpath,
							KMimeType::findByFileContent(vocabpath)->name());
	if (!vocab->open(QIODevice::ReadOnly)) {
		Logger::log(i18n("[ERR] Erorr when openin the vocabulary file at \"%1\"", vocabpath));
		vocab->deleteLater();
		return false;
	}
	QTextStream vocabStream(vocab);
	vocabStream.setCodec("UTF-8");

	QString line, term, word;
	QString pronunciation;
	int splitter;

	//skip NS_E and NS_B
	for (int i=0; (i < 4) && (!vocabStream.atEnd()); i++)
		vocabStream.readLine(1024);

	line = vocabStream.readLine();
	
	while (!line.isNull())
	{
		line = line.trimmed(); //test if we can do this together with the readLine() without destroying the isNull()
		if (!line.startsWith("% "))
		{
			//read the word
			splitter = line.indexOf("\t");
			word = line.left(splitter).trimmed();
			if (word.isEmpty()) continue;
			pronunciation = line.mid(splitter).trimmed();
			wordlist->append(Word(word, pronunciation, term, (isShadowlist) ? 0 : trainManager->getProbability(word.toUpper())));
		} else
		{
			//its a new terminal!
			term = line.mid(2).trimmed();
			//strip multiple definitions
			if (!terminals.contains(term)) terminals.append(term);
		}
		line = vocabStream.readLine();
	}
	wordlist = this->sortList(wordlist);

	Logger::log(i18n("[INF] Wordlist created"));

	vocab->close();
	vocab->deleteLater();
	return wordlist;
}


bool WordListManager::refreshWordListFiles(const QByteArray& simpleVocab,
		const QByteArray& activeVocab, const QByteArray& activeLexicon)
{
	QMutexLocker lock1(&wordListLock);
	
	Q_UNUSED(simpleVocab);
	
	QFile vocaF(KStandardDirs::locateLocal("appdata", "model/model.voca"));
	QFile wordListF(KStandardDirs::locateLocal("appdata", "model/lexicon"));
	if (!vocaF.open(QIODevice::WriteOnly) || !wordListF.open(QIODevice::WriteOnly))
		return false;
	
	vocaF.write(activeVocab);
	wordListF.write(activeLexicon);
	
	vocaF.close();
	wordListF.close();
	
	if (!initWordList()) return false;
	
	emit wordlistChanged();
	return true;
}


bool WordListManager::refreshShadowListFiles(const QByteArray& shadowVocab)
{
	QMutexLocker lock2(&shadowLock);
	
	QFile vocaF(KStandardDirs::locateLocal("appdata", "model/shadow.voca"));
	if (!vocaF.open(QIODevice::WriteOnly))
		return false;
	
	vocaF.write(shadowVocab);
	
	vocaF.close();
	
	if (!initShadowList()) return false;
	
	emit shadowListChanged();
	return true;
}

/**
 * \brief [DEPRECATED] Removes the doubles in the supplied input list
 * \author Peter Grasch
 * @param in 
 * @return 
 */
WordList* WordListManager::removeDoubles(WordList *in)
{
	if (!in) return NULL;
	
	Logger::log(i18n("[INF] Removing doubles"));
	
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
 * \brief Gets the given word and returns a pointer to it (NULL if not found)
 * \note Uses the _fast_ binary search of getWordIndex()
 * @param word Name of the word
 * @param pronunciation Pronunciation of the word
 * @param terminal The terminal of the word
 * @param isShadowed If the word is shadowed (reference parameter)
 * @return The word (null if not found)
 */
Word* WordListManager::getWord(const QString& word, const QString& pronunciation, const QString& terminal, bool &isShadowed)
{
	Word *w=NULL;
	isShadowed = false;
	wordListLock.lock();
	bool found;
	WordList *wList = getWordList();
	int wIndex = getWordIndex(wList, found, word, pronunciation, terminal);
	if (found)
	{
		w = (Word*) &(wList->at(wIndex));
	}
	wordListLock.unlock();
	if (w)
		return w;

	isShadowed = true;
	shadowLock.lock();
	wList = getShadowList();
	wIndex = getWordIndex(wList, found, word, pronunciation, terminal);
	if (found)
	{
		w = (Word*) &(wList->at(wIndex));
	}
	shadowLock.unlock();

	return w;
}

/**
 * \brief Moves the given word to the shadowlist
 * \author Peter Grasch
 * \warning THEORETICALLY this code (we must lock the shadowlist AND the main wordlist) might yield to a deadlock
 * \note uses binary search to determine where exactly to insert the new (old) word
 * @param w The given word
 * @return success (i.e. the file is not found); (this may also be false due to an error when deleting the prompts for the word!)
 */
bool WordListManager::moveToShadow(Word *w)
{
	int i=0;
	if (!w) return false;
	TrainingManager::getInstance()->deleteWord(w);
	shadowLock.lock();
	wordListLock.lock();
	while (i < wordlist->count())
	{
		if (&(wordlist->at(i)) == w)
		{
			Word w = wordlist->takeAt(i);
			w.setProbability(0);
			bool found;
			int index = getWordIndex(shadowList, found, w.getWord(), w.getPronunciation(), w.getTerminal());
			if (found) {
				KMessageBox::information(0, i18n("This word already exists in the shadow lexicon. It will not be inserted again."));
			} else
				shadowList->insert(index, w);
				
			shadowDirty = mainDirty = true;
			break;
		}
		i++;
	}
	wordListLock.unlock();
	shadowLock.unlock();
	return save();
}

bool WordListManager::deleteCompletely(Word *w, bool shadowed)
{
	if (!w) return false;
	//search for every sample that has the word in it and remove it
	//delete the entry in
	//	dict
	//	shadowdict
	//	voca
	//	prompts

	if (!shadowed)
	{
		TrainingManager::getInstance()->deleteWord(w); //if the word is shadowed we can't have any
		wordListLock.lock();
		WordList *main = getWordList();
		int end = main->count();
		for (int i=0; i < end; i++)
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
		int end = shadow->count();
		for (int i=0; i < end; i++)
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
QString* WordListManager::getTerminal(const QString& name, const QString& pronunciation, WordList *wlist)
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
 * @param includeShadow Should we also look in the shadowdict?
 * @author Peter Grasch
 * @return Wordname
 */
QString WordListManager::getRandomWord(const QString& terminal, bool includeShadow)
{
	wordListLock.lock();
	WordList *main = getWordList();
	int start;
	int i;
	int end;
	if (main->count() > 0)
	{
		start = qrand()%main->count();
		i=start;
		end = main->count();
		while (i<end)
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
	}
	wordListLock.unlock();
	
	if (!includeShadow) return "";
	
	shadowLock.lock();
	//still didn't find anything?
	//move on to the shadowlist
	WordList *shadowList = getShadowList();
	
	if (shadowList->count() > 0)
	{
		start = qrand()%shadowList->count();
		i=start;
		end = shadowList->count();
		while (i<end)
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
	}
	shadowLock.unlock();
	
	return ""; //we couldn't find a word - return what we got
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




/**
 * \brief Merges the lists
 * \warning The input pointer might become invalid after calling this function
 * @param a List a
 * @param b List b
 * @return The merged list
 */
WordList* WordListManager::mergeLists(WordList *a, WordList *b, bool keepDoubles)
{
	if (!a) return b;
	if (!b) return a;
	

	WordList *target;
	WordList *source;
	
	if (a->count() > b->count()) // a bigger than b
	{
		target = a;
		source = b;
	} else {
		target = b;
		source = a;
	}
	
	int wCount = source->count();
	for (int i=0; i < wCount; i++)
	{
		bool found;
		Word currentWord = source->at(i);
		int index = getWordIndex(target, found, currentWord.getWord(), currentWord.getPronunciation(),
					 currentWord.getTerminal());
		if (keepDoubles || !found)
			target->insert(index, currentWord);
	}
	
	delete source;
	return target;
}


WordList* WordListManager::getWords(const QString& word, bool includeShadow, bool fuzzy, bool keepDoubles)
{
	WordList *out;
	
	out = getMainstreamWords(word, fuzzy);
	
	if (!includeShadow) return out;

	return this->mergeLists(getShadowedWords(word, fuzzy), out, keepDoubles);
}


WordList* WordListManager::searchForWords(WordList *list, const QString& word, bool fuzzy)
{
	bool found;
	WordList *out = new WordList();
	if (!list || list->isEmpty()) return out;
	
	if (!fuzzy)		// great! we can perform a binary search
	{
		int indexOfWord = getWordIndex(list, found, word);
		if (!found) return out;
		
		//go up and down around the found index and add all matching words
		int i=indexOfWord;
		while ((i >= 0) && (list->at(i).getWord().toUpper() == word.toUpper()))
		{
			out->append(list->at(i));
			i--;
		}
		i=indexOfWord+1;
		while ((i < list->count()) && (list->at(i).getWord().toUpper() == word.toUpper()))
		{
			out->append(list->at(i));
			i++;
		}
	} else { //nope - incremental only :(
		
		WordList::const_iterator i = list->constBegin();
		WordList::const_iterator end = list->constEnd();
		
		
		if (word.isEmpty()) { //copy everything
			for ( ;i < end; i++)
				out->append(*i);
		} else
			for ( ; i < end; i++)
				if ((*i).getWord().contains(word, Qt::CaseInsensitive))
					out->append((*i));
	}
	
	return out;
}


bool WordListManager::mainWordListContains(Word *word)
{
	QMutexLocker m(&wordListLock);
	return wordListContains(getWordList(), word);
}

bool WordListManager::extraListContains(Word *word)
{
	QMutexLocker m(&shadowLock);
	return wordListContains(getShadowList(), word);
}


bool WordListManager::mainWordListContainsStr(const QString& word)
{
	QMutexLocker m(&wordListLock);
	return wordListContainsStr(getWordList(), word);
}

bool WordListManager::extraListContainsStr(const QString& word)
{
	QMutexLocker m(&shadowLock);
	return wordListContainsStr(getShadowList(), word);
}

bool WordListManager::wordListContains(WordList *list, Word *word)
{
	Q_ASSERT(list);

	int i=0;
	int count = list->count();
	
	while ((i<count) && (list->at(i) != *(word)))
		i++;
	
	return (i!=count) /*did we go all the way through?*/;
}

bool WordListManager::wordListContainsStr(WordList *list, const QString& word)
{
	Q_ASSERT(list);

	int i=0;
	int count = list->count();
	while ((i<count) && (list->at(i).getWord() != word))
		i++;
	
	return (i!=count) /*did we go all the way through?*/;
}

WordList* WordListManager::getMainstreamWords(const QString& word, bool fuzzy)
{
	wordListLock.lock();
	WordList* found = searchForWords(getWordList(), word, fuzzy);
	wordListLock.unlock();
	return found;
}

WordList* WordListManager::getShadowedWords(const QString& word, bool fuzzy)
{
	shadowLock.lock();
	WordList* found = searchForWords(getShadowList(), word, fuzzy);
	shadowLock.unlock();
	return found;
}

void WordListManager::renameTerminal(const QString& from, const QString& to, bool includeShadow)
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
	activeTerminals.replaceInStrings(QRegExp("^"+from+"$"), to);

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
	shadowTerminals.replaceInStrings(QRegExp("^"+from+"$"), to);
}

/**
 * \brief Adds the words to the local wordlist
 * Inserts the words into the current list (alphabetically)
 * Tries to omit duplicates...
 * 
 * This actually doesn't use binary search to look up the correct position where to insert the new words,
 * but rather a "smart" linear search:
 *  * We know that both the wordlists are sorted and start from the previous insertion spot
 * This is in the common case (large new voc; small oldvoc;) even faster.
 * 
 * \author Peter Grasch
 * \param WordList *list
 * List of words to add (DANGER: this pointer may be invalid after calling this function!)
 */
void WordListManager::addWords(WordList *list, bool isSorted, bool shadow)
{
	if (isRunning()) wait();

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
		

	Logger::log(i18n("[INF] Adding %1 words to the wordlist", list->count()));

	if (shadow)
	{
		WordList::const_iterator end = list->constEnd();
		for (WordList::const_iterator i = list->constBegin(); i < end; i++)
		{
			if (!shadowTerminals.contains((*i).getTerminal()))
				shadowTerminals<< (*i).getTerminal();
		}
	}else
	{
		WordList::const_iterator lastElem = list->constEnd();
		for (WordList::const_iterator i = list->constBegin(); i < lastElem; i++)
		{
			if (!activeTerminals.contains((*i).getTerminal()))
				activeTerminals << (*i).getTerminal();
		}
	}

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
	
	while ((newList->count() >0) && (i < wordcount))
	{
		Word tmp=newList->at(0);
		while ((i<wordcount) && (main->at(i) < tmp))
		{ i++; }
		if (i!=wordcount) 
		{
			if (main->at(i) != tmp) //no doubles
			{
				main->insert(i, tmp);
				wordcount++;
			} else newList->removeAt(0); //removeAt the double
		}
	}
	while (newList->count() > 0)
	{
		main->append(newList->takeAt(0));
		wordcount++;
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

	Logger::log(i18n("[INF] The wordlist now contains %1 words", wordlist->count()));
	
	this->save();
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
WordListManager::~WordListManager()
{
    delete wordlist;
    delete shadowList;
}
