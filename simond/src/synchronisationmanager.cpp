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

#include "synchronisationmanager.h"

#include <speechmodelbase/model.h>
#include <speechmodelbase/wordlistcontainer.h>
#include <speechmodelbase/grammarcontainer.h>
#include <speechmodelbase/languagedescriptioncontainer.h>
#include <speechmodelbase/trainingcontainer.h>

#include <QString>
#include <QFileInfo>
#include <QDir>

#include <KStandardDirs>
#include <QFile>
#include <KConfig>
#include <KConfigGroup>
#include <KDebug>


SynchronisationManager::SynchronisationManager(const QString& username, QObject *parent) : QObject(parent)
{
	this->username = username;
}


Model* SynchronisationManager::getActiveModel()
{
	if (username.isEmpty()) return 0;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");

	QString configPath = dirPath+"activemodelrc";
	KConfig config( configPath, KConfig::SimpleConfig );
	int channels;
	int sampleRate;
	KConfigGroup cGroup(&config, "");
	channels = cGroup.readEntry("channels").toInt();
	sampleRate = cGroup.readEntry("samplerate").toInt();
	
	QFile hmmDefs(dirPath+"hmmdefs");
	QFile tiedlist(dirPath+"tiedlist");
	QFile dict(dirPath+"model.dict");
	QFile term(dirPath+"model.term");
	
	if ((!hmmDefs.open(QIODevice::ReadOnly)) || 
		(!tiedlist.open(QIODevice::ReadOnly)) || 
		(!dict.open(QIODevice::ReadOnly)) || 
		(!term.open(QIODevice::ReadOnly)))
	{
		kDebug() << "Failed to gather active model";
		return false;
	}

	return new Model(channels, sampleRate, hmmDefs.readAll(), tiedlist.readAll(), dict.readAll(), term.readAll());
}

QDateTime SynchronisationManager::getActiveModelDate()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");

	KConfig config( dirPath+"activemodelrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("Date", QDateTime());

// 	QDateTime maxModifiedDate = qMax(QFileInfo(dirPath+"hmmdefs").lastModified(),
// 					 QFileInfo(dirPath+"tiedlist").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(configPath).lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"model.dict").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"model.term").lastModified());
// 	return maxModifiedDate;
}

bool SynchronisationManager::storeActiveModel(const QDateTime& changedDate, int sampleRate, int channels, const QByteArray& hmmDefs,
		const QByteArray& tiedList, const QByteArray& dict, const QByteArray& term)
{
	//TODO: Implement
	return false;
}

QDateTime SynchronisationManager::getWordListDate()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");

	KConfig config( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("WordListDate", QDateTime());
// 	QDateTime maxModifiedDate = qMax(QFileInfo(dirPath+"simplevocab").lastModified(),
// 					 QFileInfo(dirPath+"lexicon").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"model.voca").lastModified());
// 	return maxModifiedDate;
}

bool SynchronisationManager::hasWordList()
{
	QDir dir(KStandardDirs::locateLocal("appdata", "models/"+username+"/src/"));
	QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	if (entries.contains("simpleVocab") && 
		entries.contains("activeVocab") && 
		entries.contains("activeLexicon"))
		return true;
	else return false;
}


WordListContainer* SynchronisationManager::getWordList()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	QFile simpleVocab(dirPath+"simplevocab");
	QFile activeVocab(dirPath+"model.voca");
	QFile activeLexicon(dirPath+"lexicon");
	
	if ((!simpleVocab.open(QIODevice::ReadOnly))
		|| (!activeVocab.open(QIODevice::ReadOnly))
		|| (!activeLexicon.open(QIODevice::ReadOnly)))
		return 0;

	return new WordListContainer(simpleVocab.readAll(), activeVocab.readAll(), activeLexicon.readAll());
}


bool SynchronisationManager::storeWordList(const QDateTime& changedDate, const QByteArray& simpleVocab,
			const QByteArray& activeVocab, const QByteArray& activeLexicon)
{
	if (username.isEmpty()) return false;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	QFile simpleVocabFile(dirPath+"simplevocab");
	QFile activeVocabFile(dirPath+"model.voca");
	QFile activeLexiconFile(dirPath+"lexicon");

	if ((!simpleVocabFile.open(QIODevice::WriteOnly))
		|| (!activeVocabFile.open(QIODevice::WriteOnly))
		|| (!activeLexiconFile.open(QIODevice::WriteOnly)))
		return 0;
	
	simpleVocabFile.write(simpleVocab);
	activeVocabFile.write(activeVocab);
	activeLexiconFile.write(activeLexicon);
	simpleVocabFile.close();
	activeVocabFile.close();
	activeLexiconFile.close();

	KConfig config( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("WordListDate", changedDate);
	config.sync();
	return true;
}



QDateTime SynchronisationManager::getGrammarDate()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	KConfig config( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("GrammarDate", QDateTime());
}


bool SynchronisationManager::hasGrammar()
{
	QDir dir(KStandardDirs::locateLocal("appdata", "models/"+username+"/src/"));
	QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	if (entries.contains("grammarStructures"))
		return true;
	else return false;
}


GrammarContainer* SynchronisationManager::getGrammar()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");

	QFile grammar(dirPath+"model.grammar");
	if (!grammar.open(QIODevice::ReadOnly))
		return 0;

	return new GrammarContainer(grammar.readAll());
}


bool SynchronisationManager::storeGrammar(const QDateTime& changedDate, const QByteArray& grammarStructures)
{
	if (username.isEmpty()) return false;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	QFile grammarFile(dirPath+"model.grammar");
	
	if ((!grammarFile.open(QIODevice::WriteOnly)))
		return 0;
	
	grammarFile.write(grammarStructures);
	grammarFile.close();

	KConfig config( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("GrammarDate", changedDate);
	config.sync();

	return true;
}



QDateTime SynchronisationManager::getLanguageDescriptionDate()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");

	KConfig config( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("LanguageDescriptionDate", QDateTime());

// 	QDateTime maxModifiedDate = qMax(QFileInfo(dirPath+"shadowlexicon").lastModified(),
// 					 QFileInfo(dirPath+"tree1.hed").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"shadow.voca").lastModified());
// 	return maxModifiedDate;
}


bool SynchronisationManager::hasLanguageDescription()
{
	QDir dir(KStandardDirs::locateLocal("appdata", "models/"+username+"/src/"));
	QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	if (entries.contains("shadowVocab") && 
		entries.contains("shadowLexicon") && 
		entries.contains("treeHed"))
		return true;
	else return false;
}


LanguageDescriptionContainer* SynchronisationManager::getLanguageDescription()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");

	QFile treeHed(dirPath+"tree1.hed");
	QFile shadowVocab(dirPath+"shadow.voca");
	QFile shadowLexicon(dirPath+"shadowlexicon");

	if ((!treeHed.open(QIODevice::ReadOnly))
		|| (!shadowVocab.open(QIODevice::ReadOnly))
		|| (!shadowLexicon.open(QIODevice::ReadOnly)))
		return 0;

	return new LanguageDescriptionContainer(shadowVocab.readAll(), shadowLexicon.readAll(), treeHed.readAll());
}


bool SynchronisationManager::storeLanguageDescription(const QDateTime& changedDate, const QByteArray& shadowVocab, const QByteArray& shadowLexicon, 
			const QByteArray& treeHed)
{
	if (username.isEmpty()) return false;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	QFile treeHedFile(dirPath+"tree1.hed");
	QFile shadowVocabFile(dirPath+"shadow.voca");
	QFile shadowLexiconFile(dirPath+"shadowlexicon");
	
	if ((!treeHedFile.open(QIODevice::WriteOnly))
		|| (!shadowVocabFile.open(QIODevice::WriteOnly))
		|| (!shadowLexiconFile.open(QIODevice::WriteOnly)))
		return 0;
	
	treeHedFile.write(treeHed);
	shadowVocabFile.write(shadowVocab);
	shadowLexiconFile.write(shadowLexicon);

	treeHedFile.close();
	shadowVocabFile.close();
	shadowLexiconFile.close();

	KConfig config( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("LanguageDescriptionDate", changedDate);
	config.sync();
	return true;
}



QDateTime SynchronisationManager::getTrainingDate()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");

	KConfig config( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("TrainingDate", QDateTime());

// 	QDateTime maxModifiedDate = qMax(QFileInfo(dirPath+"wav_config").lastModified(),
// 					 QFileInfo(dirPath+"prompts").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(configPath).lastModified());
// 	return maxModifiedDate;
}


bool SynchronisationManager::hasTraining()
{
	QDir dir(KStandardDirs::locateLocal("appdata", "models/"+username+"/src/"));
	QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	if (entries.contains("wavConfig") &&
		entries.contains("trainingrc") && 
		entries.contains("prompts"))
		return true;
	else return false;
}


TrainingContainer* SynchronisationManager::getTraining()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	KConfig config( dirPath+"trainingrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	int channels = cGroup.readEntry("channels").toInt();
	int sampleRate = cGroup.readEntry("samplerate").toInt();


	QFile wavConfig(dirPath+"wav_config");
	
	if (!wavConfig.open(QIODevice::ReadOnly))
		return 0;
	
	QFile prompts(dirPath+"prompts");
	if (!prompts.open(QIODevice::ReadOnly))
		return 0;

	return new TrainingContainer(sampleRate, channels, wavConfig.readAll(),
				  prompts.readAll());
}


bool SynchronisationManager::storeTraining(const QDateTime& changedDate, int soundChannels, int sampleRate, const QByteArray& wavConfig,
			const QByteArray& prompts)
{
	if (username.isEmpty()) return false;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	QString configPath = dirPath+"trainingrc";
	KConfig config( configPath, KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("channels", soundChannels);
	cGroup.writeEntry("samplerate", sampleRate);
	config.sync();

	QFile wavConfigFile(dirPath+"wav_config");
	QFile promptsFile(dirPath+"prompts");

	if ((!wavConfigFile.open(QIODevice::WriteOnly)) || (!promptsFile.open(QIODevice::WriteOnly)))
		return false;
	
	wavConfigFile.write(wavConfig);
	promptsFile.write(prompts);

	wavConfigFile.close();
	promptsFile.close();

	KConfig configg( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroupg(&configg, "");
	cGroupg.writeEntry("TrainingDate", changedDate);
	configg.sync();
	return true;
}



// ModelContainer* SynchronisationManager::getModelSrc()
// {
// 	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
// 	QString configPath = dirPath+"trainingrc";
// 	KConfig config( configPath, KConfig::SimpleConfig );
// 	int channels;
// 	int sampleRate;
// 	KConfigGroup cGroup(&config, "");
// 	channels = cGroup.readEntry("channels").toInt();
// 	sampleRate = cGroup.readEntry("samplerate").toInt();
// 
// 	QFile *wavConfig = new QFile(dirPath+"wav_config");
// 	QFile *treeHed = new QFile(dirPath+"tree1.hed");
// 	QFile *grammar = new QFile(dirPath+"model.grammar");
// 	
// 	QFile *simpleVocab = new QFile(dirPath+"simplevocab");
// 	QFile *activeVocab = new QFile(dirPath+"lexicon");
// 	QFile *activeLexicon = new QFile(dirPath+"model.voca");
// 	QFile *shadowVocab = new QFile(dirPath+"shadowlexicon");
// 	QFile *shadowLexicon = new QFile(dirPath+"shadow.voca");
// 	QFile *prompts = new QFile(dirPath+"prompts");
// 
// 	if ((!wavConfig->open(QIODevice::ReadOnly)) || (!treeHed->open(QIODevice::ReadOnly))
// 		|| (!grammar->open(QIODevice::ReadOnly))
// 		|| (!simpleVocab->open(QIODevice::ReadOnly))
// 		|| (!activeVocab->open(QIODevice::ReadOnly))
// 		|| (!activeLexicon->open(QIODevice::ReadOnly))
// 		|| (!shadowVocab->open(QIODevice::ReadOnly))
// 		|| (!shadowLexicon->open(QIODevice::ReadOnly))
// 		|| (!prompts->open(QIODevice::ReadOnly)))
// 		return 0;
// 	
// 	QHash<QString,QString> trainingsMap;
// 
// 	QTextStream ts(prompts);
// 	while (!ts.atEnd())
// 	{
// 		QString line = ts.readLine();
// 		int firstSpace=line.indexOf(" ");
// 		trainingsMap.insert(line.left(firstSpace), line.mid(firstSpace+1));
// 	}
// 	prompts->close();
// 	delete prompts;
// 	
// 	return new ModelContainer(sampleRate, channels, wavConfig,
// 				   grammar, simpleVocab, activeVocab, activeLexicon, 
// 				   shadowVocab, shadowLexicon, treeHed, trainingsMap);
// }

// bool SynchronisationManager::storeModelSrc(int sampleRate, int soundChannels, const QByteArray& wavConfig,
// 					const QByteArray& grammarStructures, const QByteArray& simpleVocab,
// 					const QByteArray& activeVocab, const QByteArray& activeLexicon, 
// 				        const QByteArray& shadowVocab, const QByteArray& shadowLexicon, 
// 				        const QByteArray& treeHed, const QHash<QString, QString>& trainingsMap)
// {
// 	if (username.isEmpty()) return false;
// 
// 	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
// 	QString configPath = dirPath+"trainingrc";
// 	KConfig config( configPath, KConfig::SimpleConfig );
// 	KConfigGroup cGroup(&config, "");
// 	cGroup.writeEntry("channels", soundChannels);
// 	cGroup.writeEntry("samplerate", sampleRate);
// 	config.sync();
// 
// 	QFile wavConfigFile(dirPath+"wav_config");
// 	QFile treeHedFile(dirPath+"tree1.hed");
// 	QFile grammarFile(dirPath+"model.grammar");
// 	
// 	QFile simpleVocabFile(dirPath+"simplevocab");
// 	QFile activeVocabFile(dirPath+"lexicon");
// 	QFile activeLexiconFile(dirPath+"model.voca");
// 	QFile shadowVocabFile(dirPath+"shadowlexicon");
// 	QFile shadowLexiconFile(dirPath+"shadow.voca");
// 	QFile promptsFile(dirPath+"prompts");
// 
// 	if ((!wavConfigFile.open(QIODevice::WriteOnly)) || (!treeHedFile.open(QIODevice::WriteOnly))
// 		|| (!grammarFile.open(QIODevice::WriteOnly))
// 		|| (!simpleVocabFile.open(QIODevice::WriteOnly))
// 		|| (!activeVocabFile.open(QIODevice::WriteOnly))
// 		|| (!activeLexiconFile.open(QIODevice::WriteOnly))
// 		|| (!shadowVocabFile.open(QIODevice::WriteOnly))
// 		|| (!shadowLexiconFile.open(QIODevice::WriteOnly))
// 		|| (!promptsFile.open(QIODevice::WriteOnly)))
// 		return 0;
// 	
// 	wavConfigFile.write(wavConfig);
// 	treeHedFile.write(treeHed);
// 	grammarFile.write(grammarStructures);
// 	simpleVocabFile.write(simpleVocab);
// 	activeVocabFile.write(activeVocab);
// 	activeLexiconFile.write(activeLexicon);
// 	shadowVocabFile.write(shadowVocab);
// 	shadowLexiconFile.write(shadowLexicon);
// 
// 	QStringList trainingFiles = trainingsMap.keys();
// 	foreach (const QString& file, trainingFiles)
// 		promptsFile.write(file.toUtf8()+" "+trainingsMap.value(file).toUtf8());
// 	
// 	
// 	wavConfigFile.close();
// 	treeHedFile.close();
// 	grammarFile.close();
// 	simpleVocabFile.close();
// 	activeVocabFile.close();
// 	activeLexiconFile.close();
// 	shadowVocabFile.close();
// 	shadowLexiconFile.close();
// 	promptsFile.close();
// 	return true;
// }



QDateTime SynchronisationManager::getModelSrcDate()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
// 	QString configPath = dirPath+"trainingrc";

	KConfig config( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	QDateTime maxModifiedDate = qMax(cGroup.readEntry("WordListDate", QDateTime()),
					 cGroup.readEntry("GrammarDate", QDateTime()));
	maxModifiedDate = qMax(maxModifiedDate, cGroup.readEntry("LanguageDescriptionDate", QDateTime()));
	maxModifiedDate = qMax(maxModifiedDate, cGroup.readEntry("TrainingDate", QDateTime()));

// 	QDateTime maxModifiedDate = qMax(QFileInfo(dirPath+"wav_config").lastModified(),
// 					 QFileInfo(dirPath+"tree1.hed").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(configPath).lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"model.grammar").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"simplevocab").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"lexicon").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"model.voca").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"shadowlexicon").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"shadow.voca").lastModified());
// 	maxModifiedDate = qMax(maxModifiedDate, QFileInfo(dirPath+"prompts").lastModified());
	return maxModifiedDate;
}

SynchronisationManager::~SynchronisationManager()
{
	
}
