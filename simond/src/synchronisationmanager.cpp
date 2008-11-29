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
#include <QFile>
#include <QBuffer>
#include <QRegExp>
#include <QMutableMapIterator>
#include <KStandardDirs>
#include <KComponentData>
#include <KAboutData>
#include <KConfig>
#include <KConfigGroup>
#include <KDebug>



SynchronisationManager::SynchronisationManager(const QString& username, QObject *parent) : QObject(parent)
{
	this->username = username;
	//TODO adjust this to the current rev.
	KConfig currentModelConfig(KStandardDirs::locateLocal("appdata", "models/"+username+"/src/currentmodelrc"));
	KConfigGroup cGroup(&currentModelConfig, "");
	currentSrcContainerPath=cGroup.readEntry("CurrentModel", "");
	currentSrcContainerDate=cGroup.readEntry("CurrentModelDate", QDateTime());

	srcContainerTempPath = KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName()+"/"+username+"/sync/");
}


Model* SynchronisationManager::getActiveModel()
{
	if (username.isEmpty()) return 0;

	QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");

	QString configPath = dirPath+"activemodelrc";
	KConfig config( configPath, KConfig::SimpleConfig );
	qint32 sampleRate;
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

bool SynchronisationManager::storeActiveModel(const QDateTime& changedDate, qint32 sampleRate, const QByteArray& hmmDefs,
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

void SynchronisationManager::setActiveModelSampleRate(qint32 activeModelSampleRate)
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
	KConfig config( currentSrcContainerPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("WordListDate", QDateTime());
}

bool SynchronisationManager::hasWordList(const QString& modelPath)
{
	QString modelDirPath;
	if (modelPath.isEmpty())
		modelDirPath = currentSrcContainerPath;
	else modelDirPath=modelPath;
	QDir dir(modelDirPath);
	QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	if (entries.contains("simpleVocab") && 
		entries.contains("activeVocab") && 
		entries.contains("activeLexicon"))
		return true;
	else return false;
}


WordListContainer* SynchronisationManager::getWordList()
{
	QFile simpleVocab(currentSrcContainerPath+"simplevocab");
	QFile activeVocab(currentSrcContainerPath+"model.voca");
	QFile activeLexicon(currentSrcContainerPath+"lexicon");
	
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

	QFile simpleVocabFile(srcContainerTempPath+"simplevocab");
	QFile activeVocabFile(srcContainerTempPath+"model.voca");
	QFile activeLexiconFile(srcContainerTempPath+"lexicon");

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

	KConfig config( srcContainerTempPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("WordListDate", changedDate);
	config.sync();
	return true;
}



QDateTime SynchronisationManager::getGrammarDate()
{
	KConfig config( currentSrcContainerPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("GrammarDate", QDateTime());
}


bool SynchronisationManager::hasGrammar(const QString& modelPath)
{
	QString modelDirPath;
	if (modelPath.isEmpty())
		modelDirPath = currentSrcContainerPath;
	else modelDirPath=modelPath;
	QDir dir(modelDirPath);
	QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	if (entries.contains("grammarStructures"))
		return true;
	else return false;
}


GrammarContainer* SynchronisationManager::getGrammar()
{
	QFile grammar(currentSrcContainerPath+"model.grammar");
	if (!grammar.open(QIODevice::ReadOnly))
		return 0;

	return new GrammarContainer(grammar.readAll());
}


bool SynchronisationManager::storeGrammar(const QDateTime& changedDate, const QByteArray& grammarStructures)
{
	if (username.isEmpty()) return false;

	QFile grammarFile(srcContainerTempPath+"model.grammar");
	
	if ((!grammarFile.open(QIODevice::WriteOnly)))
		return 0;
	
	grammarFile.write(grammarStructures);
	grammarFile.close();

	KConfig config( srcContainerTempPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("GrammarDate", changedDate);
	config.sync();

	return true;
}



QDateTime SynchronisationManager::getLanguageDescriptionDate()
{
	KConfig config( currentSrcContainerPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("LanguageDescriptionDate", QDateTime());
}


bool SynchronisationManager::hasLanguageDescription(const QString& modelPath)
{
	QString modelDirPath;
	if (modelPath.isEmpty())
		modelDirPath = currentSrcContainerPath;
	else modelDirPath=modelPath;
	QDir dir(modelDirPath);
	QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	if (entries.contains("shadowVocab") && 
		entries.contains("shadowLexicon") && 
		entries.contains("treeHed"))
		return true;
	else return false;
}


LanguageDescriptionContainer* SynchronisationManager::getLanguageDescription()
{

	QFile treeHed(currentSrcContainerPath+"tree1.hed");
	QFile shadowVocab(currentSrcContainerPath+"shadow.voca");

	if ((!treeHed.open(QIODevice::ReadOnly))
		|| (!shadowVocab.open(QIODevice::ReadOnly)))
		return 0;

	return new LanguageDescriptionContainer(shadowVocab.readAll(), treeHed.readAll());
}


bool SynchronisationManager::storeLanguageDescription(const QDateTime& changedDate, const QByteArray& shadowVocab, 
			const QByteArray& treeHed)
{
	if (username.isEmpty()) return false;

	QFile treeHedFile(srcContainerTempPath+"tree1.hed");
	QFile shadowVocabFile(srcContainerTempPath+"shadow.voca");
	
	if ((!treeHedFile.open(QIODevice::WriteOnly))
		|| (!shadowVocabFile.open(QIODevice::WriteOnly)))
		return 0;
	
	treeHedFile.write(treeHed);
	shadowVocabFile.write(shadowVocab);

	treeHedFile.close();
	shadowVocabFile.close();

	KConfig config( srcContainerTempPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("LanguageDescriptionDate", changedDate);
	config.sync();
	return true;
}



QDateTime SynchronisationManager::getTrainingDate()
{
	KConfig config( currentSrcContainerPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("TrainingDate", QDateTime());
}


bool SynchronisationManager::hasTraining(const QString& modelPath)
{
	QString modelDirPath;
	if (modelPath.isEmpty())
		modelDirPath = currentSrcContainerPath;
	else modelDirPath=modelPath;
	QDir dir(modelDirPath);

	QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	if (entries.contains("wavConfig") &&
		entries.contains("trainingrc") && 
		entries.contains("prompts"))
		return true;
	else return false;
}


TrainingContainer* SynchronisationManager::getTraining()
{
	KConfig config( currentSrcContainerPath+"trainingrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	qint32 sampleRate = cGroup.readEntry("SampleRate").toInt();


	QFile wavConfig(currentSrcContainerPath+"wav_config");
	
	if (!wavConfig.open(QIODevice::ReadOnly))
		return 0;
	
	QFile prompts(currentSrcContainerPath+"prompts");
	if (!prompts.open(QIODevice::ReadOnly))
		return 0;

	return new TrainingContainer(sampleRate, wavConfig.readAll(),
				  prompts.readAll());
}


bool SynchronisationManager::storeTraining(const QDateTime& changedDate, qint32 sampleRate, const QByteArray& wavConfig,
			const QByteArray& prompts)
{
	if (username.isEmpty()) return false;

	QString configPath = srcContainerTempPath+"trainingrc";
	KConfig config( configPath, KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("SampleRate", sampleRate);
	config.sync();

	QFile promptsFile(srcContainerTempPath+"prompts");

	QFile wavConfigFile(srcContainerTempPath+"wav_config");

	if ((!wavConfigFile.open(QIODevice::WriteOnly)) || (!promptsFile.open(QIODevice::WriteOnly)))
		return false;
	
	wavConfigFile.write(wavConfig);
	promptsFile.write(prompts);

	wavConfigFile.close();
	promptsFile.close();

	KConfig configg( srcContainerTempPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroupg(&configg, "");
	cGroupg.writeEntry("TrainingDate", changedDate);
	configg.sync();
	
	return true;
}


void SynchronisationManager::buildMissingSamples()
{
	QFile promptsFile(srcContainerTempPath+"prompts");
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
	KConfig config( currentSrcContainerPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	QDateTime maxModifiedDate = qMax(cGroup.readEntry("WordListDate", QDateTime()),
					 cGroup.readEntry("GrammarDate", QDateTime()));
	maxModifiedDate = qMax(maxModifiedDate, cGroup.readEntry("LanguageDescriptionDate", QDateTime()));
	maxModifiedDate = qMax(maxModifiedDate, cGroup.readEntry("TrainingDate", QDateTime()));
	return maxModifiedDate;
}

bool SynchronisationManager::hasModelSrc()
{
	return hasWordList() && hasGrammar() && hasTraining();
}

bool SynchronisationManager::startSynchronisation()
{
	if (QFile::exists(srcContainerTempPath+"lock")) 
		return false;

	QFile lock(srcContainerTempPath+"lock");
	if (!lock.open(QIODevice::WriteOnly))
		return false;

	lock.write("Congratulations! You have discovered a secret message!");
	lock.close();
	return true;
}

bool SynchronisationManager::cleanTemp()
{
	QDir tempDir(srcContainerTempPath);
	QStringList files = tempDir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	files.removeAll("lock");

	bool allRemoved=true;
	foreach (const QString& file, files)
	{
		if (!QFile::remove(srcContainerTempPath+file))
			allRemoved=false;
	}

	return (QFile::remove(srcContainerTempPath+"lock")) && allRemoved && removeExcessModelBackups();
}

bool SynchronisationManager::abort()
{
	return cleanTemp();
}

bool SynchronisationManager::commit()
{
	if (!QFile::exists(srcContainerTempPath+"lock")) return false; //ARGH! someone could have screwed with this model!

	QDir tempDir(srcContainerTempPath);
	QStringList files = tempDir.entryList(QDir::Files|QDir::NoDotAndDotDot);
	files.removeAll("lock");
	if (files.count() == 0) {
		QFile::remove(srcContainerTempPath+"lock"); // unlock
		return true; // nothing to do; "commit" done
	}

	KConfig config( srcContainerTempPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	QDateTime newSrcContainerTime = qMax(cGroup.readEntry("WordListDate", QDateTime()),
					 cGroup.readEntry("GrammarDate", QDateTime()));
	newSrcContainerTime = qMax(newSrcContainerTime, cGroup.readEntry("LanguageDescriptionDate", QDateTime()));
	newSrcContainerTime = qMax(newSrcContainerTime, cGroup.readEntry("TrainingDate", QDateTime()));
	if (newSrcContainerTime.isNull()) return false; // wtf?

	QString newSrcContainerPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/"+newSrcContainerTime.toString("yyyy-MM-dd_hh:mm:ss")+"/");

	if (newSrcContainerPath.isEmpty()) return false;

	bool allCopied=true;
	foreach (const QString& file, files)
	{
		if (!QFile::copy(srcContainerTempPath+file, newSrcContainerPath+file))
		{
			allCopied=false;
		}
	}
	if (!allCopied) {
		return false;
	}

	KConfig currentModelConfig(KStandardDirs::locateLocal("appdata", "models/"+username+"/src/currentmodelrc"));
	KConfigGroup cGroup2(&currentModelConfig, "");
	cGroup2.writeEntry("CurrentModel", newSrcContainerPath);
	cGroup2.writeEntry("CurrentModelDate", newSrcContainerTime);
	currentModelConfig.sync();
	currentSrcContainerPath=newSrcContainerPath;
	return cleanTemp();
}

QMap<QDateTime, QString> SynchronisationManager::getModels()
{
	QMap<QDateTime, QString> models;

	QDir modelSrcDir(KStandardDirs::locateLocal("appdata", "models/"+username+"/src/"));
	if (!modelSrcDir.exists()) return models;

	QStringList folders = modelSrcDir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
	foreach (const QString folder, folders)
	{
		QDateTime folderDate = QDateTime::fromString(folder, "yyyy-MM-dd_hh:mm:ss");
		if (!folderDate.isValid()) continue;

		models.insert(folderDate, modelSrcDir.absolutePath()+QDir::separator()+folder+QDir::separator());
	}
	return models;
}

QMap<QDateTime, QString> SynchronisationManager::getWordLists()
{
	QMap<QDateTime, QString> models = getModels();

	//remove every model that does not contain a wordlist
	QMap<QDateTime, QString>::iterator i = models.begin();
	while (i != models.end())
	{
		QString path = i.value()+QDir::separator();
		if (!QFile::exists(path+"simplevocab") ||
				!QFile::exists(path+"model.voca") ||
				!QFile::exists(path+"lexicon"))
		{
			//does not contain a valid wordlist
			i = models.erase(i);
		} else 
			i++;
	}
	return models;
}


QMap<QDateTime, QString> SynchronisationManager::getGrammars()
{
	QMap<QDateTime, QString> models = getModels();

	//remove every model that does not contain a grammar
	QMap<QDateTime, QString>::iterator i = models.begin();
	while (i != models.end())
	{
		QString path = i.value()+QDir::separator();
		if (!QFile::exists(path+"model.grammar"))
		{
			//does not contain a valid grammar
			i = models.erase(i);
		} else 
			i++;
	}
	return models;
}

QMap<QDateTime, QString> SynchronisationManager::getTrainingDatas()
{
	QMap<QDateTime, QString> models = getModels();

	//remove every model that does not contain valid trainingsdatas
	QMap<QDateTime, QString>::iterator i = models.begin();
	while (i != models.end())
	{
		QString path = i.value()+QDir::separator();
		if (!QFile::exists(path+"trainingrc") ||
				!QFile::exists(path+"prompts") ||
				!QFile::exists(path+"wav_config"))
		{
			i = models.erase(i);
		} else 
			i++;
	}
	return models;

}

QMap<QDateTime, QString> SynchronisationManager::getLanguageDescriptions()
{
	QMap<QDateTime, QString> models = getModels();

	//remove every model that does not contain a language description
	QMap<QDateTime, QString>::iterator i = models.begin();
	while (i != models.end())
	{
		QString path = i.value()+QDir::separator();
		if (!QFile::exists(path+"tree1.hed")||
				!QFile::exists(path+"shadow.voca"))
		{
			//does not contain a valid language description
			i = models.erase(i);
		} else 
			i++;
	}
	return models;
}


bool SynchronisationManager::switchToModel(const QDateTime& modelDate)
{
	if (!startSynchronisation()) return false;

	QDateTime newModelDate = QDateTime::currentDateTime();
	KConfig config( srcContainerTempPath+"modelsrcrc", KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("WordListDate", newModelDate);
	cGroup.writeEntry("GrammarDate", newModelDate);
	cGroup.writeEntry("TrainingDate", newModelDate);
	cGroup.writeEntry("LanguageDescriptionDate", newModelDate);
	config.sync();

	QString wordListPath, grammarPath, trainingPath, languageDescriptionPath;
	QMap<QDateTime,QString> wordlistModels = getWordLists();
	for (QMap<QDateTime,QString>::const_iterator i=wordlistModels.constBegin(); i != wordlistModels.constEnd(); i++)
		if (i.key() >= modelDate)
		{
			wordListPath = i.value();
			break;
		}
	QMap<QDateTime,QString> grammarModels = getGrammars();
	for (QMap<QDateTime,QString>::const_iterator i=grammarModels.constBegin(); i != grammarModels.constEnd(); i++)
		if (i.key() >= modelDate)
		{
			grammarPath = i.value();
			break;
		}
	QMap<QDateTime,QString> trainingModels = getTrainingDatas();
	for (QMap<QDateTime,QString>::const_iterator i=trainingModels.constBegin(); i != trainingModels.constEnd(); i++)
		if (i.key() >= modelDate)
		{
			trainingPath = i.value();
			break;
		}
	QMap<QDateTime,QString> languageDescriptionModels = getLanguageDescriptions();
	for (QMap<QDateTime,QString>::const_iterator i=languageDescriptionModels.constBegin(); i != languageDescriptionModels.constEnd(); i++)
		if (i.key() >= modelDate)
		{
			languageDescriptionPath = i.value();
			break;
		}

	if (wordListPath.isEmpty() || grammarPath.isEmpty() || trainingPath.isEmpty() || languageDescriptionPath.isEmpty()) {
		abort();
		return false;
	}

	bool allCopied=true;
	if (!copyWordList(wordListPath, srcContainerTempPath)) allCopied=false;
	if (!copyTrainingData(trainingPath, srcContainerTempPath)) allCopied=false;
	if (!copyGrammar(grammarPath, srcContainerTempPath)) allCopied=false;
	if (!copyLanguageDescription(languageDescriptionPath, srcContainerTempPath)) allCopied=false;

	if (!allCopied)
	{
		abort();
		return false;
	}

	return commit();
}


bool SynchronisationManager::copyWordList(const QString& sourcePath, const QString& targetPath)
{
	bool allFine=true;
	if (!QFile::exists(targetPath+"lexicon") || !QFile::exists(targetPath+"model.voca") || !QFile::exists(targetPath+"simplevocab"))
	{
		if (!QFile::copy(sourcePath+"lexicon", targetPath+"lexicon")) allFine=false;
		if (!QFile::copy(sourcePath+"model.voca", targetPath+"model.voca")) allFine=false;
		if (!QFile::copy(sourcePath+"simplevocab", targetPath+"simplevocab")) allFine=false;
	}

	return allFine;
}

bool SynchronisationManager::copyGrammar(const QString& sourcePath, const QString& targetPath)
{
	bool allFine=true;
	if (!QFile::exists(targetPath+"model.grammar"))
		if (!QFile::copy(sourcePath+"model.grammar", targetPath+"model.grammar")) allFine=false;

	return allFine;
}

bool SynchronisationManager::copyTrainingData(const QString& sourcePath, const QString& targetPath)
{
	bool allFine=true;
	if (!QFile::exists(targetPath+"prompts") || !QFile::exists(targetPath+"trainingrc") || !QFile::exists(targetPath+"wav_config"))
	{
		if (!QFile::copy(sourcePath+"prompts", targetPath+"prompts")) allFine=false;
		if (!QFile::copy(sourcePath+"trainingrc", targetPath+"trainingrc")) allFine=false;
		if (!QFile::copy(sourcePath+"wav_config", targetPath+"wav_config")) allFine=false;
	}

	return allFine;
}

bool SynchronisationManager::copyLanguageDescription(const QString& sourcePath, const QString& targetPath)
{
	bool allFine=true;
	if (!QFile::exists(targetPath+"shadow.voca") || !QFile::exists(targetPath+"tree1.hed"))
	{
		if (!QFile::copy(sourcePath+"shadow.voca", targetPath+"shadow.voca")) allFine=false;
		if (!QFile::copy(sourcePath+"tree1.hed", targetPath+"tree1.hed")) allFine=false;
	}

	return allFine;
}




//FIXME: I crash!
bool SynchronisationManager::removeExcessModelBackups()
{
	int maxBackupedModels=5; //TODO make configurable
	
	QMap<QDateTime, QString> models = getModels();

	//date is ascending so the we can remove from the front until we have removed enough
//	QMutableMapIterator<QDateTime, QString> i(models);
	while ((models.count() > maxBackupedModels) && (models.count() >= 2))
	{
		QString modelToRemovePath = models.value(models.keys().at(0));
		QString modelTargetPath = models.value(models.keys().at(0));

		if (hasWordList(modelToRemovePath) && !copyWordList(modelToRemovePath, modelTargetPath))
			return false;
		if (hasGrammar(modelToRemovePath) && !copyGrammar(modelToRemovePath, modelTargetPath))
			return false;
		if (hasLanguageDescription(modelToRemovePath) && !copyLanguageDescription(modelToRemovePath, modelTargetPath))
			return false;
		if (hasTraining(modelToRemovePath) && !copyTrainingData(modelToRemovePath, modelTargetPath))
			return false;
	
		//remove modelToRemovePath
		QDir oldModelDir(modelToRemovePath);
		QStringList files = oldModelDir.entryList(QDir::Files|QDir::NoDotAndDotDot);
		bool allRemoved=true;
		foreach (const QString& file, files)
		{
			if (!QFile::remove(modelToRemovePath+file))
				allRemoved=false;
		}
		oldModelDir.cdUp(); //move out of oldModelDir
		if (!allRemoved || !oldModelDir.rmdir(modelToRemovePath))
			return false;

		models.remove(models.keys().at(0));
	}
	
	return true;
}

QString SynchronisationManager::getLatestPath(const QMap<QDateTime, QString>& models)
{
	QList<QDateTime> dates = models.keys();
	if (dates.isEmpty()) return QString();
	
	return models.value(dates.at(dates.count()-1));
}

QString SynchronisationManager::getLexiconPath()
{
	QString dir = getLatestPath(getWordLists());
	if (dir.isNull()) return QString();
	
	return dir+"lexicon";
}

QString SynchronisationManager::getGrammarPath()
{
	QString dir = getLatestPath(getGrammars());
	if (dir.isNull()) return QString();
	
	return dir+"model.grammar";
}

QString SynchronisationManager::getVocabPath()
{
	QString dir = getLatestPath(getWordLists());
	if (dir.isNull()) return QString();
	
	return dir+"model.voca";
}

QString SynchronisationManager::getPromptsPath()
{
	QString dir = getLatestPath(getTrainingDatas());
	if (dir.isNull()) return QString();
	
	return dir+"prompts";
}

QString SynchronisationManager::getTreeHedPath()
{
	QString dir = getLatestPath(getLanguageDescriptions());
	if (dir.isNull()) return QString();
	
	return dir+"tree1.hed";
}

QString SynchronisationManager::getWavConfigPath()
{
	QString dir = getLatestPath(getTrainingDatas());
	if (dir.isNull()) return QString();
	
	return dir+"wav_config";
}

SynchronisationManager::~SynchronisationManager()
{
	if (QFile::exists(srcContainerTempPath+"lock"))
		cleanTemp();
}
