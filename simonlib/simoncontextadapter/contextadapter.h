#ifndef CONTEXTADAPTER_H
#define CONTEXTADAPTER_H

/** \file contextadapter.h
 * \brief The file containing the ContextAdapter baseclass header.
 */

#include <QObject>
#include <QHash>

#include <speechmodelcompilationadapter/modelcompilationadapter.h>
#include <speechmodelcompilation/modelcompilationmanager.h>
#include "simoncontextadapter_export.h"

/**
 *	@class ContextAdapter
 *	@brief The ContextAdapter class provides a context-sensitive wrapper for the ModelCompilationAdapter
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
 *	\sa ModelCompilationAdapter, ClientSocket
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class SIMONCONTEXTADAPTER_EXPORT ContextAdapter : public QObject
{
    Q_OBJECT

public:
    ContextAdapter(ModelCompilationManager *modelCompilationManager,
                   ModelCompilationAdapter *modelCompilationAdapter,
                   QString username, QObject *parent=0);

    ~ContextAdapter();

    void updateDeactivatedScenarios(QStringList deactivatedScenarios);

    bool startAdaption(ModelCompilationAdapter::AdaptionType adaptionType, const QString& lexiconPathOut,
      const QString& grammarPathOut, const QString& simpleVocabPathOut,
      const QString& promptsPathOut, const QStringList& scenarioPathsIn,
      const QString& promptsIn);

    void clearCache();

private:
    ModelCompilationAdapter *m_modelCompilationAdapter;
    ModelCompilationManager *m_modelCompilationManager;
    QStringList m_deactivatedScenarios;
    QStringList m_currentScenarioSet;
    QString m_username;

    //key: comma concatenated list of deactivated scenarios
    //value: directory of the model cache
    QHash<QString, QString> m_modelCache;

public slots:
    void storeModelInCache();

signals:
    void modelLoadedFromCache();
};

#endif // CONTEXTADAPTER_H
