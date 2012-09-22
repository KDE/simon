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

#ifndef CONTEXTADAPTER_H
#define CONTEXTADAPTER_H

/** \file contextadapter.h
 * \brief The file containing the ContextAdapter baseclass header.
 */

#include "simoncontextadapter_export.h"
#include "situation.h"

#include <speechmodelcompilation/modelcompilationmanager.h>

#include <QObject>
#include <QHash>
#include <QMutex>

class CachedModel;
class ModelSource;

/**
 *	@class ContextAdapter
 *	@brief The ContextAdapter class provides a context-sensitive wrapper for the ModelCompilationAdapter and ModelCompilationManager
 *
 *      The ContextAdapter manages a cache of adapted grammar models that have been generated with the current
 *      Scenario list and various contexts.  When a Scenario is deactivated due to context, the deactivated Scenario
 *      list is relayed to the ContextAdapter.  If a model had already been adapted with that deactivated Scenario list
 *      (ie. the appropriate model was already in the cache), the model is loaded from the cache and the adaption step is bypassed.
 *      Otherwise, the ModelCompilationAdapter is used to adapt a new model, and that model is stored in the ContextAdapter's
 *      cache.  Bypassing the adaption step with caching helps speed up context-based model switches.
 *
 *      Whenever the base Scenario list or prompts changes, the ContextAdapter clears its cache.
 *
 *	\sa ModelCompilationManager, ClientSocket
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class SIMONCONTEXTADAPTER_EXPORT ContextAdapter : public QObject
{
    Q_OBJECT

public:
    explicit ContextAdapter(QString username, QObject *parent=0);

    enum Activity
    {
        NoActivity,
        CompilingModel
    };

    void updateDeactivatedScenarios( const QStringList& deactivatedScenarios );
    void updateDeactivatedSampleGroups(const QStringList& deactivatedSampleGroups);

    void clearCache();

    void updateModelCompilationParameters(const QDateTime& modelDate, int baseModelType, const QString& baseModelPath,
                                          const QStringList& scenarioPaths, const QString& promptsPathIn);

    //wrapper functions
    
    bool hasBuildLog() { return m_modelCompilationManager->hasBuildLog(); }
    QString getGraphicBuildLog() { return m_modelCompilationManager->getGraphicBuildLog(); }
    QString getBuildLog() { return m_modelCompilationManager->getBuildLog(); }

    void abort();
    
    bool isCompiling() const;
    
    QString currentModelPath() const;

private:
    QMutex m_compileLock;
    QString m_username;
    
    ModelCompilationManager *m_modelCompilationManager;
    
    ModelSource *m_currentSource;
    
    Situation m_requestedSituation;
    QHash<Situation, CachedModel*> m_modelCache;
    
    void readCachedModels();
    void storeCachedModels();
    
    void buildCurrentSituation();
    
    void buildNext();
    void introduceNewModel(const Situation& situation);
    
    void safelyAddContextFreeModelToCache();
    QStringList adaptScenarios(const QStringList& scenarioPaths, const QStringList& deactivatedScenarios);
    QString adaptPrompts(const QString& promptsPath, const QStringList& deactivatedSampleGroups);
    
    void adaptAndBuild(const Situation& situation, CachedModel* model);
      

private slots:
    void slotModelReady(uint fingerprint, const QString& path);
    void slotModelCompilationAborted( ModelCompilation::AbortionReason reason );

signals:
    //relaying signals
    void newModelReady();                    /// Always emitted when a model generation / evaluation process is completed
    void modelCompiled(const QString& path); /// only emitted, when the context-free model has changed
    void modelCompilationAborted();
    void status(QString mesg, int now, int max);
    void error(QString);
    
    void wordUndefined(const QString&);
    void classUndefined(const QString&);
    void phonemeUndefined(const QString&);
};

#endif // CONTEXTADAPTER_H
