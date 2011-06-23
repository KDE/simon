#ifndef CONTEXTADAPTER_H
#define CONTEXTADAPTER_H

#include <QObject>
#include <QHash>

#include <speechmodelcompilationadapter/modelcompilationadapter.h>
#include <speechmodelcompilation/modelcompilationmanager.h>
#include "simoncontextadapter_export.h"

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

//    bool startCompilation(ModelCompilationManager::CompilationType compilationType,
//      const QString& hmmDefsPath, const QString& tiedListPath,
//      const QString& dictPath, const QString& dfaPath,
//      const QString& baseHmmDefsPath, const QString& baseTiedlistPath,
//      const QString& baseStatsPath, const QString& baseMacrosPath,
//      const QString& samplePath,
//      const QString& lexiconPath, const QString& grammarPath,
//      const QString& vocabPath, const QString& promptsPath,
//      const QString& treeHedPath, const QString& wavConfigPath,
//      const QString& scriptBasePrefix);

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
