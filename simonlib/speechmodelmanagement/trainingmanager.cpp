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
}

bool TrainingManager::init()
{
	//init prompts
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
 * \brief Deletes the samples containing the given word
 * \author Peter Grasch
 * This will normally be used to delete a word from the language model (because no sample can contain a word
 * not in the model)
 * @param w The word to remove
 * @return Success
 */
bool TrainingManager::deleteWord ( Word *w )
{
	if (!promptsTable) init();
	
	QString wordToDelete = w->getWord().toUpper();

	QMutexLocker lock(&promptsLock);
	
	kDebug() << "Removing word: " << wordToDelete;
	QStringList sampleFileNames = promptsTable->keys();
	bool sampleAlreadyDeleted;
	bool succ = true;
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
					succ = false;

				sampleAlreadyDeleted = true;
				i--;
			}
		}
	}
	promptsLock.unlock();
	return (savePrompts() && succ);
}

/**
 * \brief Deletes the prompt corresponding to the key
 * \author Peter Grasch
 * @param key The key to delete
 * @return success
 */
bool TrainingManager::deletePrompt ( QString key )
{
	if (!promptsTable) init();
	QString path = SpeechModelManagementConfiguration::modelTrainingsDataPath().path()+"/"+key+".wav";
	
	kDebug() << "removing " << path;

	QMutexLocker lock(&promptsLock);
	promptsTable->remove ( key );
	//removes the sample
	return QFile::remove ( path );
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
bool TrainingManager::savePrompts()
{
	QMutexLocker lock(&promptsLock);
	if (!writePromptsFile(getPrompts(), KStandardDirs::locateLocal("appdata", "model/prompts"))) return false;

	//TODO: only emit this when it really changed
	emit trainingDataChanged();
	return true;
}

bool TrainingManager::writePromptsFile(PromptsTable* prompts, QString path)
{
	if (!promptsTable) init();
	
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
	if (!promptsTable) init();
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
	init();
	
	return true;
}


/**
 * \brief chechs if all words in the dict and returns those that aren't
 * \author Susanne Tschernegg, Peter Grasch
 * @return Missing words
 */
QStringList TrainingManager::missingWords(const QStringList& prompts)
{
	QStringList strListAllWords;
	for ( int x=0; x<prompts.count(); x++ )
	{
		QStringList strList = prompts[x].split ( " " );
		int strListSize = strList.size();
		for ( int y=0; y < strListSize; y++ )
		{
			QString word = strList.at ( y );
				
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
			word = word.trimmed();
			
			if (!WordListManager::getInstance()->mainWordListContainsStr(word))
			{
				if (!strListAllWords.contains(word))
					strListAllWords.append ( word );
			}
		}
	}
	return strListAllWords;
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
	{
		return this->trainingTexts->at ( i );
	} else return NULL;
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
// void TrainingManager::finishTrainingSession()
// {
// 	addSamples ( sampleHash );
// 	sampleHash.clear();
// 	
// 	emit trainingFinished();
// 	
// 	init();
// }


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
	if (!promptsTable) init();
	
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
// void TrainingManager::addSamples ( const QHash<QString, QString>& trainingsMap )
// {
// 	Q_ASSERT(promptsTable);
// 	
// 	QMutexLocker lock(&promptsLock);
// 	
// 	foreach (QString key, trainingsMap.keys())
// 		promptsTable->insert(key, trainingsMap.value(key));
// 	
// 	savePrompts();
// }

bool TrainingManager::addSample ( const QString& fileBaseName, const QString& prompt )
{
	Q_ASSERT(promptsTable);
	
	if (promptsTable->contains(fileBaseName)) 
		return false;
	
	promptsTable->insert(fileBaseName, prompt);
	return true;
}

bool TrainingManager::removeSample(const QString& fileBaseName)
{
	Q_ASSERT(promptsTable);
	
	return (promptsTable->remove(fileBaseName) > 0);
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingManager::~TrainingManager()
{
    delete trainingTexts;
}


