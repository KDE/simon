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
#include <QBuffer>
#include <QRegExp>


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
	int sampleRate;
	KConfigGroup cGroup(&config, "");
	sampleRate = cGroup.readEntry("SampleRate").toInt();
	
	QFile hmmDefs(dirPath+"hmmdefs");
	QFile tiedlist(dirPath+"tiedlist");
	QFile dict(dirPath+"model.dict");
	QFile dfa(dirPath+"model.dfa");
	
	if ((!hmmDefs.open(QIODevice::ReadOnly)) || 
		(!tiedlist.open(QIODevice::ReadOnly)) || 
		(!dict.open(QIODevice::ReadOnly)) || 
		(!dfa.open(QIODevice::ReadOnly)))
	{
		kDebug() << "Failed to gather active model";
		return false;
	}

	return new Model(sampleRate, hmmDefs.readAll(), tiedlist.readAll(), dict.readAll(), dfa.readAll());
}

QDateTime SynchronisationManager::getActiveModelDate()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");

	KConfig config( dirPath+"activerc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("Date", QDateTime());
}

bool SynchronisationManager::storeActiveModel(const QDateTime& changedDate, int sampleRate, const QByteArray& hmmDefs,
		const QByteArray& tiedList, const QByteArray& dict, const QByteArray& dfa)
{
	if (username.isEmpty()) return false;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
	QFile hmmDefsFile(dirPath+"hmmdefs");
	QFile tiedListFile(dirPath+"tiedlist");
	QFile dictFile(dirPath+"model.dict");
	QFile dfaFile(dirPath+"model.dfa");

	if ((!hmmDefsFile.open(QIODevice::WriteOnly))
		|| (!tiedListFile.open(QIODevice::WriteOnly))
		|| (!dictFile.open(QIODevice::WriteOnly))
		|| (!dfaFile.open(QIODevice::WriteOnly)))
		return 0;
	
	hmmDefsFile.write(hmmDefs);
	tiedListFile.write(tiedList);
	dictFile.write(dict);
	dfaFile.write(dfa);
	hmmDefsFile.close();
	tiedListFile.close();
	dictFile.close();
	dfaFile.close();

	KConfig config( dirPath+"activerc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("Date", changedDate);
	cGroup.writeEntry("SampleRate", sampleRate);
	config.sync();
	return true;
}

void SynchronisationManager::setActiveModelSampleRate(int activeModelSampleRate)
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
	KConfig config( dirPath+"activerc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("SampleRate", activeModelSampleRate);
	config.sync();
}

bool SynchronisationManager::hasActiveModel()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
	return (QFile::exists(dirPath+"hmmdefs")&&QFile::exists(dirPath+"tiedlist")
		&&QFile::exists(dirPath+"model.dict")&&QFile::exists(dirPath+"model.dfa"));
}

void SynchronisationManager::modelCompiled()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
	KConfig config( dirPath+"activerc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("Date", QDateTime::currentDateTime());
	config.sync();
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

	if ((!treeHed.open(QIODevice::ReadOnly))
		|| (!shadowVocab.open(QIODevice::ReadOnly)))
		return 0;

	return new LanguageDescriptionContainer(shadowVocab.readAll(), treeHed.readAll());
}


bool SynchronisationManager::storeLanguageDescription(const QDateTime& changedDate, const QByteArray& shadowVocab, 
			const QByteArray& treeHed)
{
	if (username.isEmpty()) return false;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	QFile treeHedFile(dirPath+"tree1.hed");
	QFile shadowVocabFile(dirPath+"shadow.voca");
	
	if ((!treeHedFile.open(QIODevice::WriteOnly))
		|| (!shadowVocabFile.open(QIODevice::WriteOnly)))
		return 0;
	
	treeHedFile.write(treeHed);
	shadowVocabFile.write(shadowVocab);

	treeHedFile.close();
	shadowVocabFile.close();

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
	int sampleRate = cGroup.readEntry("SampleRate").toInt();


	QFile wavConfig(dirPath+"wav_config");
	
	if (!wavConfig.open(QIODevice::ReadOnly))
		return 0;
	
	QFile prompts(dirPath+"prompts");
	if (!prompts.open(QIODevice::ReadOnly))
		return 0;

	return new TrainingContainer(sampleRate, wavConfig.readAll(),
				  prompts.readAll());
}


bool SynchronisationManager::storeTraining(const QDateTime& changedDate, int sampleRate, const QByteArray& wavConfig,
			const QByteArray& prompts)
{
	if (username.isEmpty()) return false;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	QString configPath = dirPath+"trainingrc";
	KConfig config( configPath, KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("SampleRate", sampleRate);
	config.sync();

	QFile promptsFile(dirPath+"prompts");

	QFile wavConfigFile(dirPath+"wav_config");

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


void SynchronisationManager::buildMissingSamples()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");
	QFile promptsFile(dirPath+"prompts");
	QStringList oldList, newList;
	QDir samplesDir(KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/"));
	oldList = samplesDir.entryList(QStringList() << "*.wav");
	

	if (promptsFile.open(QIODevice::ReadOnly))
	{
		while (!promptsFile.atEnd())
		{
			QString promptsLine = QString::fromUtf8(promptsFile.readLine());
			newList << promptsLine.left(promptsLine.indexOf(" "));
		}
		promptsFile.close();
	}
	
	foreach (QString fileName, newList)
	{
		if ((!oldList.contains(fileName+".wav")) && (!this->missingFiles.contains(fileName)))
			missingFiles << fileName;
	}
	
}


QByteArray SynchronisationManager::getSample(const QString& sampleName)
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/");
	QFile f(dirPath+"/"+sampleName.toUtf8());
	kDebug() << "Retrieving " << dirPath+"/"+sampleName.toUtf8();
	if (!f.open(QIODevice::ReadOnly)) return QByteArray();
	QByteArray content = f.readAll();
	return content;
}

bool SynchronisationManager::storeSample(const QByteArray& sample)
{
	if (missingFiles.isEmpty()) return false;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/");

	QFile f(dirPath+missingFiles.at(0)+".wav");
	if (!f.open(QIODevice::WriteOnly)) return false;

	f.write(sample);
	f.close();

	missingFiles.removeAt(0);
	return true;
}

QString SynchronisationManager::missingSample()
{
	if (missingFiles.isEmpty()) return QString();

	return missingFiles.at(0);
}


QDateTime SynchronisationManager::getModelSrcDate()
{
	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/");

	KConfig config( dirPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	QDateTime maxModifiedDate = qMax(cGroup.readEntry("WordListDate", QDateTime()),
					 cGroup.readEntry("GrammarDate", QDateTime()));
	maxModifiedDate = qMax(maxModifiedDate, cGroup.readEntry("LanguageDescriptionDate", QDateTime()));
	maxModifiedDate = qMax(maxModifiedDate, cGroup.readEntry("TrainingDate", QDateTime()));
	return maxModifiedDate;
}

SynchronisationManager::~SynchronisationManager()
{
	
}
