#ifndef CONTEXTADAPTER_H
#define CONTEXTADAPTER_H

/** \file contextadapter.h
 * \brief The file containing the ContextAdapter baseclass header.
 */

#include <QObject>
#include <QHash>

#include <speechmodelcompilationadapter/modelcompilationadapter.h>
#include <speechmodelcompilationadapter/modelcompilationadapterhtk.h>
#include <speechmodelcompilation/modelcompilationmanager.h>
#include "simoncontextadapter_export.h"

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
 *      Whenever the base Scenario list changes, the ContextAdapter clears its cache.
 *
 *	\sa ModelCompilationAdapter, ModelCompilationManager, ClientSocket
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class SIMONCONTEXTADAPTER_EXPORT ContextAdapter : public QObject
{
    Q_OBJECT

public:
    ContextAdapter(QString username, QObject *parent=0);

    ~ContextAdapter();

    enum Activity
    {
        NoActivity,
        CompilingModel
    };

    void updateDeactivatedScenarios(QStringList deactivatedScenarios);
    bool loadLanguageModelFromCache(QStringList deactivatedScenarios);
    void storeLanguageModelInCache(QStringList deactivatedScenarios);

    void updateAcousticModelSampleGroup(QString sampleGroup);
    bool loadAcousticModelFromCache(QString sampleGroup);
    void storeAcousticModelInCache(QString sampleGroup);

    void clearCache();
    void currentCompilationAborted();
    bool shouldRecompileModel();

    //wrapper functions for ModelCompilationAdapter
    bool startAdaption(ModelCompilationAdapter::AdaptionType adaptionType, const QString& lexiconPathOut,
      const QString& grammarPathOut, const QString& simpleVocabPathOut,
      const QString& promptsPathOut, const QStringList& scenarioPathsIn,
      const QString& promptsIn);
    QString getStatus() { return m_modelCompilationAdapter->getStatus(); }
    QString lexiconPath() { return m_modelCompilationAdapter->lexiconPath(); }
    QString grammarPath() { return m_modelCompilationAdapter->grammarPath(); }
    QString simpleVocabPath() { return m_modelCompilationAdapter->simpleVocabPath(); }
    QString promptsPath() { return m_modelCompilationAdapter->promptsPath(); }
    int wordCount() { return m_modelCompilationAdapter->wordCount(); }
    int pronunciationCount() { return m_modelCompilationAdapter->pronunciationCount(); }
    int sampleCount() { return m_modelCompilationAdapter->sampleCount(); }
    void clearPoisonedPhonemes() { m_modelCompilationAdapter->clearPoisonedPhonemes(); }
    void poisonPhoneme( const QString& phoneme ) { m_modelCompilationAdapter->poisonPhoneme(phoneme); }
    int maxProgress() { return m_modelCompilationAdapter->maxProgress(); }

    //wrapper functions for ModelCompilationManager
    bool startCompilation(ModelCompilationManager::CompilationType compilationType,
      const QString& hmmDefsPath, const QString& tiedListPath,
      const QString& dictPath, const QString& dfaPath,
      const QString& baseHmmDefsPath, const QString& baseTiedlistPath,
      const QString& baseStatsPath, const QString& baseMacrosPath,
      const QString& samplePath,
      const QString& lexiconPath, const QString& grammarPath,
      const QString& vocabPath, const QString& promptsPath,
      const QString& treeHedPath, const QString& wavConfigPath,
      const QString& scriptBasePrefix);
    bool hasBuildLog() { return m_modelCompilationManager->hasBuildLog(); }
    QString getGraphicBuildLog() { return m_modelCompilationManager->getGraphicBuildLog(); }
    QString getBuildLog() { return m_modelCompilationManager->getBuildLog(); }
    static QString information(bool condensed=false) { return ModelCompilationManager::information(condensed); }
    bool codeAudioDataFromScp(const QString& path) { return m_modelCompilationManager->codeAudioDataFromScp(path); }
    bool reestimate(const QString& command) { return m_modelCompilationManager->reestimate(command); }
    bool managerIsRunning() { return m_modelCompilationManager->isRunning(); }

    //wrapper function for both
    void abort() { m_modelCompilationManager->abort();
                   m_modelCompilationAdapter->abort();}

private:
    ModelCompilationAdapter *m_modelCompilationAdapter;
    ModelCompilationManager *m_modelCompilationManager;
    QStringList m_requestedDeactivatedScenarios;
    QStringList m_currentModelDeactivatedScenarios;
    QStringList m_currentlyCompilingDeactivatedScenarios;
    QStringList m_currentScenarioSet;
    QString m_username;
    QString m_currentSampleGroup;
    QString m_requestedSampleGroup;
    QString m_compilingSampleGroup;
    bool m_newAcousticModel;

    ContextAdapter::Activity m_currentActivity;

    //key: comma concatenated list of deactivated scenarios
    //value: directory of the model cache
    QHash<QString, QString> m_modelCache;

    //key: sample group name
    //value: name of directory with a prompts file and acoustic model for just that sample group
    QHash<QString, QString> m_acousticModelCache;

    void finishedModelRequest(bool viaCache);

private slots:
    void hasNewlyGeneratedModel();

public slots:
    void aborted();

signals:
    void modelLoadedFromCache();
    void forceModelRecompilation();

    //for relaying compilation adapter signals
    void adaptStatus(QString, int progressNow);
    void adaptError(QString);
    void adaptionComplete();
    void adaptionAborted();

    //for relaying compilation manager signals
    void manageStatus(QString, int progressNow, int progressTotal=2600);
    void manageError(QString);
    void wordUndefined(const QString&);
    void classUndefined(const QString&);
    void phonemeUndefined(const QString&);
    void modelCompiled();
    void activeModelCompilationAborted();
};

#endif // CONTEXTADAPTER_H
