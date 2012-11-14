/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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

#include "contextadapter.h"
#include "situation.h"
#include "cachedmodel.h"
#include "modelsource.h"
#include <speechmodelcompilation/modelcompilation.h>
#include <speechmodelcompilation/modelcompilationmanager.h>
#include <speechmodelcompilation/modelcompilationmanagerhtk.h>
#include <speechmodelcompilation/modelcompilationmanagersphinx.h>
#include <simonutils/fileutils.h>
#include <simonscenarios/model.h>
#include <QDir>
#include <QSettings>
#include <KStandardDirs>
#include <KDebug>
#include <KAboutData>
#include <KDateTime>
#include <KTar>
#include <KConfigGroup>

ContextAdapter::ContextAdapter(QString username, QObject *parent) :
  QObject(parent),
  m_username(username),
  m_modelCompilationManager(0),
  m_currentSource(0)
{
  readCachedModels();
  setupBackend(ContextAdapter::FromConfiguration);
}

ContextAdapter::BackendType ContextAdapter::getConfiguredDefaultBackendType()
{
  KConfig config( KStandardDirs::locateLocal("config", "simonmodelcompilationrc"), KConfig::FullConfig );
  KConfigGroup backendGroup(&config, "Backend");
  int type(-1);
  type = backendGroup.readEntry("backend", 0);
  if (type == 0)
    return ContextAdapter::SPHINX;

  return ContextAdapter::HTK;
}

void ContextAdapter::setupBackend(ContextAdapter::BackendType backendType)
{
  kDebug() << "Setting up backend: " << backendType;

  if (backendType == ContextAdapter::FromConfiguration) {
    backendType = getConfiguredDefaultBackendType();
  }

  if (((backendType == ContextAdapter::SPHINX) && dynamic_cast<ModelCompilationManagerSPHINX*>(m_modelCompilationManager)) ||
      ((backendType == ContextAdapter::HTK) && dynamic_cast<ModelCompilationManagerHTK*>(m_modelCompilationManager)))
    return; // already set up

  ModelCompilationManager *old = m_modelCompilationManager;
  if(backendType == SPHINX)
    m_modelCompilationManager = new ModelCompilationManagerSPHINX(m_username, this);
  else
    m_modelCompilationManager = new ModelCompilationManagerHTK(m_username, this);
  delete old;

  connect(m_modelCompilationManager, SIGNAL(modelReady(uint, QString)), this, SLOT(slotModelReady(uint,QString)));
  connect(m_modelCompilationManager, SIGNAL(modelCompilationAborted(ModelCompilation::AbortionReason)), this, SLOT(slotModelCompilationAborted(ModelCompilation::AbortionReason)));

  connect(m_modelCompilationManager, SIGNAL(classUndefined(QString)), this, SIGNAL(classUndefined(QString)));
  connect(m_modelCompilationManager, SIGNAL(wordUndefined(QString)), this, SIGNAL(wordUndefined(QString)));
  connect(m_modelCompilationManager, SIGNAL(phonemeUndefined(QString)), this, SIGNAL(phonemeUndefined(QString)));
  connect(m_modelCompilationManager, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
  connect(m_modelCompilationManager, SIGNAL(status(QString, int, int)), this, SIGNAL(status(QString, int, int)));
}

ContextAdapter::~ContextAdapter()
{
  delete m_modelCompilationManager;
  delete m_currentSource;
  qDeleteAll(m_modelCache);
}


void ContextAdapter::readCachedModels()
{
  qDeleteAll(m_modelCache);
  m_modelCache.clear();

  QSettings ini(KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/models.ini"), QSettings::IniFormat);

  ini.beginGroup("Models");
  int size = ini.beginReadArray("Model");
  for (int i=0; i < size; i++) {
    ini.setArrayIndex(i);

    m_modelCache.insert(Situation(ini.value("DeactivatedScenarios").toStringList(), ini.value("DeactivatedSampleGroups").toStringList()),
                        new CachedModel(ini.value("CompiledDate").toDateTime(),
                                        (CachedModel::ModelState) ini.value("State").toInt(),
                                        (unsigned) ini.value("FingerPrint").toInt(),
					(ContextAdapter::BackendType) ini.value("Type", (int) ContextAdapter::SPHINX).toInt()
                                        ));
  }
  ini.endArray();
  size = ini.beginReadArray("OrphanedCache");
  for (int i=0; i < size; i++) {
    ini.setArrayIndex(i);
    m_orphanedCache << (unsigned) ini.value("FingerPrint").toInt();
  }
  ini.endArray();
  ini.endGroup();

  safelyAddContextFreeModelToCache();
}

void ContextAdapter::storeCachedModels()
{
  QSettings ini(KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/models.ini"), QSettings::IniFormat);

  ini.beginGroup("Models");
  ini.beginWriteArray("Model");
  int i=0;
  for (QHash<Situation, CachedModel*>::const_iterator j = m_modelCache.constBegin(); j != m_modelCache.constEnd(); j++) {
    ini.setArrayIndex(i++);
    ini.setValue("DeactivatedScenarios", j.key().deactivatedScenarios());
    ini.setValue("DeactivatedSampleGroups", j.key().deactivatedSampleGroups());
    ini.setValue("CompiledDate", j.value()->compiledDate());
    ini.setValue("State", j.value()->state());
    ini.setValue("FingerPrint", j.value()->srcFingerPrint());
    ini.setValue("Type", j.value()->type());
  }
  ini.endArray();
  ini.beginWriteArray("OrphanedCache");
  i=0;
  for (QList<uint>::const_iterator j = m_orphanedCache.constBegin(); j != m_orphanedCache.constEnd(); j++) {
    ini.setArrayIndex(i++);
    ini.setValue("FingerPrint", *j);
  }
  ini.endArray();
  ini.endGroup();
}

void ContextAdapter::safelyAddContextFreeModelToCache()
{
  if (!m_modelCache.contains(Situation())) {
    introduceNewModel(Situation());
  }
}

void ContextAdapter::clearCache()
{
  for (QHash<Situation, CachedModel*>::iterator j = m_modelCache.begin(); j != m_modelCache.end(); j++)
    QFile::remove(KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/"+j.key().id()+".sbm"));
  qDeleteAll(m_modelCache);
  m_modelCache.clear();
  safelyAddContextFreeModelToCache();
}

void ContextAdapter::abort()
{
  if (m_modelCompilationManager)
    m_modelCompilationManager->abort();
}

bool ContextAdapter::isCompiling() const
{
  if (!m_modelCompilationManager)
    return false;
  return m_modelCompilationManager->isRunning();
}

void ContextAdapter::updateDeactivatedScenarios(const QStringList& deactivatedScenarios)
{
  m_requestedSituation.setDeactivatedScenarios(deactivatedScenarios);
  buildCurrentSituation();
}

void ContextAdapter::updateDeactivatedSampleGroups(const QStringList& deactivatedSampleGroups)
{
  m_requestedSituation.setDeactivatedSampleGroups(deactivatedSampleGroups);
  buildCurrentSituation();
}

void ContextAdapter::buildCurrentSituation()
{
  kDebug() << "Building current situation: " << m_requestedSituation.deactivatedSampleGroups() << m_requestedSituation.deactivatedScenarios();
  m_compileLock.lock();
  CachedModel *model = m_modelCache.value(m_requestedSituation, 0);
  if (model) {
    kDebug() << "Model already exists and has state: " << model->state();
    if ((model->state() & CachedModel::Current) || (model->state() == CachedModel::Building)) {
      //we are golden
      m_compileLock.unlock();
      if (model->state() & CachedModel::Current) emit newModelReady();
      return;
    }
    model->setState(CachedModel::ToBeEvaluated); //mark it to be build
  } else
    introduceNewModel(m_requestedSituation);
  m_compileLock.unlock();

  buildNext();
}

void ContextAdapter::introduceNewModel ( const Situation& situation )
{
  m_modelCache.insert(situation, new CachedModel(QDateTime(), CachedModel::ToBeEvaluated, -1, ContextAdapter::Null));
}

void ContextAdapter::buildNext()
{
  if (!m_currentSource || (m_modelCompilationManager && m_modelCompilationManager->isRunning())) return;

  kDebug() << "Locking compile lock. Models to check: " << m_modelCache.count();
  m_compileLock.lock();

  Q_ASSERT(m_modelCache.value(Situation())); //should be here at any time

  if (m_modelCache.value(Situation())->state() == CachedModel::ToBeEvaluated) {
    //prioritize context free model
    adaptAndBuild(Situation(), m_modelCache.value(Situation()));
  } else {
    for (QHash<Situation, CachedModel*>::iterator j = m_modelCache.begin(); j != m_modelCache.end(); j++) {
      if (j.value()->state() == CachedModel::ToBeEvaluated) {
        kDebug() << "Building model " << j.key().id();
        adaptAndBuild(j.key(), j.value());
        break;
      } else
        kDebug() << "Model is fine: " << j.key().id();
    }
  }

  storeCachedModels();

  m_compileLock.unlock();
}
void ContextAdapter::adaptAndBuild ( const Situation& situation, CachedModel* model )
{
  //apply situation information on input before calling startModelCompilation
  QStringList scenarioPaths = adaptScenarios(m_currentSource->scenarioPaths(), situation.deactivatedScenarios());
  QString inputPrompts = m_currentSource->promptsPath();
  QString adaptedPromptsPath = adaptPrompts(inputPrompts, situation.deactivatedSampleGroups());

  kDebug() << "Starting model build";
  model->setState(CachedModel::Building);

  QString name;
  QDateTime creationDate;
  QString type;
  ContextAdapter::BackendType bType = ContextAdapter::Null;

  if (m_currentSource->baseModelType() != 2 /* no base model */) {
    KTar tar(m_currentSource->baseModelPath(), "application/x-gzip");
    if (!Model::parseContainer(tar, creationDate, name, type)) {
      emit error(i18n("Base model is corrupt."));
      slotModelCompilationAborted(ModelCompilation::InsufficientInput);
    } else {
      if (type == "SPHINX")
        bType = ContextAdapter::SPHINX;
      else if (type == "HTK")
        bType = ContextAdapter::HTK;
    }
  } else
    bType = getConfiguredDefaultBackendType();

  setupBackend(bType);
  model->setType(bType);
  m_modelCompilationManager->startModelCompilation(m_currentSource->baseModelType(), m_currentSource->baseModelPath(), scenarioPaths, adaptedPromptsPath);
}


QStringList ContextAdapter::adaptScenarios ( const QStringList& scenarioPaths, const QStringList& deactivatedScenarios )
{
  QStringList scenarioPathsOut;
  foreach (const QString& path, scenarioPaths)
    if (!deactivatedScenarios.contains(QFileInfo(path).fileName()))
      scenarioPathsOut.append(path);
  return scenarioPathsOut;
}


QString ContextAdapter::adaptPrompts ( const QString& promptsPath, const QStringList& deactivatedSampleGroups )
{
  QString outPath = KStandardDirs::locateLocal("tmp",
                                            KGlobal::mainComponent().aboutData()->appName()+'/'+m_username+"/context/prompts_"+
                                            QString::number(qHash(deactivatedSampleGroups.join(";"))));
  kDebug() << "=============== Adapting prompts: " << deactivatedSampleGroups << promptsPath << outPath;
  QFile outFile(outPath);
  QFile promptsFile(promptsPath);
  bool allEmpty = true;

  if (!promptsFile.open(QIODevice::ReadOnly) || !outFile.open(QIODevice::WriteOnly))
    return QString();

  while (!promptsFile.atEnd()) {
    QByteArray line = promptsFile.readLine();
    QList<QByteArray> tokens = line.split('"');
    if (tokens.count() == 1) {
      allEmpty = false;
      outFile.write(line);
    } else {
      if (tokens.count() != 3) continue; //malformed line

      if (!deactivatedSampleGroups.contains(QString::fromUtf8(tokens[1]))) {
        outFile.write(tokens[0]+' '+tokens[2]);
        allEmpty = false;
      }
    }
  }
  if (allEmpty) return QString(); // no prompts left after adaption

  return outPath;
}

void ContextAdapter::updateModelCompilationParameters ( const QDateTime& modelDate, int baseModelType, const QString& baseModelPath, const QStringList& scenarioPaths, const QString& promptsPathIn )
{
  kDebug() << "Updating model parameters";
  if (m_modelCompilationManager)
    m_modelCompilationManager->abort();

  m_compileLock.lock();
  ModelSource *src = m_currentSource;
  m_currentSource = new ModelSource(modelDate, baseModelType, baseModelPath, scenarioPaths, promptsPathIn);
  delete src;

  for (QHash<Situation, CachedModel*>::iterator j = m_modelCache.begin(); j != m_modelCache.end(); j++) {
    if (j.value()->compiledDate() < modelDate) {
      j.value()->setState(CachedModel::ToBeEvaluated);
    }
  }
  safelyAddContextFreeModelToCache(); //it might have been removed when canceling the compilation e.g. because of missing prompts / grammar
  storeCachedModels();
  m_compileLock.unlock();

  buildNext();
}

void ContextAdapter::slotModelReady(uint fingerprint, const QString& path)
{
  kDebug() << "Model ready: " << fingerprint << path;
  bool announce = false;
  m_compileLock.lock();
  for (QHash<Situation, CachedModel*>::iterator j = m_modelCache.begin(); j != m_modelCache.end(); j++) {
    if (j.value()->state() == CachedModel::Building) {
      j.value()->setState(CachedModel::Current);

      if (j.value()->srcFingerPrint() != fingerprint) {
        kDebug() << "Model has changed";
        //delete old cached model here iff no other cached model has the old fingerprint.
        bool isOnlyOne = true;
        for (QHash<Situation, CachedModel*>::const_iterator k = m_modelCache.constBegin(); k != m_modelCache.end(); k++) {
          if (!(k.key() == j.key()) && (k.value()->srcFingerPrint() == j.value()->srcFingerPrint())) {
            isOnlyOne = false;
            break;
          }
        }
        if (isOnlyOne) {
	  uint oldFingerPrint = j.value()->srcFingerPrint();
	  m_orphanedCache.insert(0, oldFingerPrint);

          //trim cache size
	  while (m_orphanedCache.size() > m_orphanedCacheSize) {
            bool cachedModelExists;
            QString oldCachePath = m_modelCompilationManager->cachedModelPath(m_orphanedCache.takeLast(), &cachedModelExists);
            if (cachedModelExists)
              QFile::remove(oldCachePath);
	  }
        }

        //announce a changed model if it's the context-free model
        if (j.key().id() == "active")
          announce = true;
      } else
        kDebug() << "Model hasn't changed";

      j.value()->setSrcFingerPrint(fingerprint);
      j.value()->setCompiledDate(m_currentSource->date());
      break;
    }
  }
  storeCachedModels();
  m_compileLock.unlock();

  if (announce)
    emit modelCompiled(path);
  emit newModelReady();
}

void ContextAdapter::slotModelCompilationAborted(ModelCompilation::AbortionReason reason)
{
  m_compileLock.lock();
  kDebug() << "Aborted model compilation with reason: " << reason;
  for (QHash<Situation, CachedModel*>::iterator j = m_modelCache.begin(); j != m_modelCache.end(); j++) {
    if (j.value()->state() == CachedModel::Building) {
      if (reason == ModelCompilation::InsufficientInput) {
        // this  model is a null model; deactivate recognition
        (*j)->setState(CachedModel::Null);
      } else
        m_modelCache.erase(j);
      break;
    }
  }
  safelyAddContextFreeModelToCache();
  storeCachedModels();
  m_compileLock.unlock();

  emit modelCompilationAborted();

  if (reason == ModelCompilation::InsufficientInput)
    emit newModelReady();
}

void ContextAdapter::currentModel(QString& path, ContextAdapter::BackendType& type) const
{
  kDebug() << "Requested situation: " << m_requestedSituation.deactivatedSampleGroups() << m_requestedSituation.deactivatedScenarios();
  //try to find models for:
  // 1. the currently requested situation
  // 2. if that's not available let's see if we have a general model
  foreach (const Situation& s, QList<Situation>() << m_requestedSituation << Situation()) {
    if (m_modelCache.contains(s)) {
      kDebug() << "Situation found: " << s.id() << (int) m_modelCache.value(s)->state();
      if (m_modelCache.value(s)->state() == CachedModel::Current) {
        path = m_modelCompilationManager->cachedModelPath(m_modelCache.value(s)->srcFingerPrint());
	type = m_modelCache.value(s)->type();
	return;
      } else if (m_modelCache.value(s)->state() == CachedModel::Null) {
	break; // go to null model
      }
    }
  }
  path = QString();
  type = ContextAdapter::Null;
}

