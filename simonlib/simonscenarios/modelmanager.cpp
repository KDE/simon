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

#include "modelmanager.h"
#include "trainingmanager.h"
#include "speechmodelmanagementconfiguration.h"

#include <simonscenarios/model.h>
#include <simonscenarios/languagedescriptioncontainer.h>
#include <simonscenarios/trainingcontainer.h>
#include <simonscenarios/shadowvocabulary.h>

#include <simonscenarios/scenariomanager.h>

#include <KStandardDirs>
#include <KMimeType>
#include <KFilterBase>
#include <KFilterDev>
#include <QFile>
#include <QFileInfo>
#include <QBuffer>
#include <QDir>
#include <QBuffer>


ModelManager::ModelManager(QObject* parent) : QObject(parent),
	inGroup(false),
	modelChangedFlag(false)
{
	connect (ScenarioManager::getInstance(), SIGNAL(scenariosChanged()), 
		  this, SLOT(modelHasChanged()));

	connect (ScenarioManager::getInstance(), SIGNAL(shadowVocabularyChanged()), 
		  this, SLOT(modelHasChanged()));

	connect (ScenarioManager::getInstance(), SIGNAL(baseModelChanged()), 
		  this, SLOT(modelHasChanged()));
	
	connect (TrainingManager::getInstance(), SIGNAL(trainingDataChanged()),
		  this, SLOT(modelHasChanged()));
	
	connect (TrainingManager::getInstance(), SIGNAL(trainingSettingsChanged()),
		  this, SLOT(modelHasChanged()));
}

void ModelManager::modelHasChanged()
{
	if (inGroup) 
		modelChangedFlag=true;
	else
		emit modelChanged();
}

void ModelManager::startGroup()
{
	modelChangedFlag=false;
	inGroup=true;
}

void ModelManager::commitGroup(bool silent)
{
	if (modelChangedFlag && !silent)
		emit modelChanged();
	modelChangedFlag=false;
	inGroup=false;
}


Model* ModelManager::createActiveContainer()
{
	qint32 modelSampleRate=SpeechModelManagementConfiguration::modelSampleRate();
	
	QFile hmmDefs(KStandardDirs::locate("appdata", "model/hmmdefs"));
	QFile tiedList(KStandardDirs::locate("appdata", "model/tiedlist"));
	
	QFile dict(KStandardDirs::locate("appdata", "model/model.dict"));
	QFile dfa(KStandardDirs::locate("appdata", "model/model.dfa"));
	
	if ((!hmmDefs.open(QIODevice::ReadOnly)) || (!tiedList.open(QIODevice::ReadOnly))
		|| (!dict.open(QIODevice::ReadOnly))
		|| (!dfa.open(QIODevice::ReadOnly)))
		return 0;
	
	return new Model(modelSampleRate, hmmDefs.readAll(), tiedList.readAll(), dict.readAll(), dfa.readAll());
}

Model* ModelManager::createBaseModelContainer()
{
	qint32 modelType = ScenarioManager::getInstance()->baseModelType();

	if (modelType == 2)
		return new Model(modelType, QByteArray(), QByteArray(), QByteArray(), QByteArray());
	
	QFile hmmDefs(KStandardDirs::locate("appdata", "model/basehmmdefs"));
	QFile tiedList(KStandardDirs::locate("appdata", "model/basetiedlist"));
	QFile macros(KStandardDirs::locate("appdata", "model/basemacros"));
	QFile stats(KStandardDirs::locate("appdata", "model/basestats"));
	
	if ((!hmmDefs.open(QIODevice::ReadOnly)) || (!tiedList.open(QIODevice::ReadOnly))
		|| (!macros.open(QIODevice::ReadOnly)) || (!stats.open(QIODevice::ReadOnly)))
		return 0;
	
	return new Model(modelType, hmmDefs.readAll(), tiedList.readAll(), macros.readAll(), stats.readAll());
}

qint32 ModelManager::getActiveModelSampleRate()
{
	return SpeechModelManagementConfiguration::modelSampleRate();
}

QDateTime ModelManager::getActiveContainerModifiedTime()
{
	if (!QFile::exists(KStandardDirs::locateLocal("appdata", "model/activemodelrc")))
		return QDateTime();
	KConfig config( KStandardDirs::locateLocal("appdata", "model/activemodelrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("Date", QDateTime());
}

QDateTime ModelManager::getBaseModelDate()
{
	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("BaseModelDate", QDateTime());
}

bool ModelManager::storeBaseModel(const QDateTime& changedTime, int baseModelType, 
					const QByteArray& hmmDefs, const QByteArray& tiedList,
					const QByteArray& macros, const QByteArray& stats)
{
	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("BaseModelDate", changedTime);
	cGroup.writeEntry("BaseModelType", baseModelType);
	config.sync();
	
	ScenarioManager::getInstance()->setBaseModelType(baseModelType);
	
	QFile hmmDefsFile(KStandardDirs::locateLocal("appdata", "model/basehmmdefs"));
	QFile tiedlistFile(KStandardDirs::locateLocal("appdata", "model/basetiedlist"));
	QFile macrosFile(KStandardDirs::locateLocal("appdata", "model/basemacros"));
	QFile statsFile(KStandardDirs::locateLocal("appdata", "model/basestats"));
	
	if (!hmmDefsFile.open(QIODevice::WriteOnly)
		|| !tiedlistFile.open(QIODevice::WriteOnly)
		|| !macrosFile.open(QIODevice::WriteOnly)
		|| !statsFile.open(QIODevice::WriteOnly))
		return false;
	
	hmmDefsFile.write(hmmDefs);
	tiedlistFile.write(tiedList);
	macrosFile.write(macros);
	statsFile.write(stats);
	
	hmmDefsFile.close();
	tiedlistFile.close();
	macrosFile.close();
	statsFile.close();
	return true;
}

bool ModelManager::storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& hmmDefs,
			const QByteArray& tiedList, const QByteArray& dict, const QByteArray& dfa)
{
	KConfig config( KStandardDirs::locateLocal("appdata", "model/activemodelrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("Date", changedTime);
	config.sync();
	
	SpeechModelManagementConfiguration::setModelSampleRate(sampleRate);
	
	QFile hmmDefsFile(KStandardDirs::locateLocal("appdata", "model/hmmdefs"));
	QFile tiedlistFile(KStandardDirs::locateLocal("appdata", "model/tiedlist"));
	QFile dictFile(KStandardDirs::locateLocal("appdata", "model/model.dict"));
	QFile dfaFile(KStandardDirs::locateLocal("appdata", "model/model.dfa"));
	
	if (!hmmDefsFile.open(QIODevice::WriteOnly)
		|| !tiedlistFile.open(QIODevice::WriteOnly)
		|| !dictFile.open(QIODevice::WriteOnly)
		|| !dfaFile.open(QIODevice::WriteOnly))
		return false;
	
	hmmDefsFile.write(hmmDefs);
	tiedlistFile.write(tiedList);
	dictFile.write(dict);
	dfaFile.write(dfa);
	
	hmmDefsFile.close();
	tiedlistFile.close();
	dictFile.close();
	dfaFile.close();
	
	return true;
}

#include <stdlib.h>
QByteArray ModelManager::getSample(const QString& sampleName)
{
	QString dirPath = SpeechModelManagementConfiguration::modelTrainingsDataPath().path()+QDir::separator();
	#ifdef Q_OS_WIN32
	dirPath = dirPath.toUpper();
	#endif
	QString path = dirPath+sampleName;
	
	QFile f(path);
	QFileInfo fInfo(f);
	QDir d(dirPath);
	if (!d.exists()) return QByteArray();
		
	//don't get tricked by /path/to/samples/../../../etc/passwd
	if (!fInfo.absoluteFilePath().contains(d.absolutePath()))
	{
		kDebug() << fInfo.absoluteFilePath() << " does not contain " << d.absolutePath();
		return QByteArray(); 
	}

	if (!f.open(QIODevice::ReadOnly))
		return QByteArray();
	return f.readAll();
}


LanguageDescriptionContainer* ModelManager::getLanguageDescriptionContainer()
{
	QFile treeHed(KStandardDirs::locate("appdata", "model/tree1.hed"));
	QFile shadowVocab(KStandardDirs::locate("appdata", "shadowvocabulary.xml"));


	if (!(shadowVocab.open(QIODevice::ReadOnly)) || (!treeHed.open(QIODevice::ReadOnly)))
		return 0;

	return new LanguageDescriptionContainer(shadowVocab.readAll(), treeHed.readAll());
}

QDateTime ModelManager::getLanguageDescriptionModifiedTime()
{
	ShadowVocabulary *vocab = ScenarioManager::getInstance()->getShadowVocabulary();
	if (!vocab) return QDateTime();

	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return qMax(vocab->lastModified(), cGroup.readEntry("LanguageDescriptionDate", QDateTime()));
}



bool ModelManager::storeLanguageDescription(const QDateTime& changedTime, QByteArray& shadowVocab, 
				        const QByteArray& treeHed)
{
	ShadowVocabulary *vocab = ScenarioManager::getInstance()->getShadowVocabulary();

	QIODevice *vocabDev = NULL;

	QString vocabTempPath = KStandardDirs::locateLocal("tmp", "simon/tmpshadow");
	QFile f(vocabTempPath);
	if (!f.open(QIODevice::WriteOnly))
		return false;
	f.write(shadowVocab);
	f.close();

	vocabDev = KFilterDev::deviceForFile(vocabTempPath, KMimeType::findByFileContent(vocabTempPath)->name());

	/*
	//KMimeType can't resolve mimetype like this
	KMimeType::Ptr mime = KMimeType::findByContent(shadowVocab);
	QStringList parentMimeTypes;
	if (mime)
		parentMimeTypes = mime->parentMimeTypes();
	kDebug() << "Shadow mime type" << parentMimeTypes;

	KFilterBase *base = NULL;
	int i=0;
	while (!base && i < parentMimeTypes.count())
	{
		base = KFilterBase::findFilterByMimeType(parentMimeTypes[i]);
		i++;
	}
	if (base == 0)
	{
		kDebug() << "Couldn't resolve mime type";
		vocabDev = new QBuffer(&shadowVocab);
	} else {
		kDebug() << "Using mime type filter";
		vocabDev = base->device();
	}
	*/

	if (!vocab->reset(vocabDev) || !vocab->save()) return false;
	delete vocabDev;

	QFile treeHedF(KStandardDirs::locateLocal("appdata", "model/tree1.hed"));
	if (!treeHedF.open(QIODevice::WriteOnly))
		return false;
	
	treeHedF.write(treeHed);
	treeHedF.close();
	
	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("LanguageDescriptionDate", changedTime);
	config.sync();
	return true;
}


TrainingContainer* ModelManager::getTrainingContainer()
{
	qint32 modelSampleRate=SpeechModelManagementConfiguration::modelSampleRate();
	
	QFile wavConfig(KStandardDirs::locate("appdata", "model/wav_config"));
	QFile prompts(KStandardDirs::locate("appdata", "model/prompts"));
	

	if ((!wavConfig.open(QIODevice::ReadOnly)) || (!prompts.open(QIODevice::ReadOnly)))
		return 0;

	return new TrainingContainer(modelSampleRate, wavConfig.readAll(),
				  prompts.readAll());
}

QDateTime ModelManager::getTrainingModifiedTime()
{
	if (!QFile::exists(KStandardDirs::locateLocal("appdata", "model/modelsrcrc")))
		return QDateTime();

	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	return cGroup.readEntry("TrainingDate", QDateTime());
}

bool ModelManager::storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& wavConfig,
					const QByteArray& prompts)
{
	if (!TrainingManager::getInstance()->refreshTraining(sampleRate, prompts))
		return false;
	
	if (!wavConfig.isEmpty())
	{
		QFile wavConfigF(KStandardDirs::locateLocal("appdata", "model/wav_config"));
		if (!wavConfigF.open(QIODevice::WriteOnly))
			return false;
	
		wavConfigF.write(wavConfig);
		wavConfigF.close();
	}
	
	
	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("TrainingDate", changedTime);
	config.sync();
	return true;
}

void ModelManager::buildMissingSamplesList()
{
	QStringList newList = TrainingManager::getInstance()->getPrompts()->keys();
	QDir samplesDir(SpeechModelManagementConfiguration::modelTrainingsDataPath().path());
	QStringList oldList = samplesDir.entryList(QStringList() << "*.wav");
	
	foreach (QString fileName, newList) {
		if ((!oldList.contains(fileName+".wav")) && (!this->missingFiles.contains(fileName)))
			missingFiles << fileName;
	}
	kDebug() << "Missing samples: " << missingFiles;
}


bool ModelManager::storeSample(const QByteArray& sample)
{
	if (missingFiles.isEmpty()) return false;

	QString dirPath = TrainingManager::getInstance()->getTrainingDir()+'/';

	QFile f(dirPath+missingFiles.at(0)+".wav");
	if (!f.open(QIODevice::WriteOnly)) return false;

	f.write(sample);
	f.close();

	missingFiles.removeAt(0);
	return true;
}

QString ModelManager::missingSample()
{
	if (missingFiles.isEmpty()) return QString();

	return missingFiles.at(0);
}

bool ModelManager::hasTraining()
{
	if (getTrainingModifiedTime().isNull()) return false;
	return (QFile::exists(KStandardDirs::locate("appdata", "model/wav_config")) && 
		  QFile::exists(KStandardDirs::locate("appdata", "model/prompts")));
}

bool ModelManager::hasLanguageDescription()
{
	if (getLanguageDescriptionModifiedTime().isNull()) return false;
	return (QFile::exists(KStandardDirs::locate("appdata", "model/tree1.hed")) && 
		  QFile::exists(KStandardDirs::locate("appdata", "model/shadow.voca")));
}

bool ModelManager::hasActiveContainer()
{
	if (getActiveContainerModifiedTime().isNull()) return false;
	return (QFile::exists(KStandardDirs::locate("appdata", "model/hmmdefs")) && 
		  QFile::exists(KStandardDirs::locate("appdata", "model/tiedlist")) &&
		  QFile::exists(KStandardDirs::locate("appdata", "model/model.dict")) && 
		  QFile::exists(KStandardDirs::locate("appdata", "model/model.dfa")));
}


