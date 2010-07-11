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

#include <simonscenarios/model.h>
#include <simonscenarios/languagedescriptioncontainer.h>
#include <simonscenarios/trainingcontainer.h>
#include <simonscenarios/scenario.h>

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

SynchronisationManager::SynchronisationManager(const QString& user_name, QObject* parent) : QObject(parent),
username(user_name),
srcContainerTempPath(KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName() + '/' + user_name + "/sync/"))
{
}


void SynchronisationManager::buildMissingScenarios(const QStringList& remoteScenarioList)
{
  missingScenarios.clear();
  commonScenarios.clear();

  QStringList localScenarioList = getAllScenarioIds();

  foreach (const QString& rId, remoteScenarioList) {
    if (!localScenarioList.contains(rId))
      missingScenarios << rId;
    else commonScenarios << rId;
  }
}


QStringList SynchronisationManager::getAllScenarioIds()
{
  QStringList paths = getAllScenarios();
  QStringList ids;
  foreach (const QString& path, paths) {
    QFileInfo fi(path);
    if (!ids.contains(fi.fileName()))
      ids << fi.fileName();
  }
  return ids;
}


QStringList SynchronisationManager::getAllScenarios()
{
  QStringList scenarioIds;
  QMap<QDateTime, QString> models = getModels();
  if (models.isEmpty()) return scenarioIds;

  QMap<QDateTime, QString>::iterator i = models.end();
  while (i != models.begin()) {
    --i;
    QString path = i.value()+QDir::separator()+"scenarios"+QDir::separator();
    QDir d(path);
    QStringList ids = d.entryList(QDir::Files|QDir::NoDotAndDotDot);
    foreach (const QString& id, ids)
      if (!scenarioIds.contains(id))
      scenarioIds << path+QDir::separator()+id;
  }
  return scenarioIds;
}


void SynchronisationManager::couldntRetreiveScenario()
{
  if (!missingScenarios.isEmpty())
    missingScenarios.removeAt(0);
  else if (!commonScenarios.isEmpty())
    commonScenarios.removeAt(0);
}


void SynchronisationManager::scenarioUpToDate()
{
  commonScenarios.removeAt(0);
}


QString SynchronisationManager::missingScenario()
{
  if (!missingScenarios.isEmpty())
    return missingScenarios.at(0);
  return QString();
}


QString SynchronisationManager::commonScenario()
{
  if (!commonScenarios.isEmpty())
    return commonScenarios.at(0);
  return QString();
}


void SynchronisationManager::scenarioSynchronized()
{
  if (!commonScenarios.isEmpty()) {
    commonScenarios.removeAt(0);
  } else if (!missingScenarios.isEmpty())
  missingScenarios.removeAt(0);
}


bool SynchronisationManager::storeScenario(const QByteArray& scenario)
{
  QString scenarioId = missingScenario();
  if (scenarioId.isNull())
    scenarioId = commonScenario();

  kDebug() << "Storing scenario at id: " << scenarioId;
  scenarioSynchronized();

  QString path = srcContainerTempPath+QDir::separator()+"scenarios"+QDir::separator()+scenarioId;
  QFile f(path);
  if (!f.open(QIODevice::WriteOnly)) {
    kDebug() << "Cannot store scenario at: " << path;
    return false;
  }

  f.write(scenario);
  return true;
}


QByteArray SynchronisationManager::getScenario(const QString& scenarioId)
{
  QString path = getLatestScenarioPath(scenarioId);
  QFile f(path);
  if (!f.open(QIODevice::ReadOnly))
    return QByteArray();
  return f.readAll();
}


QString SynchronisationManager::getLatestScenarioPath(const QString& id)
{
  QStringList paths = getAllScenarios();
  foreach (const QString& path, paths) {
    QFileInfo fi(path);
    if (fi.fileName() == id) {
      return path;
    }
  }
  kDebug() << "Latest scenario path of scenario: " << id << " not found!";
  return QString();
}


QDateTime SynchronisationManager::localScenarioDate(const QString& scenarioId)
{
  QString path = getLatestScenarioPath(scenarioId);
  if (path.isNull()) return QDateTime();

  return scenarioDate(path);
}


QDateTime SynchronisationManager::scenarioDate(const QString& path)
{
  Scenario *s = new Scenario("");
  if (!s->skim(path)) {
    delete s;
    return QDateTime();
  }
  QDateTime t = s->modifiedDate();
  delete s;
  return t;
}


Model* SynchronisationManager::getActiveModel()
{
  if (username.isEmpty()) return 0;

  QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");

  QString configPath = dirPath+"activerc";
  KConfig config( configPath, KConfig::SimpleConfig );
  qint32 sampleRate;
  KConfigGroup cGroup(&config, "");
  bool ok;
  sampleRate = cGroup.readEntry("SampleRate").toInt(&ok);
  if (!ok) return 0;

  QFile hmmDefs(dirPath+"hmmdefs");
  QFile tiedlist(dirPath+"tiedlist");
  QFile dict(dirPath+"model.dict");
  QFile dfa(dirPath+"model.dfa");

  if ((!hmmDefs.open(QIODevice::ReadOnly)) ||
    (!tiedlist.open(QIODevice::ReadOnly)) ||
    (!dict.open(QIODevice::ReadOnly)) ||
  (!dfa.open(QIODevice::ReadOnly))) {
    kDebug() << "Failed to gather active model";
    return 0;
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
    return false;

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


QDateTime SynchronisationManager::getBaseModelDate()
{
  QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
  KConfig config( dirPath+"activerc", KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  return cGroup.readEntry("BaseModelDate", QDateTime());
}


Model* SynchronisationManager::getBaseModel()
{
  if (username.isEmpty()) return 0;

  QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");

  QString configPath = dirPath+"activerc";
  KConfig config( configPath, KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");

  bool ok;
  qint32 baseModelType = cGroup.readEntry("BaseModelType").toInt(&ok);
  if (!ok) return 0;

  QFile hmmDefs(dirPath+"basehmmdefs");
  QFile tiedlist(dirPath+"basetiedlist");
  QFile macrosFile(dirPath+"basemacros");
  QFile statsFile(dirPath+"basestats");

  if ((!hmmDefs.open(QIODevice::ReadOnly)) ||
    (!tiedlist.open(QIODevice::ReadOnly)) ||
    (!macrosFile.open(QIODevice::ReadOnly)) ||
  (!statsFile.open(QIODevice::ReadOnly))) {
    kDebug() << "Failed to gather active model";
    return 0;
  }

  return new Model(baseModelType, hmmDefs.readAll(), tiedlist.readAll(),
    macrosFile.readAll(), statsFile.readAll());
}


int SynchronisationManager::getBaseModelType()
{
  if (username.isEmpty()) return 0;

  QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
  QString configPath = dirPath+"activerc";
  KConfig config( configPath, KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");

  bool ok;
  int baseModelType = cGroup.readEntry("BaseModelType").toInt(&ok);
  if (!ok) return 2;
  return baseModelType;
}


bool SynchronisationManager::storeBaseModel(const QDateTime& changedDate, int modelType,
const QByteArray& hmmDefs, const QByteArray& tiedList,
const QByteArray& macros, const QByteArray& stats)
{
  if (username.isEmpty()) return false;

  QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
  QFile hmmDefsFile(dirPath+"basehmmdefs");
  QFile tiedListFile(dirPath+"basetiedlist");
  QFile macrosFile(dirPath+"basemacros");
  QFile statsFile(dirPath+"basestats");

  if ((!hmmDefsFile.open(QIODevice::WriteOnly))
    || (!tiedListFile.open(QIODevice::WriteOnly))
    || (!macrosFile.open(QIODevice::WriteOnly))
    || (!statsFile.open(QIODevice::WriteOnly)))
    return false;

  hmmDefsFile.write(hmmDefs);
  tiedListFile.write(tiedList);
  macrosFile.write(macros);
  statsFile.write(stats);

  hmmDefsFile.close();
  tiedListFile.close();
  macrosFile.close();
  statsFile.close();

  KConfig config( dirPath+"activerc", KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("BaseModelDate", changedDate);
  cGroup.writeEntry("BaseModelType", modelType);
  config.sync();
  return true;
}


void SynchronisationManager::modelCompiled()
{
  QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
  KConfig config( dirPath+"activerc", KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("Date", QDateTime::currentDateTime());
  config.sync();
}


QString SynchronisationManager::getLatestLanguageDescriptionPath()
{
  QMap<QDateTime, QString> langDescs = getLanguageDescriptions();
  if (langDescs.isEmpty()) return QString();

  QList<QDateTime> dates = langDescs.keys();
  return langDescs.value(dates.at(dates.count()-1));
}


QDateTime SynchronisationManager::getLanguageDescriptionDate(QString path)
{
  if (path.isEmpty())
    path = getLatestLanguageDescriptionPath();
  if (path.isNull()) return QDateTime();

  KConfig config( path+"modelsrcrc", KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  return cGroup.readEntry("LanguageDescriptionDate", QDateTime());
}


bool SynchronisationManager::hasLanguageDescription(const QString& modelPath)
{
  if (modelPath.isEmpty())
    return !getLanguageDescriptions().isEmpty();

  QDir dir(modelPath);
  QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
  if (entries.contains("shadowlexicon.xml") &&
    entries.contains("tree1.hed"))
    return true;
  else return false;
}


LanguageDescriptionContainer* SynchronisationManager::getLanguageDescription()
{
  QString path = getLatestLanguageDescriptionPath();
  if (path.isNull()) return 0;

  QFile treeHed(path+"tree1.hed");
  QFile shadowVocab(path+"shadowlexicon.xml");

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
  QFile shadowVocabFile(srcContainerTempPath+"shadowlexicon.xml");

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


QString SynchronisationManager::getLatestTrainingPath()
{
  QMap<QDateTime, QString> trainings = getTrainingDatas();
  if (trainings.isEmpty()) return QString();

  QList<QDateTime> dates = trainings.keys();
  return trainings.value(dates.at(dates.count()-1));
}


QDateTime SynchronisationManager::getTrainingDate(QString path)
{
  if (path.isEmpty())
    path = getLatestTrainingPath();
  if (path.isNull()) return QDateTime();

  KConfig config( path+"modelsrcrc", KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  return cGroup.readEntry("TrainingDate", QDateTime());
}


bool SynchronisationManager::hasTraining(const QString& modelPath)
{
  if (modelPath.isEmpty())
    return !getTrainingDatas().isEmpty();

  QDir dir(modelPath);

  QStringList entries = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
  if (entries.contains("wav_config") &&
    entries.contains("trainingrc") &&
    entries.contains("prompts"))
    return true;
  else return false;
}


TrainingContainer* SynchronisationManager::getTraining()
{
  QString path = getLatestTrainingPath();
  KConfig config( path+"trainingrc", KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  qint32 sampleRate = cGroup.readEntry("SampleRate", 16000);

  QFile wavConfig(path+"wav_config");

  if (!wavConfig.open(QIODevice::ReadOnly))
    return 0;

  QFile prompts(path+"prompts");
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

  missingFiles.clear();                           //rebuilding this...

  if (promptsFile.open(QIODevice::ReadOnly)) {
    while (!promptsFile.atEnd()) {
      QString promptsLine = QString::fromUtf8(promptsFile.readLine());
      newList << promptsLine.left(promptsLine.indexOf(' '));
    }
    promptsFile.close();
  }

  foreach (const QString& fileName, newList) {
    if ((!oldList.contains(fileName+".wav")) && (!this->missingFiles.contains(fileName)))
      missingFiles << fileName;
  }

}


QByteArray SynchronisationManager::getSample(const QString& sampleName)
{
  QString dirPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/");

  #ifdef Q_OS_WIN
  dirPath = dirPath.toUpper();
  #ifdef UNICODE
  QFile f(dirPath+QDir::separator()+sampleName.toUtf8());
  #else
  QFile f(dirPath+QDir::separator()+sampleName.toLatin1());
  #endif
  #else
  QFile f(dirPath+QDir::separator()+sampleName.toAscii());
  #endif

  QDir d(dirPath);
  if (!d.exists()) return QByteArray();

  QFileInfo fInfo(f);
  //do not get tricked by /path/to/samples/../../../etc/passwd
  if (!fInfo.canonicalFilePath().contains(d.canonicalPath()))
    return QByteArray();

  kDebug() << "Retrieving " << dirPath+'/'+sampleName.toUtf8();
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


QDateTime SynchronisationManager::getCompileModelSrcDate()
{
  QDateTime trainingDate = getTrainingDate();
  QDateTime scenarioRcDate = selectedScenariosDate();
  QDateTime baseModelDate = getBaseModelDate();
  kDebug() << trainingDate << scenarioRcDate << baseModelDate;

  int baseModelType = getBaseModelType();
  if (((baseModelType == 2) && trainingDate.isNull()) || scenarioRcDate.isNull() ||
  ((baseModelType != 2) && (baseModelDate.isNull()))) {
    return QDateTime();
  }

  QDateTime srcDate = qMax(trainingDate, scenarioRcDate);
  srcDate = qMax(srcDate, baseModelDate);

  QStringList scenarios = getLatestSelectedScenarioList();
  foreach (const QString& scenarioId, scenarios) {
    srcDate = qMax(srcDate, localScenarioDate(scenarioId));
  }

  return srcDate;
}


QDateTime SynchronisationManager::getModelSrcDate()
{
  QDateTime compileSrcDate = getCompileModelSrcDate();
  if (compileSrcDate.isNull()) return QDateTime();

  return qMax(compileSrcDate, getLanguageDescriptionDate());
}


bool SynchronisationManager::hasModelSrc()
{
  //language description needed for tree.hed
  int baseModelType = getBaseModelType();
  return ((getLatestSelectedScenarioList().count() > 0) &&
    ((baseModelType != 2) || (hasTraining() && hasLanguageDescription())));
}


bool SynchronisationManager::shouldRecompileModel()
{
  kDebug() << getActiveModelDate();
  kDebug() << getCompileModelSrcDate();
  kDebug() << hasModelSrc();
  return ((getActiveModelDate() < getCompileModelSrcDate())
    && (hasModelSrc()));
}


bool SynchronisationManager::startSynchronisation()
{
  if (QFile::exists(srcContainerTempPath+"lock")) {
    kDebug() << "Lock exists";
    return false;
  }

  QDir d(srcContainerTempPath);
  d.mkdir("scenarios");

  QFile lock(srcContainerTempPath+"lock");
  if (!lock.open(QIODevice::WriteOnly)) {
    kDebug() << "Cannot open lock file at" << srcContainerTempPath+"lock";
    return false;
  }

  lock.write("Congratulations! You have discovered a secret message!");
  lock.close();
  return true;
}


bool SynchronisationManager::removeAllFiles(const QString& dir)
{
  QDir tempDir(dir);
  QStringList files = tempDir.entryList(QDir::Files|QDir::NoDotAndDotDot);
  bool allRemoved=true;
  foreach (const QString& file, files) {
    if (!QFile::remove(dir+QDir::separator()+file)) {
      allRemoved=false;
    }
  }
  return allRemoved;
}


bool SynchronisationManager::cleanTemp()
{
  return removeDirectory(srcContainerTempPath) && QDir().mkpath(srcContainerTempPath)
    && removeExcessModelBackups();
  /*	bool allRemoved = removeAllFiles(srcContainerTempPath+QDir::separator()+"scenarios");
    kDebug() << allRemoved;

    allRemoved = removeAllFiles(srcContainerTempPath) && allRemoved;
    kDebug() << allRemoved;

    return (allRemoved && removeExcessModelBackups());*/
}


bool SynchronisationManager::abort()
{
  return cleanTemp();
}


QDateTime SynchronisationManager::getSelectedScenarioListModifiedDateFromPath(const QString& path)
{
  KConfig config( path, KConfig::SimpleConfig );
  KConfigGroup cg(&config, "");
  return cg.readEntry("LastModified", QDateTime());
}


bool SynchronisationManager::commit()
{
                                                  //ARGH! someone could have screwed with this model!
  if (!QFile::exists(srcContainerTempPath+"lock")) return false;

  KConfig config( srcContainerTempPath+"modelsrcrc", KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");

  QDateTime newSrcContainerTime = qMax(cGroup.readEntry("LanguageDescriptionDate", QDateTime()),
    cGroup.readEntry("TrainingDate", QDateTime()));
  //kDebug() << "LanguageDescriptionDate:" << cGroup.readEntry("LanguageDescriptionDate", QDateTime());
  //kDebug() << "TrainingDate:" << cGroup.readEntry("TrainingDate", QDateTime());
  //kDebug() << newSrcContainerTime;
  //kDebug() << "SelectedScenarioListModifiedDate:" << getSelectedScenarioListModifiedDateFromPath(srcContainerTempPath+QDir::separator()+"simonscenariosrc");

  //***************
  newSrcContainerTime = qMax(newSrcContainerTime, getSelectedScenarioListModifiedDateFromPath(srcContainerTempPath+QDir::separator()+"simonscenariosrc"));

  QDir scenarioDir(srcContainerTempPath+"scenarios");
  QStringList scenarios = scenarioDir.entryList(QDir::Files|QDir::NoDotAndDotDot);
  foreach (const QString& scenarioPath, scenarios) {
    QDateTime sDate = scenarioDate(srcContainerTempPath+"scenarios"+QDir::separator()+scenarioPath);
    kDebug() << "Merging scenario: " << scenarioPath << sDate;
    newSrcContainerTime = qMax(newSrcContainerTime, sDate);
  }

  kDebug() << "New container time: " << newSrcContainerTime;
  if (newSrcContainerTime.isNull()) {
    kDebug() << "Calling cleanTemp";
    return cleanTemp();                           // nothing to process
  }

  QString newSrcContainerPath = KStandardDirs::locateLocal("appdata", "models/"+username+"/src/"+newSrcContainerTime.toString("yyyy-MM-dd_hh-mm-ss")+'/');

  if (newSrcContainerPath.isEmpty()) return false;

  bool allCopied=true;
  if (hasTraining(srcContainerTempPath) && !copyTrainingData(srcContainerTempPath, newSrcContainerPath)) allCopied=false;
  kDebug() << "Copied training successfully: " << allCopied;
  if (hasLanguageDescription(srcContainerTempPath) && !copyLanguageDescription(srcContainerTempPath, newSrcContainerPath)) allCopied=false;
  kDebug() << "Copied lang desc successfully: " << allCopied;
  if (hasScenarioRc(srcContainerTempPath) && !copyScenarioRc(srcContainerTempPath, newSrcContainerPath)) allCopied = false;
  kDebug() << "Copied scenariorc successfully: " << allCopied;
  if (!copyScenarios(srcContainerTempPath, newSrcContainerPath)) allCopied = false;
  kDebug() << "Copied scenarios successfully: " << allCopied;

  if (!allCopied) {
    kDebug() << "Failed to copy all files. Aborting";
    cleanTemp();
    return false;
  }

  return cleanTemp();
}


QMap<QDateTime, QString> SynchronisationManager::getModels()
{
  QMap<QDateTime, QString> models;

  QDir modelSrcDir(KStandardDirs::locateLocal("appdata", "models/"+username+"/src/"));
  if (!modelSrcDir.exists()) return models;

  QStringList folders = modelSrcDir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
  foreach (const QString& folder, folders) {
    QDateTime folderDate = QDateTime::fromString(folder, "yyyy-MM-dd_hh-mm-ss");
    if (!folderDate.isValid()) continue;

    models.insert(folderDate, modelSrcDir.absolutePath()+QDir::separator()+folder+QDir::separator());
  }
  return models;
}


QMap<QDateTime, QString> SynchronisationManager::getTrainingDatas()
{
  QMap<QDateTime, QString> models = getModels();

  //remove every model that does not contain valid trainingsdatas
  QMap<QDateTime, QString>::iterator i = models.begin();
  while (i != models.end()) {
    QString path = i.value()+QDir::separator();
    if (!QFile::exists(path+"trainingrc") ||
      !QFile::exists(path+"prompts") ||
    !QFile::exists(path+"wav_config")) {
      i = models.erase(i);
    } else
        ++i;
  }
  return models;
}


QMap<QDateTime, QString> SynchronisationManager::getLanguageDescriptions()
{
  QMap<QDateTime, QString> models = getModels();

  //remove every model that does not contain a language description
  QMap<QDateTime, QString>::iterator i = models.begin();
  while (i != models.end()) {
    QString path = i.value()+QDir::separator();
    if (!QFile::exists(path+"tree1.hed")||
    !QFile::exists(path+"shadowlexicon.xml")) {
      //does not contain a valid language description
      i = models.erase(i);
    } else
        ++i;
  }
  return models;
}


bool SynchronisationManager::switchToModel(const QDateTime& modelDate)
{
  if (!startSynchronisation()) return false;

  QString trainingPath, languageDescriptionPath, scenarioRcPath;

  QMap<QDateTime,QString> trainingModels = getTrainingDatas();
  for (QMap<QDateTime,QString>::const_iterator i=trainingModels.constBegin();
    (i != trainingModels.constEnd()) && (i.key() <= modelDate); i++)
  trainingPath = i.value();

  QMap<QDateTime,QString> languageDescriptionModels = getLanguageDescriptions();
  for (QMap<QDateTime,QString>::const_iterator i=languageDescriptionModels.constBegin();
    (i != languageDescriptionModels.constEnd()) && (i.key()<=modelDate); i++)
  languageDescriptionPath = i.value();

  QMap<QDateTime,QString> scenarioRcModels = getSelectedScenarioLists();
  for (QMap<QDateTime,QString>::const_iterator i=scenarioRcModels.constBegin();
    (i != scenarioRcModels.constEnd()) && (i.key()<=modelDate); i++)
  scenarioRcPath = i.value();

  if (languageDescriptionPath.isEmpty() || scenarioRcPath.isEmpty()) {
    abort();
    kDebug() << "One of the paths is empty";
    kDebug() << languageDescriptionPath << scenarioRcPath;
    return false;
  }

  bool allCopied=true;
  if (trainingPath.isEmpty())
    allCopied &= createTrainingData(srcContainerTempPath);
  else
    allCopied &= copyTrainingData(trainingPath, srcContainerTempPath);
  if (!copyLanguageDescription(languageDescriptionPath, srcContainerTempPath)) allCopied=false;

  //copy scenariosrc from closest  model date
  if (!copyScenarioRc(scenarioRcPath, srcContainerTempPath)) allCopied=false;

  //copy all scenarios from that date on to the current model path:
  //	Simply call copyScenarios for every earlier scenario path to accumulate all scenarios
  QMap<QDateTime,QString> allSrcModels = getModels();
  QMap<QDateTime,QString>::const_iterator modelI = allSrcModels.constEnd();
  if (allSrcModels.isEmpty()) {
    kDebug() << "No src models found...?";
    return false;
  }

  kDebug() << "Switching to this model: " << modelDate;

  do {
    --modelI;
    kDebug() << "Current model: " << modelI.key();
    if (modelI.key() <= modelDate) {
      kDebug() << "Copying scenarios...";
      allCopied &= copyScenarios(modelI.value(), srcContainerTempPath, true);
    } else kDebug() << "Not relevant";
  }
  while (modelI != allSrcModels.constBegin());
  kDebug() << "modelI is now at begin" << modelI.key();

  if (!allCopied) {
    abort();
    kDebug() << "could not copy everything";
    return false;
  }

  //updating date stamp
  touchTempModel();

  kDebug() << "Committing";
  return commit();
}


void SynchronisationManager::touchTempModel()
{
  QDateTime newModelDate = QDateTime::currentDateTime();
  KConfig config( srcContainerTempPath+"modelsrcrc", KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("TrainingDate", newModelDate);
  cGroup.writeEntry("LanguageDescriptionDate", newModelDate);
  config.sync();

  KConfig scenarioRc( srcContainerTempPath+"simonscenariosrc", KConfig::SimpleConfig );
  KConfigGroup scenarioRcGroup(&scenarioRc, "");
  scenarioRcGroup.writeEntry("LastModified", newModelDate);
  scenarioRc.sync();
}


bool SynchronisationManager::createTrainingData(const QString& dest)
{
  bool allFine=true;
  QFile promptsF(dest+"prompts");
  allFine &= promptsF.open(QIODevice::WriteOnly);
  promptsF.close();
  QFile trainingrcF(dest+"trainingrc");
  allFine &= trainingrcF.open(QIODevice::WriteOnly);
  trainingrcF.close();
  QFile wavConfigF(dest+"wav_config");
  allFine &= wavConfigF.open(QIODevice::WriteOnly);
  wavConfigF.close();

  KConfig config( dest+"modelsrcrc", KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("TrainingDate", QDateTime::currentDateTime());
  config.sync();

  return allFine;
}


bool SynchronisationManager::copyTrainingData(const QString& sourcePath, const QString& targetPath)
{
  bool allFine=true;
  if (!QFile::exists(targetPath+"prompts") || !QFile::exists(targetPath+"trainingrc") || !QFile::exists(targetPath+"wav_config")) {
    if (!QFile::copy(sourcePath+"prompts", targetPath+"prompts")) allFine=false;
    if (!QFile::copy(sourcePath+"trainingrc", targetPath+"trainingrc")) allFine=false;
    if (!QFile::copy(sourcePath+"wav_config", targetPath+"wav_config")) allFine=false;

    KConfig config( targetPath+"modelsrcrc", KConfig::SimpleConfig );
    KConfigGroup cGroup(&config, "");
    cGroup.writeEntry("TrainingDate", getTrainingDate(sourcePath));
    config.sync();
  }

  return allFine;
}


bool SynchronisationManager::copyLanguageDescription(const QString& sourcePath, const QString& targetPath)
{
  bool allFine=true;
  if (!QFile::exists(targetPath+"shadowlexicon.xml") || !QFile::exists(targetPath+"tree1.hed")) {
    if (!QFile::copy(sourcePath+"shadowlexicon.xml", targetPath+"shadowlexicon.xml")) allFine=false;
    if (!QFile::copy(sourcePath+"tree1.hed", targetPath+"tree1.hed")) allFine=false;

    KConfig config( targetPath+"modelsrcrc", KConfig::SimpleConfig );
    KConfigGroup cGroup(&config, "");
    cGroup.writeEntry("LanguageDescriptionDate", getLanguageDescriptionDate(sourcePath));
    config.sync();
  }

  return allFine;
}


bool SynchronisationManager::copyScenarios(const QString& source, const QString& dest, bool touchAccessTime)
{
  kDebug() << "Copying scenarios from " << source << " to " << dest;
  QDir scenarioDir(source+QDir::separator()+"scenarios");

  //problems with source or dest?
  QDir d(dest+QDir::separator()+"scenarios");
  if (!scenarioDir.exists() || (!d.exists() && !d.mkpath(dest+QDir::separator()+"scenarios"))) {
    kDebug() << "Failed to create target folders";
    kDebug() << "scenariodir exists: " << scenarioDir.exists();
    kDebug() << "d exists: " << d.exists();
    kDebug() << "d mkpath: " << d.mkpath(dest+QDir::separator()+"scenarios");
    return false;
  }

  bool allCopied=true;

  QStringList scenarios = scenarioDir.entryList(QDir::Files|QDir::NoDotAndDotDot);
  foreach (const QString& scenario, scenarios) {
    QString scenarioPath = source+QDir::separator()+"scenarios"+QDir::separator()+scenario;
    QString destPath = dest+QDir::separator()+"scenarios"+QDir::separator()+scenario;

    kDebug() << "Copying scenario: " << scenarioPath << destPath;

    if (QFile::exists(destPath))
      continue;

    if (!QFile::copy(scenarioPath, destPath)) {
      allCopied = false;
      kDebug() << "Error: Copy failed";
    }
    else {
      kDebug() << "Copying successful";
      if (touchAccessTime) {
        kDebug() << "Touching scenario! " << destPath;
        //touch it
        QDomDocument doc("scenario");
        QFile file(destPath);
        if ((!file.open(QIODevice::ReadWrite))
        || (!doc.setContent(&file))) {
          allCopied = false;
          continue;
        }

        doc.documentElement().setAttribute("lastModified", QDateTime::currentDateTime().toString(Qt::ISODate));

        file.seek(0);
        file.write(doc.toString().toUtf8());

        file.close();
      }
    }
  }
  kDebug() << "All scenarios copied: " << allCopied;

  return allCopied;
}


bool SynchronisationManager::copyScenarioRc(const QString& source, const QString& dest)
{
  QString rcSource = source+QDir::separator()+"simonscenariosrc";
  QString rcDest = dest+QDir::separator()+"simonscenariosrc";
  if ((!QFile::exists(rcDest)) && !QFile::copy(rcSource, rcDest)) {
    kDebug() << "Failed to copy scenario rc from " << source+QDir::separator()+"simonscenariosrc" << "to "
      << dest+QDir::separator()+"simonscenariosrc";
    return false;
  }

  return true;
}


void SynchronisationManager::deletedScenarios(const QStringList& ids, const QList<QDateTime>& scenarioTimes)
{
  if (ids.count() != scenarioTimes.count()) {
    kWarning() << "Error when retreiving the scenarios to delete";
    return;
  }

  for (int i=0; i < ids.count(); i++) {
    kDebug() << "Deleted scenario: " << ids[i];
    QDateTime localDate = localScenarioDate(ids[i]);

    if (localDate > scenarioTimes[i])
      //server version is newer than the deleted one from the client
      //keep it
      continue;

    QString path;
    do {
      path = getLatestScenarioPath(ids[i]);
    } while (!path.isEmpty() && QFile::remove(path));
  }
}


bool SynchronisationManager::hasScenarioRc(const QString& modelPath)
{
  return QFile::exists(modelPath+QDir::separator()+"simonscenariosrc");
}


bool SynchronisationManager::removeExcessModelBackups()
{
  kDebug() << "Removing excess model backups";
  KConfig config(KStandardDirs::locate("config", "simondrc"));
  KConfigGroup cGroup(&config, "ModelManagement");
  int maxBackupedModels = cGroup.readEntry("ModelBackups", 8);

  QMap<QDateTime, QString> models = getModels();

  //date is ascending so the we can remove from the front until we have removed enough
  //	QMutableMapIterator<QDateTime, QString> i(models);
  while ((models.count() > maxBackupedModels) && (models.count() >= 2)) {
    QString modelToRemovePath = models.value(models.keys().at(0));
    QString modelTargetPath = models.value(models.keys().at(1));

    //copy functions will not override if the same files are already in the target path
    if (hasLanguageDescription(modelToRemovePath) && !copyLanguageDescription(modelToRemovePath, modelTargetPath)) {
      kDebug() << "Language description failed";
      return false;
    }
    if (hasTraining(modelToRemovePath) && !copyTrainingData(modelToRemovePath, modelTargetPath)) {
      kDebug() << "Training failed";
      return false;
    }
    if (hasScenarioRc(modelToRemovePath) && !copyScenarioRc(modelToRemovePath, modelTargetPath)) {
      kDebug() << "ScenarioRc failed";
      return false;
    }
    if (!copyScenarios(modelToRemovePath, modelTargetPath)) {
      kDebug() << "Scenarios failed";
      return false;
    }

    //remove modelToRemovePath
    if (!removeDirectory(modelToRemovePath)) {
      kDebug() << "Removing directory failed: " << modelToRemovePath;
      return false;
    }

    kDebug() << "Removed " << modelToRemovePath;
    models.remove(models.keys().at(0));
  }

  return true;
}


bool SynchronisationManager::removeDirectory(const QString& dir)
{
  QDir directory(dir);
  QStringList dirs = directory.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
  foreach (const QString& d, dirs)
  if (!removeDirectory(dir+QDir::separator()+d)) {
    kDebug() << "Sub directory remove failed: " << d;
    return false;
  }

  if (!removeAllFiles(dir)) {
    kDebug() << "removeAllFiles failed";
    return false;
  }

  bool rmdir = directory.rmdir(dir);
  kDebug() << "RmDir: " << rmdir;
  return rmdir;
}


QDateTime SynchronisationManager::selectedScenariosDate()
{
  return getSelectedScenarioListModifiedDateFromPath(getLatestSelectedScenarioListPath());
}


QMap<QDateTime, QString> SynchronisationManager::getSelectedScenarioLists()
{
  QMap<QDateTime, QString> models = getModels();

  //remove every model that does not contain valid selected scenarios list
  QMap<QDateTime, QString>::iterator i = models.begin();
  while (i != models.end()) {
    QString path = i.value()+QDir::separator();
    if (!QFile::exists(path+"simonscenariosrc"))
      i = models.erase(i);
    else
      ++i;
  }
  return models;
}


QString SynchronisationManager::getLatestSelectedScenarioListPath()
{
  QString dir = getLatestPath(getSelectedScenarioLists());
  if (dir.isNull()) return QString();

  return dir+QDir::separator()+"simonscenariosrc";
}


QStringList SynchronisationManager::getLatestSelectedScenarioList()
{
  KConfig config( getLatestSelectedScenarioListPath(), KConfig::SimpleConfig );
  KConfigGroup cg(&config, "");
  return cg.readEntry("SelectedScenarios", QStringList());
}


bool SynchronisationManager::storeSelectedScenarioList(const QDateTime& modifiedDate, const QStringList& scenarioIds)
{
  QString path = srcContainerTempPath+QDir::separator()+"simonscenariosrc";
  KConfig config( path, KConfig::SimpleConfig );
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("LastModified", modifiedDate);
  cGroup.writeEntry("SelectedScenarios", scenarioIds);
  config.sync();
  kDebug() << "Stored simonscenariorc to " << srcContainerTempPath+QDir::separator()+"simonscenariosrc";
  return true;
}


QString SynchronisationManager::getLatestPath(const QMap<QDateTime, QString>& models)
{
  QList<QDateTime> dates = models.keys();
  if (dates.isEmpty()) return QString();

  return models.value(dates.at(dates.count()-1));
}


QStringList SynchronisationManager::getScenarioPaths()
{
  QStringList srcs = getLatestSelectedScenarioList();
  QStringList out;
  foreach (const QString& id, srcs)
    out << getLatestScenarioPath(id);

  return out;
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
