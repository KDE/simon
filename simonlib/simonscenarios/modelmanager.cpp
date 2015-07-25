/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
#include <simonscenarios/modelmetadata.h>
#include <simonscenarios/languagedescriptioncontainer.h>
#include <simonscenarios/trainingcontainer.h>
#include <simonscenarios/shadowvocabulary.h>

#include <simonscenarios/scenariomanager.h>

#include <QFile>
#include <QFileInfo>
#include <QBuffer>
#include <QDir>

#include <KStandardDirs>
#include <KMimeType>
#include <KDateTime>
#include <KFilterBase>
#include <KFilterDev>
#include <KTar>

ModelManager* ModelManager::instance = 0;

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

  // read active model and build blacklistedTranscriptions
  QString activePath = KStandardDirs::locate("appdata", "model/active.sbm");
  if (QFile::exists(activePath)) {
    KTar tar(activePath, "application/x-gzip");
    ModelMetadata *data = metaData(tar);
    if (data) {
      updateBlacklistedTranscriptions(data);
      delete data;
    }
  }
}

ModelManager* ModelManager::getInstance()
{
  if (!instance) {
    instance = new ModelManager();
  }
  return instance;
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

  QFile activeContainer(KStandardDirs::locate("appdata", "model/active.sbm"));

  if (!activeContainer.open(QIODevice::ReadOnly))
    return 0;

  return new Model(modelSampleRate, activeContainer.readAll());
}


Model* ModelManager::createBaseModelContainer()
{
  qint32 modelType = baseModelType();

  if (modelType == 2)
    return new Model(modelType, QByteArray());

  QFile baseContainer(KStandardDirs::locate("appdata", "model/basemodel.sbm"));

  if (!baseContainer.open(QIODevice::ReadOnly))
    return 0;

  return new Model(modelType, baseContainer.readAll());
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


bool ModelManager::storeBaseModel(const QDateTime& changedTime, int baseModelType, const QByteArray& container)
{
  KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("BaseModelDate", changedTime);
  cGroup.writeEntry("BaseModelType", baseModelType);
  config.sync();

  QString repoPath = KStandardDirs::locateLocal("appdata", "model/base/srv" + changedTime.toString(Qt::ISODate) + ".sbm");
  //store both as selected base model and in the local repository
  foreach (const QString& path, QStringList() << KStandardDirs::locateLocal("appdata", "model/basemodel.sbm")
                                                    << repoPath) {
    QFile containerFile(path);
    if (!containerFile.open(QIODevice::WriteOnly))
      return false;
    containerFile.write(container);
    containerFile.close();
  }
  setBaseModel(repoPath, baseModelType);

  QDateTime creationDate;
  QString name;
  ModelMetadata *data = metaDataFromBuffer(container);
  if (data) {
    name = data->name();
    creationDate = data->dateTime();
    delete data;
  }
  announceBaseModel(name, baseModelType, creationDate);
  return true;
}

void ModelManager::announceBaseModel(const QString& name, int type, const QDateTime& creationDate)
{
  emit baseModelStored(type, creationDate, name);
}


int ModelManager::baseModelType()
{
  return SpeechModelManagementConfiguration::modelType();
}

QString ModelManager::baseModel()
{
  return SpeechModelManagementConfiguration::selectedBaseModel();
}

void ModelManager::setBaseModel(const QString& path, int type)
{
  SpeechModelManagementConfiguration::setSelectedBaseModel(path);
  SpeechModelManagementConfiguration::setModelType(type);
  SpeechModelManagementConfiguration::self()->writeConfig();
  ScenarioManager::getInstance()->touchBaseModelAccessTime();
}

QString ModelManager::languageProfileName()
{
  return SpeechModelManagementConfiguration::languageProfileName();
}
void ModelManager::setLanguageProfileName(const QString& name)
{
  SpeechModelManagementConfiguration::setLanguageProfileName(name);
  SpeechModelManagementConfiguration::self()->writeConfig();
}

ModelMetadata* ModelManager::metaData(KTar& tar)
{
  if (!tar.open(QIODevice::ReadOnly)) return 0;
  const KArchiveDirectory *d = tar.directory();
  if (!d) return 0;

  const KArchiveFile *entry = dynamic_cast<const KArchiveFile*>(d->entry("metadata.xml"));
  if (!entry) return 0;

  QDomDocument doc;
  doc.setContent(entry->data());

  return new ModelMetadata(doc.documentElement());
}

bool ModelManager::updateBlacklistedTranscriptions(ModelMetadata* data)
{
  blacklistedTranscriptions = data->droppedPronunciations();
  kDebug() << "Blacklisted transcriptions: " << blacklistedTranscriptions;
  return true;
}

bool ModelManager::storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& container)
{
  KConfig config( KStandardDirs::locateLocal("appdata", "model/activemodelrc"), KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("Date", changedTime);
  config.sync();

  SpeechModelManagementConfiguration::setModelSampleRate(sampleRate);

  QFile containerFile(KStandardDirs::locateLocal("appdata", "model/active.sbm"));

  if (!containerFile.open(QIODevice::WriteOnly))
    return false;

  containerFile.write(container);
  containerFile.close();

  bool success = false;
  QDateTime creationDate;
  QString name;
  ModelMetadata *data = metaDataFromBuffer(container);
  if (data) {
    success = updateBlacklistedTranscriptions(data);
    creationDate = data->dateTime();
    name = data->name();
    delete data;
  }
  emit activeModelStored(creationDate, name);
  return success;
}

ModelMetadata* ModelManager::metaDataFromBuffer(const QByteArray& input)
{
  ModelMetadata *data = 0;
  KTar *tar = archiveFromBuffer(input);
  if (tar) {
    data = metaData(*tar);
    QIODevice *d = tar->device();
    delete tar;
    delete d;
  }
  return data;
}

/**
 * \warning The caller has to delete the archive's underlying device
 */
KTar* ModelManager::archiveFromBuffer(const QByteArray& input)
{
  // safely remove the const qualifier as the buffer is only opened read-only
  QBuffer buffer(const_cast<QByteArray*>(&input));
  if (!buffer.open(QIODevice::ReadOnly))
    return 0;
  QIODevice *uncompressed = KFilterDev::device(&buffer, "application/x-gzip", false);
  if (!uncompressed || !uncompressed->open(QIODevice::ReadOnly))
    return 0;

  //seeking doesn't work properly in kfilterdevs, which trips up KTar. Instead, decompress
  //completely before passing it on to KTar
  QBuffer *uncompressedBuffer = new QBuffer;
  if (!uncompressedBuffer->open(QIODevice::ReadWrite))
    return 0;
  uncompressedBuffer->write(uncompressed->readAll());
  uncompressedBuffer->seek(0);
  delete uncompressed;

  return new KTar(uncompressedBuffer);
}

QByteArray ModelManager::getSample(const QString& sampleName)
{
  QString dirPath = QDir::toNativeSeparators(
    SpeechModelManagementConfiguration::modelTrainingsDataPath().path()+QDir::separator());
  #ifdef Q_OS_WIN32
  dirPath = dirPath.toUpper();
  #endif
  QString path = QDir::toNativeSeparators(dirPath+sampleName);

  QFile f(path);
  QFileInfo fInfo(f);
  QDir d(dirPath);
  if (!d.exists()) return QByteArray();

  //do not get tricked by /path/to/samples/../../../etc/passwd
  if (!fInfo.canonicalFilePath().contains(d.canonicalPath())) {
    kDebug() << fInfo.canonicalFilePath() << " does not contain " << d.canonicalPath();
    return QByteArray();
  }

  if (!f.open(QIODevice::ReadOnly))
    return QByteArray();
  return f.readAll();
}


LanguageDescriptionContainer* ModelManager::getLanguageDescriptionContainer()
{
  QFile shadowVocab(KStandardDirs::locate("appdata", "shadowvocabulary.xml"));
  QFile languageProfile(KStandardDirs::locate("appdata", "model/languageProfile"));

  QByteArray languageP;
  if (languageProfile.open(QIODevice::ReadOnly))
    languageP += languageProfile.readAll();

  return new LanguageDescriptionContainer((shadowVocab.open(QIODevice::ReadOnly)) ?
                shadowVocab.readAll() : QByteArray(), languageP);
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
    const QByteArray& languageProfile)
{
  ShadowVocabulary *vocab = ScenarioManager::getInstance()->getShadowVocabulary();

  QIODevice *vocabDev = 0;

  QString vocabTempPath = KStandardDirs::locateLocal("tmp", "simon/tmpshadow");
  QFile f(vocabTempPath);
  if (!f.open(QIODevice::WriteOnly))
    return false;
  f.write(shadowVocab);
  f.close();

  vocabDev = KFilterDev::deviceForFile(vocabTempPath, KMimeType::findByFileContent(vocabTempPath)->name());

  if (!vocab->reset(vocabDev) || !vocab->save()) return false;
  delete vocabDev;

  QFile languageProfileF(KStandardDirs::locateLocal("appdata", "model/languageProfile"));
  if (!languageProfileF.open(QIODevice::WriteOnly))
    return false;

  languageProfileF.write(languageProfile);
  languageProfileF.close();

  KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("LanguageDescriptionDate", changedTime);
  config.sync();
  return true;
}


TrainingContainer* ModelManager::getTrainingContainer()
{
  qint32 modelSampleRate=SpeechModelManagementConfiguration::modelSampleRate();

  QFile prompts(KStandardDirs::locate("appdata", "model/prompts"));

  if (!prompts.open(QIODevice::ReadOnly))
    return 0;

  return new TrainingContainer(modelSampleRate, prompts.readAll());
}


QDateTime ModelManager::getTrainingModifiedTime()
{
  if (!QFile::exists(KStandardDirs::locateLocal("appdata", "model/modelsrcrc")))
    return QDateTime();

  KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  return cGroup.readEntry("TrainingDate", QDateTime());
}


bool ModelManager::storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& prompts)
{
  if (!TrainingManager::getInstance()->refreshTraining(sampleRate, prompts))
    return false;

  KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("TrainingDate", changedTime);
  config.sync();
  return true;
}


void ModelManager::buildSampleList(QStringList& available, QStringList& missing)
{
  QStringList newList = TrainingManager::getInstance()->getPrompts()->keys();
  QDir samplesDir(SpeechModelManagementConfiguration::modelTrainingsDataPath().toLocalFile());
  QStringList oldList = samplesDir.entryList(QStringList() << "*.wav");

  foreach (QString fileName, newList) {
    fileName += +".wav";
    if (!oldList.contains(fileName)) {
      if (!missing.contains(fileName))
        missing << fileName;
    } else if (!available.contains(fileName))
      available << fileName;
  }
}


bool ModelManager::storeSample(const QString& name, const QByteArray& sample)
{
  QString dirPath = TrainingManager::getInstance()->getTrainingDir()+'/';

  QFile f(dirPath+name);
  if (!f.open(QIODevice::WriteOnly)) return false;

  f.write(sample);
  f.close();
  return true;
}


void ModelManager::touchLanguageDescription()
{
  KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("LanguageDescriptionDate", KDateTime::currentUtcDateTime().dateTime());
  config.sync();
  modelHasChanged();
}

bool ModelManager::hasTraining()
{
  if (getTrainingModifiedTime().isNull()) return false;
  return (QFile::exists(KStandardDirs::locate("appdata", "model/prompts")));
}


bool ModelManager::hasLanguageDescription()
{
  if (getLanguageDescriptionModifiedTime().isNull()) return false;
  return (QFile::exists(KStandardDirs::locate("appdata", "model/shadow.voca")));
}


bool ModelManager::hasActiveContainer()
{
  if (getActiveContainerModifiedTime().isNull()) return false;
  return QFile::exists(KStandardDirs::locate("appdata", "model/active.sbm"));
}

bool ModelManager::isTranscriptionBlackListed(const QString& transcription)
{
  return blacklistedTranscriptions.contains(transcription);
}
