#ifndef CONTEXTADAPTER_H
#define CONTEXTADAPTER_H

#include <QObject>

#include <speechmodelcompilationadapter/modelcompilationadapter.h>
#include <speechmodelcompilation/modelcompilationmanager.h>
#include "simoncontextadapter_export.h"

class SIMONCONTEXTADAPTER_EXPORT ContextAdapter : public QObject
{
    Q_OBJECT

public:
    ContextAdapter(ModelCompilationManager *modelCompilationManager,
                   ModelCompilationAdapter *modelCompilationAdapter, QObject *parent=0);

    void updateDeactivatedScenarios(QStringList deactivatedScenarios);

    bool startAdaption(ModelCompilationAdapter::AdaptionType adaptionType, const QString& lexiconPathOut,
      const QString& grammarPathOut, const QString& simpleVocabPathOut,
      const QString& promptsPathOut, const QStringList& scenarioPathsIn,
      const QString& promptsIn);

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
};

#endif // CONTEXTADAPTER_H
