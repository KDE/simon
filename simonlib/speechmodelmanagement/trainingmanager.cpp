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


#include "trainingmanager.h"

#include <simonlogging/logger.h>
#include "wordlistmanager.h"
#include "speechmodelmanagementconfiguration.h"
#include "xmltrainingtext.h"

#include <QFile>
#include <QDir>
#include <QStringList>
#include <QString>
#include <QObject>
#include <QDate>
#include <QTextStream>
#include <QVariant>
#include <QTime>
#include <QRegExp>
#include <KLocalizedString>
#include <KStandardDirs>
#include <QDebug>
#include <math.h>


TrainingManager* TrainingManager::instance;

/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 */
TrainingManager::TrainingManager(QObject *parent) : QObject(parent), promptsLock(QMutex::Recursive)
{
	trainingTexts = 0;
	promptsTable=0;
// 	connect(ModelManager::getInstance(), SIGNAL(sampleWithoutWord(QString)), this, SLOT(askDeleteLonelySample(QString)));
}

bool TrainingManager::init()
{
	return initPrompts();
}

bool TrainingManager::initPrompts()
{
	QMutexLocker lock(&promptsLock);
	PromptsTable *promptsTable = readPrompts ( KStandardDirs::locate("appdata", "model/prompts") );
	if (promptsTable) {
		this->promptsTable = promptsTable;
		return true;
	} else return false;
}

void TrainingManager::trainingSettingsSaved()
{
	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("TrainingDate", QDateTime::currentDateTime());
	config.sync();
	
	emit trainingSettingsChanged();
}

TrainingManager* TrainingManager::getInstance()
{
	if (!instance)
		instance = new TrainingManager();
	return instance;
}

/**
 * \brief Asks to delete the sample which has no entry in the prompts-table
 * \author Peter Grasch
 * @param  sample The sample to delete
 */

void TrainingManager::askDeleteLonelySample(QString sample)
{
// 	if (KMessageBox::questionYesNoCancel(0, i18n("Die Datei %1 hat keine Transkription.\n\nWollen Sie sie löschen?", sample), i18n("Herrenloses Sample")) == KMessageBox::Yes)
// 	{
// 		if (!QFile::remove(sample))
// 			KMessageBox::error(0, i18n("Das Löschen des Samples ist fehlgeschlagen"), i18n("Löschen fehlgeschlagen"));
// 		else ModelManager::compileModel(); //start again
// 	}
}

/**
 * \brief Deletes the samples containing the given word
 * \author Peter Grasch
 * This will normally be used to delete a word from the language model (because no sample can contain a word
 * not in the model)
 * @param w The word to remove
 * @param recompiledLater If this flag is set, we will not prompt the user to recompile the model
 * @return Success
 */
bool TrainingManager::deleteWord ( Word *w, bool recompiledLater )
{
	if (!promptsTable) initPrompts();
	
	QString wordToDelete = w->getWord().toUpper();

	QMutexLocker lock(&promptsLock);
	
	QStringList sampleFileNames = promptsTable->keys();
	bool sampleAlreadyDeleted;
	for ( int i=0; i < this->promptsTable->count(); i++ )
	{
		QString filename = sampleFileNames[i];
		QStringList promptWords = promptsTable->value ( filename ).split ( " " );
		sampleAlreadyDeleted = false;
		for ( int j=0; j < promptWords.count() && !sampleAlreadyDeleted; j++ )
		{
			if ( promptWords[j].toUpper() == wordToDelete )
			{
				if ( !deletePrompt ( filename ) )
				{
					promptsLock.unlock();
					return false;
				}
				sampleAlreadyDeleted = true;
			}
		}
	}
	promptsLock.unlock();
	return savePrompts(recompiledLater);
}

/**
 * \brief Deletes the prompt corresponding to the key
 * \author Peter Grasch
 * @param key The key to delete
 * @return success
 */
bool TrainingManager::deletePrompt ( QString key )
{
	if (!promptsTable) initPrompts();
	
	QMutexLocker lock(&promptsLock);
	promptsTable->remove ( key );
	//removes the sample
	return QFile::remove ( SpeechModelManagementConfiguration::modelTrainingsDataPath().path()+"/"+key+".wav" );
}

QString TrainingManager::getTrainingDir()
{
	return SpeechModelManagementConfiguration::modelTrainingsDataPath().path();
}

/**
 * \brief Saves the current promptstable
 * \author Peter Grasch
 * @return Success
 */
bool TrainingManager::savePrompts(bool recompiledLater)
{
	QMutexLocker lock(&promptsLock);
	if (!writePromptsFile(getPrompts(), KStandardDirs::locateLocal("appdata", "model/prompts"))) return false;

	emit trainingDataChanged();
	if (recompiledLater) return true;

// 	if (KMessageBox::questionYesNoCancel(0, i18n("Die Trainingsdaten wurden geändert.\n\nWollen Sie das Sprachmodell jetzt neu kompilieren?"), i18n("Trainingsdaten geändert")) == KMessageBox::Yes)
// 		ModelManager::compileModel();
	return true;
}

bool TrainingManager::writePromptsFile(PromptsTable* prompts, QString path)
{
	if (!promptsTable) initPrompts();
	
	QFile promptsFile ( path );
	if ( !promptsFile.open ( QIODevice::WriteOnly ) ) return false;

	QStringList samples = this->promptsTable->keys();

	for ( int i=0; i <samples.count(); i++ )
		promptsFile.write ( samples[i].toUtf8() +" "+prompts->value ( samples[i] ).toUtf8() +"\n" );
	promptsFile.close();
	
	kDebug() << "schreibe datum...";
	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("TrainingDate", QDateTime::currentDateTime());
	config.sync();
	return true;
}

PromptsTable* TrainingManager::getPrompts()
{
	if (!promptsTable) initPrompts();
	return this->promptsTable;
}


/**
 * \brief Builds and returns the promptstable by parsing the file at the given path
 * \author Peter Grasch
 * \param QString promptspath
 * The path to the prompts file
 * \return PromptsTable*
 * The table
 */
PromptsTable* TrainingManager::readPrompts ( QString promptspath )
{
	Logger::log ( i18n ( "[INF] Parse Promptsdatei von %1" ).arg ( promptspath ) );
	PromptsTable *promptsTable = new PromptsTable();

	QFile *prompts = new QFile ( promptspath );
	prompts->open ( QFile::ReadOnly );
	if ( !prompts->isReadable() ) return new PromptsTable();

	QString label;
	QString prompt;
	QString line;
	int labelend;
	while ( !prompts->atEnd() ) //for each line that was successfully read
	{
		line = prompts->readLine ( 1024 );
		if (line.trimmed().isEmpty()) continue;
		labelend = line.indexOf ( " " );
		label = line.left ( labelend );
		prompt = line.mid ( labelend ).trimmed();

		promptsTable->insert ( label, prompt );
	}
	Logger::log ( i18n ( "[INF] %1 Prompts gelesen" ).arg ( promptsTable->count() ) );
	return promptsTable;
}

/**
 * \brief Creates a training text and sets it to be the current text
 * \author Peter Grasch
 */
void TrainingManager::trainWords ( const WordList *words )
{
	if ( !words ) return;

	Logger::log ( i18n ( "[INF] Starten eines  on-the-fly Trainings mit %1 Wörter" ).arg ( words->count() ) );

	QStringList pages;

	//we try to guess the perfect amount of words/page
	//We first go through the possible words/page word counts from 5 to 12
	//If we find a perfect match (means we have x pages with the /same/ amount of words
	//on them, we found a perfect words/page value
	//If not, we try to find the w/p count that leaves the last page with the closest
	//value of w/p than the others.
	//for example if we have given 37 words, we would end up with the following:
	//
	//	+---------+-----------------+
	//	| w/p     |    w/p last page|
	//	+---------------------------+
	//	|  5      |         2	    |
	//	|  6      |         1	    |
	//	|  7      |         2	    |
	//	|  8      |         5	    |
	//	|  9      |         1	    |
	//	|  10     |         7	    |
	//	|  11     |         4	    |
	//	|  12     |         1	    |
	//	+---------+-----------------+
	//
	//In this case the perfect amount of w/p would be 10 because even the last page
	//would have enough words for perfect accuracy

	short wordCount = words->count();
	short wordsPerPage=5;

	short maxLeftOver=0;
	short leftOverWordsPerPage=5;

	while ( ( wordCount%wordsPerPage != 0 ) && ( wordsPerPage <=12 ) )
	{
		if ( wordCount%wordsPerPage > maxLeftOver )
		{
			maxLeftOver = wordCount%wordsPerPage;
			leftOverWordsPerPage = wordsPerPage;
		}

		wordsPerPage++;
	}
	if ( wordsPerPage==13 ) wordsPerPage=leftOverWordsPerPage;

	QString page;
	QString time;
	for ( int i=0; i< ceil ( ( double ) wordCount/wordsPerPage ); i++ )
	{
		page="";
		for ( int j=0; ( j<wordsPerPage ) && ( j+ ( i*wordsPerPage ) < wordCount ); j++ )
		{
			page += words->at ( j+ ( i*wordsPerPage ) ).getWord() +QString ( " " );
		}

		pages.append ( page );

		time = qvariant_cast<QString>(QTime::currentTime());
		time.replace(QString(":"), QString("-"));
		sampleHash.insert((page.replace(" ", "_")+"_S"+QString::number(i+1)+"_"+QDate::currentDate().toString("yyyy-MM-dd")+"_"+time), page.toUpper());
	}

	TrainingText *newText = new TrainingText ( i18n ( "Spezialisiertes Training" ),
	        "", pages );

	currentText=newText;
}

/**
 * \brief Deletes the given file from the harddrive
 * \author Peter Grasch
 * \param int index
 * The index of the text to delete
 */
bool TrainingManager::deleteText ( int index )
{
	Logger::log ( i18n ( "[INF] Löschen von \"%1\" von \"%2\"" ).arg ( trainingTexts->at ( index )->getName() ).arg ( trainingTexts->at ( index )->getPath() ) );
	return QFile::remove( trainingTexts->at ( index )->getPath() );
}

/**
 * @brief Read the Training Texts and returns the list
 *
 * @return TrainingList*
 * The TrainingList (member)
 */
TrainingList* TrainingManager::readTrainingTexts ()
{
	Logger::log ( i18n ( "[INF] Lesen der Trainingtexte" ));

		//finddirs is net das was i will?...
	QStringList textsrcs = KGlobal::dirs()->findAllResources("appdata", "texts/");

	if (trainingTexts) {
		delete trainingTexts;
		trainingTexts=0;
	}
	
	trainingTexts = new TrainingList();
	for ( int i=0; i < textsrcs.count(); i++ )
	{
		XMLTrainingText *text = new XMLTrainingText ( textsrcs.at ( i ) );
		text->load ( textsrcs.at ( i ) );
		text->setRelevance ( calcRelevance ( text ) ); //FIXME: speed
		trainingTexts->append ( text );
	}
	
	return trainingTexts;
}

bool TrainingManager::refreshTraining(int sampleRate, const QByteArray& prompts)
{
	QMutexLocker lock(&promptsLock);
	
	SpeechModelManagementConfiguration::setModelSampleRate(sampleRate);
	
	QFile promptsF(KStandardDirs::locateLocal("appdata", "model/prompts"));
	if (!promptsF.open(QIODevice::WriteOnly))
		return false;
	
	promptsF.write(prompts);
	promptsF.close();
	
// 	emit promptsChanged();
	
	initPrompts();
	
	return true;
}

/**
 * \brief Marks text at the given index as the one we are training now
 * Stores a pointer of the text in the member currentText
 * \author Peter Grasch
 * \param int i
 * The index
 * \return bool
 * Success?
 */
bool TrainingManager::trainText ( int i )
{
	this->currentText = getText(i);
	
	if (!currentText) {
		return false;
	}
	
	Logger::log(i18n("[INF] Training Text: \"")+currentText->getName()+"\"");
	bool allWordsInDict = allWordsExisting();
	if(!allWordsInDict)
		return false;
	
	QString textName = getTextName();
	textName.replace(QString(" "), QString("_"));
	QString time = qvariant_cast<QString>(QTime::currentTime());
	time.replace(QString(":"), QString("-"));
	
	QString textFileName = QFile::encodeName(textName);
	for(int i=0; i<getPageCount(); i++)
	{
		sampleHash.insert((textFileName+"_S"+QString::number(i+1)+"_"+QDate::currentDate().toString("yyyy-MM-dd")+"_"+time), getPage(i));
	}
	return (currentText != NULL);
}


/**
 * \brief chechs if all words in the dict. If there some words missing in the dict, the addwordview dialog will be shown.
 * \author Susanne Tschernegg, Peter Grasch
 * @return bool
 *      returns whether all words are in the dict or not
 */
bool TrainingManager::allWordsExisting()
{
	QStringList strListAllWords;
	int pageCount=getPageCount();
	for ( int x=0; x<pageCount; x++ )
	{
		QStringList strList = getPage ( x ).split ( " " );
		int strListSize = strList.size();
		for ( int y=0; y < strListSize; y++ )
		{
			QString word = strList.at ( y );
				
				
			word = word.trimmed();
			word.remove ( "." );
			word.remove ( "," );
			word.remove ( "(" );
			word.remove ( ")" );
			word.remove ( "?" );
			word.remove ( "!" );
			word.remove ( "\"" );
			word.remove ( "/" );
			word.remove ( "\\" );
			word.remove ( "[" );
			word.remove ( "]" );
			
			
			if (!WordListManager::getInstance()->mainWordListContainsStr(word))
			{
				if (!strListAllWords.contains(word))
					strListAllWords.append ( word );
			}
		}
	}
	if ( strListAllWords.count() ==0 )
		return true;
	
	
	emit addMissingWords(strListAllWords); // tell addwordview what the hell is going on
	return false;
}

/**
 * \brief This is used to get the page <i> of the currently training text (stored in the currentText member)
 * \author Peter Grasch
 * \param int i
 * The index
 * \return QString
 * The text of the page <i>
 */
QString TrainingManager::getPage ( int i )
{
	if ( !currentText ) return "";
	return currentText->getPage ( i );
}

/**
 * \brief Returns the pagecount of the currently training text
 * \author Peter Grasch
 * \return int
 * count of pages
 */
int TrainingManager::getPageCount()
{
	if ( !currentText ) return 0;
	return currentText->getPageCount();
}

/**
 * \brief Returns the name of the currently trained text
 * \author Peter Grasch
 * \return QString
 * Name of the text
 */
QString TrainingManager::getTextName()
{
	if ( !currentText ) return "";
	return currentText->getName();
}

/**
 * \brief Returns the Text <i>
 * \author Peter Grasch
 * \param int i
 * The index of the text
 * \return TraininText*
 * Pointer to the TrainingText
 */
TrainingText* TrainingManager::getText ( int i )
{
	if (!this->trainingTexts) readTrainingTexts();
	
	if ( this->trainingTexts && (trainingTexts->count() > i))
		return this->trainingTexts->at ( i );
	else return NULL;
}



/**
 * \brief Calculates the relevance of the given text with the given wordlist
 * \author Peter Grasch, Susanne Tschernegg
 * @param text The text to check
 * @param wlist The wordlist as reference
 * @return An index of how well simon would recognize it - the lower the worse
 */
float TrainingManager::calcRelevance ( TrainingText *text )
{
	Logger::log ( i18n ( "[INF] Berechne Nutzen des Textes " ) +"\""+text->getName() +"\" ("+
	              text->getPath() +")" );
	QString currPage;
	QStringList words;
	int wordCount=0;
	int probability=0;
	int curWordCount=0;
	
	promptsLock.lock();
	int textWordCount=text->getPageCount();
	for ( int i=0; i<textWordCount; i++ )
	{
		currPage = text->getPage ( i );
		currPage.remove ( "." );
		currPage.remove ( "," );
		currPage.remove ( "?" );
		currPage.remove ( "!" );
		currPage.remove ( "\"" );
		currPage.remove ( "/" );
		currPage.remove ( "[" );
		currPage.remove ( "]" );

		words = currPage.split ( " " );
		
		curWordCount = words.count();
		
		wordCount += curWordCount;
		
		for ( int j=0; j<curWordCount; j++ )
		{
			QString currentWord = words[j];
			
			probability += getProbability ( currentWord );
		}
	}
	promptsLock.unlock();
	
	if ( wordCount > 0 )
		return qRound ( probability/wordCount );
	else return 0;
}

/**
 * @brief compiles the model
 *
 *	@author Susanne Tschernegg
 */
void TrainingManager::finishTrainingSession()
{
	addSamples ( sampleHash );
	sampleHash.clear();
	
	emit trainingFinished();
	
	initPrompts();
}


/**
 * \brief Returns the probability of the name (it is pulled out of the promptsTable)
 * \author Peter Grasch
 * \param QString wordname
 * Name of the word
 * \return int
 * Probability to recognize; The higher the more likely simon will recognize this word correctly
 */
int TrainingManager::getProbability ( QString wordname )
{
	if (!promptsTable) initPrompts();
	
	wordname = wordname.toUpper();
	if (wordRelevance.contains(wordname))
		return wordRelevance.value(wordname);
	
	QStringList prompts = promptsTable->values();
	int prob=0;
	int i=0;
	int promptscount=prompts.count();
	
	while ( i<promptscount )
	{
		prob += prompts.at ( i ).count(QRegExp(QString("( |^)%1( |$)").arg(wordname)));
		i++;
	}
	
	wordRelevance.insert(wordname, prob);
	return prob;
}

/**
 * @brief Adds the Samples to the prompts-file.
 *
 *	@author Peter Grasch
 *  @param QHash<QString, QString> *hash
 *      holds the pagenumber as text and the name of a text with the correspondenting sentence and the time and date, when the training has begun
 */
void TrainingManager::addSamples ( const QHash<QString, QString>& trainingsMap, bool recompiledLater )
{
	Q_ASSERT(promptsTable);
	
	QMutexLocker lock(&promptsLock);
	
	foreach (QString key, trainingsMap.keys())
		promptsTable->insert(key, trainingsMap.value(key));
	
	savePrompts(recompiledLater);
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingManager::~TrainingManager()
{
    delete trainingTexts;
    delete currentText;
}


