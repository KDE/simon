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
#include "speechmodelmanagementconfiguration.h"
#include "scenariomanager.h"
#include "scenario.h"

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
#include <KLocale>
#include <math.h>


TrainingManager* TrainingManager::instance;

/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 */
TrainingManager::TrainingManager(QObject* parent) : QObject(parent),
	dirty(false),
	trainingTexts(new TrainingList()),
	promptsLock(QMutex::Recursive),
	promptsTable(0)
{
	KLocale::setMainCatalog("simonlib");
}

bool TrainingManager::init()
{
	//init prompts
	QMutexLocker lock(&promptsLock);
	PromptsTable *promptsTable = readPrompts ( KStandardDirs::locate("appdata", "model/prompts") );
	if (promptsTable) {
		if (this->promptsTable)
			delete this->promptsTable;

		this->promptsTable = promptsTable;
		dirty=false;

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
	
	QString wordToDelete = htkify(w->getWord().toUpper());

	QMutexLocker lock(&promptsLock);
	
	QStringList sampleFileNames = promptsTable->keys();
	bool succ = true;
	for ( int i=0; i < sampleFileNames.count(); i++ )
	{
		QString filename = sampleFileNames[i];
		QStringList promptWords = promptsTable->value ( filename ).split ( " " );
		if (promptWords.contains(wordToDelete))
		{
			if (!deletePrompt(filename)) succ = false;
		}
	}
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
	
	QMutexLocker lock(&promptsLock);
	promptsTable->remove ( key );
	//removes the sample
	return QFile::remove ( path.toUtf8() );
}

QString TrainingManager::getTrainingDir()
{
	QString dir = SpeechModelManagementConfiguration::modelTrainingsDataPath().path();
	if (!dir.endsWith("/") 
	#ifdef Q_OS_WINDOWS
			&& !dir.endsWith("\\")
	#endif
			)
		dir += QDir::separator();

	QDir d(dir);
	if (!d.exists()) d.mkpath(dir);
		
	return dir;
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

	if (dirty)
	{
		wordRelevance.clear(); // drop probability cache
		emit trainingDataChanged();
	}

	dirty=false;

	return true;
}


/**
 * \brief Saves any given promptstable to the given path. To save the main promptstable use savePrompts instead
 * \author Peter Grasch
 * \param prompts The promptstable to store
 * \param path Where to store the file
 * \return Success
 */
bool TrainingManager::writePromptsFile(PromptsTable* prompts, QString path)
{
	if (!promptsTable) init();
	
	QFile promptsFile ( path );
	if ( !promptsFile.open ( QIODevice::WriteOnly ) ) return false;

	QStringList samples = this->promptsTable->keys();

	for ( int i=0; i <samples.count(); i++ )
		promptsFile.write ( samples[i].toUtf8() +" "+prompts->value ( samples[i] ).toUtf8() +"\n" );
	promptsFile.close();
	
	kDebug() << "schreibe datum..." << QDateTime::currentDateTime();
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
	Logger::log ( i18n ( "[INF] Parsing prompts-file from %1" ).arg ( promptspath ) );
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
		QByteArray rawLine = prompts->readLine ( 1024 );
		line = QString::fromUtf8(rawLine);
		if (line.trimmed().isEmpty()) continue;
		labelend = line.indexOf ( " " );
		label = line.left ( labelend );
		prompt = line.mid ( labelend ).trimmed();

		promptsTable->insert ( label, prompt );
	}
	prompts->close();
	delete prompts;
	Logger::log ( i18n ( "[INF] %1 Prompts read" ).arg ( promptsTable->count() ) );
	return promptsTable;
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
			
			//if (!WordListManager::getInstance()->mainWordListContainsStr(word, Qt::CaseInsensitive))
			if (!ScenarioManager::getInstance()->getCurrentScenario()->containsWord(word))
			{
				if (!strListAllWords.contains(word))
					strListAllWords.append ( word );
			}
		}
	}
	return strListAllWords;
}


/**
 * \brief Calculates the relevance of the given text with the given wordlist
 * \author Peter Grasch, Susanne Tschernegg
 * @param text The text to check
 * @param wlist The wordlist as reference
 * @return An index of how well simon would recognize it - the lower the worse
 */
float TrainingManager::calcRelevance ( const TrainingText *text )
{
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
	
	wordname = htkify(wordname.toUpper());
	if (wordRelevance.contains(wordname))
		return wordRelevance.value(wordname);
	
	QStringList prompts = promptsTable->values();
	int prob=0;
	int i=0;
	int promptscount=prompts.count();
	
	while ( i<promptscount )
	{
		//prob += prompts.at ( i ).count(QRegExp(QString("( |^)%1( |$)").arg(wordname)));
		
		//work around some strange problem in my qt version...
		QString prompt = prompts.at(i);
		prob += prompt.count(" "+wordname+" ");
		if (prompt == wordname) prob++;
		else if (prompt.startsWith(wordname+" ")) prob++;
		else if (prompt.endsWith(" "+wordname)) prob++;

		i++;
	}
	
	wordRelevance.insert(wordname, prob);
	return prob;
}

QString TrainingManager::htkify(const QString& in)
{
	QString out = in;
	out.replace("0", "0"+i18n("ZERO"));
	out.replace("1", "1"+i18n("ONE"));
	out.replace("2", "2"+i18n("TWO"));
	out.replace("3", "3"+i18n("THREE"));
	out.replace("4", "4"+i18n("FOUR"));
	out.replace("5", "5"+i18n("FIVE"));
	out.replace("6", "6"+i18n("SIX"));
	out.replace("7", "7"+i18n("SEVEN"));
	out.replace("8", "8"+i18n("EIGHT"));
	out.replace("9", "9"+i18n("NINE"));
	return out;
}

bool TrainingManager::addSample ( const QString& fileBaseName, const QString& prompt )
{
	Q_ASSERT(promptsTable);
	
	QString htkPrompt = htkify(prompt);

	if (promptsTable->contains(fileBaseName)) 
		return false;

	promptsTable->insert(fileBaseName, htkPrompt);
	dirty=true;
	return true;
}

bool TrainingManager::removeSample(const QString& fileBaseName)
{
	Q_ASSERT(promptsTable);
	
	if (promptsTable->remove(fileBaseName) > 0)
	{
		dirty=true;
		return true;
	} else return false;
}

bool TrainingManager::defaultToPowerTrain()
{
	return SpeechModelManagementConfiguration::defaultToPowerTrain();
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingManager::~TrainingManager()
{
    qDeleteAll(*trainingTexts);
    delete trainingTexts;
}


