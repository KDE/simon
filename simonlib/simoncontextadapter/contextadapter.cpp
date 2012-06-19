/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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
#include <speechmodelcompilation/modelcompilationmanager.h>
#include <speechmodelcompilation/modelcompilationmanagerhtk.h>
#include <speechmodelcompilation/modelcompilationmanagersphinx.h>
#include <QDir>
#include <QSettings>
#include <KStandardDirs>
#include <KDebug>
#include <KAboutData>
#include <KDateTime>
#include <KConfigGroup>

ContextAdapter::ContextAdapter(QString username, QObject *parent) :
  QObject(parent),
  m_currentSource(0)
{
#ifdef TSPHINX
  m_modelCompilationManager = new ModelCompilationManagerSPHINX(username, this);
#else
  m_modelCompilationManager = new ModelCompilationManagerHTK(username, this);
#endif
  //FIXME: hardcode

  m_username = username;

  connect(m_modelCompilationManager, SIGNAL(modelReady(uint, QString)), this, SLOT(slotModelReady(uint,QString)));
  connect(m_modelCompilationManager, SIGNAL(modelCompilationAborted()), this, SLOT(slotModelCompilationAborted()));

  connect(m_modelCompilationManager, SIGNAL(classUndefined(QString)), this, SIGNAL(classUndefined(QString)));
  connect(m_modelCompilationManager, SIGNAL(wordUndefined(QString)), this, SIGNAL(wordUndefined(QString)));
  connect(m_modelCompilationManager, SIGNAL(phonemeUndefined(QString)), this, SIGNAL(phonemeUndefined(QString)));
  connect(m_modelCompilationManager, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
  connect(m_modelCompilationManager, SIGNAL(status(QString, int, int)), this, SIGNAL(status(QString, int, int)));

  readCachedModels();
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
                                        (unsigned) ini.value("FingerPrint").toInt()
                                        ));
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
  }
  ini.endArray();
  ini.endGroup();
}

void ContextAdapter::safelyAddContextFreeModelToCache()
{
  kDebug() << "Checking for context-free model";
  if (!m_modelCache.contains(Situation())) {
    kDebug() << "Adding context-free model";
    introduceNewModel(Situation());
  } else
    kDebug() << "Context free model already there";
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
  m_modelCompilationManager->abort();
}

bool ContextAdapter::isCompiling() const
{
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
    if ((model->state() == CachedModel::Current) || (model->state() == CachedModel::Building)) {
      //we are golden
      m_compileLock.unlock();
      if (model->state() == CachedModel::Current) emit newModelReady();
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
  m_modelCache.insert(situation, new CachedModel(QDateTime(), CachedModel::ToBeEvaluated, -1));
}

void ContextAdapter::buildNext()
{
  if (!m_currentSource || m_modelCompilationManager->isRunning()) return;

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
  QString adaptedPromptsPath = adaptPrompts(m_currentSource->promptsPath(), situation.deactivatedSampleGroups());

  kDebug() << "Starting model compilation";
  model->setState(CachedModel::Building);
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
  kDebug() << "=============== Adapting prompts: " << deactivatedSampleGroups;
  QString outPath = KStandardDirs::locateLocal("tmp", 
                                            KGlobal::mainComponent().aboutData()->appName()+'/'+m_username+"/context/prompts_"+
                                            QString::number(qHash(deactivatedSampleGroups.join(";"))));
  QFile outFile(outPath);
  QFile promptsFile(promptsPath);
  if (!outFile.open(QIODevice::WriteOnly) || !promptsFile.open(QIODevice::ReadOnly))
    return QString();
  while (!promptsFile.atEnd()) {
    QByteArray line = promptsFile.readLine();
    QList<QByteArray> tokens = line.split('"');
    if (tokens.count() == 1) {
      outFile.write(line);
    } else {
      if (tokens.count() != 3) continue; //malformed line

      if (!deactivatedSampleGroups.contains(QString::fromUtf8(tokens[1])))
        outFile.write(tokens[0]+' '+tokens[2]);
    }
  }
  kDebug() << "Serialized prompts to: " << outPath;
  return outPath;
}

void ContextAdapter::updateModelCompilationParameters ( const QDateTime& modelDate, int baseModelType, const QString& baseModelPath, const QStringList& scenarioPaths, const QString& promptsPathIn )
{
  kDebug() << "Updating model parameters";
  m_compileLock.lock();
  m_modelCompilationManager->abort();

  ModelSource *src = m_currentSource;
  m_currentSource = new ModelSource(modelDate, baseModelType, baseModelPath, scenarioPaths, promptsPathIn);
  delete src;

  for (QHash<Situation, CachedModel*>::iterator j = m_modelCache.begin(); j != m_modelCache.end(); j++) {
    if (j.value()->compiledDate() < modelDate) {
      j.value()->setState(CachedModel::ToBeEvaluated);
      break;
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
          bool cachedModelExists;
          QString oldCachePath = m_modelCompilationManager->cachedModelPath(j.value()->srcFingerPrint(), &cachedModelExists);
          if (cachedModelExists)
            QFile::remove(oldCachePath);
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

void ContextAdapter::slotModelCompilationAborted()
{
  m_compileLock.lock();
  for (QHash<Situation, CachedModel*>::iterator j = m_modelCache.begin(); j != m_modelCache.end(); j++) {
    if (j.value()->state() == CachedModel::Building) {
      m_modelCache.erase(j);
      break;
    }
  }
  storeCachedModels();
  m_compileLock.unlock();
  emit modelCompilationAborted();
}

QString ContextAdapter::currentModelPath() const
{
  //try to find models for:
  // 1. the currently requested situation
  // 2. if that's not availble let's see if we have a general model
  foreach (const Situation& s, QList<Situation>() << m_requestedSituation << Situation())
    if (m_modelCache.contains(s) && (m_modelCache.value(s)->state() == CachedModel::Current))
      return m_modelCompilationManager->cachedModelPath(m_modelCache.value(s)->srcFingerPrint());

  // 3. if both of those faile, we have no useful model
  return QString();
}
