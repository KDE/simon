#include "contextadapter.h"
#include <KDebug>

ContextAdapter::ContextAdapter(ModelCompilationManager *modelCompilationManager,
                               ModelCompilationAdapter *modelCompilationAdapter, QObject *parent) :
    QObject(parent)
{
    m_modelCompilationManager = modelCompilationManager;
    m_modelCompilationAdapter = modelCompilationAdapter;
    m_deactivatedScenarios = QStringList();
}

void ContextAdapter::updateDeactivatedScenarios(QStringList deactivatedScenarios)
{
    m_deactivatedScenarios = deactivatedScenarios;
}

bool ContextAdapter::startAdaption(ModelCompilationAdapter::AdaptionType adaptionType, const QString& lexiconPathOut,
                                   const QString& grammarPathOut, const QString& simpleVocabPathOut,
                                   const QString& promptsPathOut, const QStringList& scenarioPathsIn,
                                   const QString& promptsIn)
{
    //get the list of NOT deactivated scenario paths
    QStringList activeScenarioPathsIn;
    QRegExp slashes = QRegExp("/+");
    foreach (QString path, scenarioPathsIn)
    {
        if (!m_deactivatedScenarios.contains(path.split(slashes).back()))
        {
            kDebug() << path.split(slashes).back() + " is active";
            activeScenarioPathsIn << path;
        }
    }

    return m_modelCompilationAdapter->startAdaption(adaptionType, lexiconPathOut,
                                                    grammarPathOut, simpleVocabPathOut,
                                                    promptsPathOut, activeScenarioPathsIn,
                                                    promptsIn);
}

//bool ContextAdapter::startCompilation(ModelCompilationManager::CompilationType compilationType,
//                                      const QString& hmmDefsPath, const QString& tiedListPath,
//                                      const QString& dictPath, const QString& dfaPath,
//                                      const QString& baseHmmDefsPath, const QString& baseTiedlistPath,
//                                      const QString& baseStatsPath, const QString& baseMacrosPath,
//                                      const QString& samplePath,
//                                      const QString& lexiconPath, const QString& grammarPath,
//                                      const QString& vocabPath, const QString& promptsPath,
//                                      const QString& treeHedPath, const QString& wavConfigPath,
//                                      const QString& scriptBasePrefix)
//{
//    return true;
//}
