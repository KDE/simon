#include "contextadapter.h"
#include <KDebug>
#include <QDir>
#include <KStandardDirs>

ContextAdapter::ContextAdapter(ModelCompilationManager *modelCompilationManager,
                               ModelCompilationAdapter *modelCompilationAdapter,
                               QString username, QObject *parent) :
    QObject(parent)
{
    m_modelCompilationManager = modelCompilationManager;
    m_modelCompilationAdapter = modelCompilationAdapter;
    m_username = username;
    m_deactivatedScenarios = QStringList();
    m_currentScenarioSet = QStringList();

    connect(m_modelCompilationManager, SIGNAL(modelCompiled()),
            this, SLOT(storeModelInCache()));

    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QDir dir;
    if (!dir.exists(cacheDir))
    {
        kDebug() << "making cache directory";
        dir.mkpath(cacheDir);
    }

}

ContextAdapter::~ContextAdapter()
{
    clearCache();
}

void ContextAdapter::updateDeactivatedScenarios(QStringList deactivatedScenarios)
{
    m_deactivatedScenarios = deactivatedScenarios;
}

//(ModelCompilationAdapter::AdaptionType)
//(ModelCompilationAdapter::AdaptLanguageModel),
//activeDir+"lexicon",
//activeDir+"model.grammar",
//activeDir+"simple.voca",
//activeDir+"prompts",
//synchronisationManager->getScenarioPaths(),
//synchronisationManager->getPromptsPath()

bool ContextAdapter::startAdaption(ModelCompilationAdapter::AdaptionType adaptionType, const QString& lexiconPathOut,
                                   const QString& grammarPathOut, const QString& simpleVocabPathOut,
                                   const QString& promptsPathOut, const QStringList& scenarioPathsIn,
                                   const QString& promptsIn)
{
    //Get paths for caching
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString cachedModelDir;
    QFile file;

    //check to see if the scenarios are different (in which case the cache would need to be cleared)
    if (m_currentScenarioSet.join(",") != scenarioPathsIn.join(","))
    {
        kDebug() << "There is a new base scenario list!";
        clearCache();
        m_currentScenarioSet = scenarioPathsIn;
    }

    //get the list of NOT deactivated scenario paths
    bool success;
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

    //see if these are in the cache
    QString deactivatedList = m_deactivatedScenarios.join(",");
    kDebug() << "Checking cache for the following entry: " + deactivatedList;
    if (!m_modelCache.value(deactivatedList, "").isEmpty())
    {
        kDebug() << "Entry found!  Retrieving files from cache";

        //if so, replace the active files with the cached ones and return
        cachedModelDir = cacheDir + m_modelCache.value(deactivatedList) + "/";

        file.remove(activeDir+"lexicon");
        file.copy(cachedModelDir+"lexicon", activeDir+"lexicon");
        file.remove(activeDir+"model.grammar");
        file.copy(cachedModelDir+"model.grammar", activeDir+"model.grammar");
        file.remove(activeDir+"simple.voca");
        file.copy(cachedModelDir+"simple.voca", activeDir+"simple.voca");
        file.remove(activeDir+"prompts");
        file.copy(cachedModelDir+"prompts", activeDir+"prompts");
        file.remove(activeDir+"hmmdefs");
        file.copy(cachedModelDir+"hmmdefs", activeDir+"hmmdefs");
        file.remove(activeDir+"tiedlist");
        file.copy(cachedModelDir+"tiedlist", activeDir+"tiedlist");
        file.remove(activeDir+"model.dict");
        file.copy(cachedModelDir+"model.dict", activeDir+"model.dict");
        file.remove(activeDir+"model.dfa");
        file.copy(cachedModelDir+"model.dfa", activeDir+"model.dfa");
        file.remove(activeDir+"basehmmdefs");
        file.copy(cachedModelDir+"basehmmdefs", activeDir+"basehmmdefs");
        file.remove(activeDir+"basetiedlist");
        file.copy(cachedModelDir+"basetiedlist", activeDir+"basetiedlist");
        file.remove(activeDir+"basemacros");
        file.copy(cachedModelDir+"basemacros", activeDir+"basemacros");
        file.remove(activeDir+"basestats");
        file.copy(cachedModelDir+"basestats", activeDir+"basestats");
        file.remove(activeDir+"activerc");
        file.copy(cachedModelDir+"activerc", activeDir+"activerc");
        file.remove(activeDir+"hmmdefs_loc");
        file.copy(cachedModelDir+"hmmdefs_loc", activeDir+"hmmdefs_loc");
        file.remove(activeDir+"julius.log");
        file.copy(cachedModelDir+"julius.log", activeDir+"julius.log");
        file.remove(activeDir+"julius.jconf");
        file.copy(cachedModelDir+"julius.jconf", activeDir+"julius.jconf");

        emit modelLoadedFromCache();

        return true;
    }

    //otherwise, adapt the model
    success =  m_modelCompilationAdapter->startAdaption(adaptionType, lexiconPathOut,
                                                        grammarPathOut, simpleVocabPathOut,
                                                        promptsPathOut, activeScenarioPathsIn,
                                                        promptsIn);

    return success;
}

//bool ContextAdapter::startCompilation(ModelCompilationManager::CompilationType compilationType,
//                                      const QString& hmmDefsPath,
//                                      const QString& tiedListPath,
//                                      const QString& dictPath,
//                                      const QString& dfaPath,
//                                      const QString& baseHmmDefsPath,
//                                      const QString& baseTiedlistPath,
//                                      const QString& baseStatsPath,
//                                      const QString& baseMacrosPath,
//                                      const QString& samplePath,
//                                      const QString& lexiconPath,
//                                      const QString& grammarPath,
//                                      const QString& vocabPath,
//                                      const QString& promptsPath,
//                                      const QString& treeHedPath,
//                                      const QString& wavConfigPath,
//                                      const QString& scriptBasePrefix)
//{
//    return true;
//}
//(ModelCompilationManager::CompileLanguageModel),
//activeDir+"hmmdefs",
//activeDir+"tiedlist",
//activeDir+"model.dict",
//activeDir+"model.dfa",
//activeDir+"basehmmdefs",
//activeDir+"basetiedlist",
//activeDir+"basemacros",
//activeDir+"basestats",
//KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/"),
//activeDir+"lexicon",
//activeDir+"model.grammar",
//activeDir+"simple.voca",
//activeDir+"prompts",
//synchronisationManager->getTreeHedPath(),
//synchronisationManager->getWavConfigPath(),
//"simon/scripts");

void ContextAdapter::storeModelInCache()
{
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString cachedModelDir;
    QDir dir;
    QFile file;
    QString deactivatedList = m_deactivatedScenarios.join(",");

    cachedModelDir.setNum(m_modelCache.count());
    kDebug() << "Adding the following entry to cache " + deactivatedList;
    m_modelCache.insert(deactivatedList, cachedModelDir);
    cachedModelDir = cacheDir + cachedModelDir + "/";
    dir.mkpath(cachedModelDir);
    kDebug() << "With the following cache directory " + cachedModelDir;

    kDebug() << "Storing files in cache";

    file.copy(activeDir+"lexicon", cachedModelDir+"lexicon");
    file.copy(activeDir+"model.grammar", cachedModelDir+"model.grammar");
    file.copy(activeDir+"simple.voca", cachedModelDir+"simple.voca");
    file.copy(activeDir+"prompts", cachedModelDir+"prompts");
    file.copy(activeDir+"hmmdefs", cachedModelDir+"hmmdefs");
    file.copy(activeDir+"tiedlist", cachedModelDir+"tiedlist");
    file.copy(activeDir+"model.dict", cachedModelDir+"model.dict");
    file.copy(activeDir+"model.dfa", cachedModelDir+"model.dfa");
    file.copy(activeDir+"basehmmdefs", cachedModelDir+"basehmmdefs");
    file.copy(activeDir+"basetiedlist", cachedModelDir+"basetiedlist");
    file.copy(activeDir+"basemacros", cachedModelDir+"basemacros");
    file.copy(activeDir+"basestats", cachedModelDir+"basestats");
    file.copy(activeDir+"activerc", cachedModelDir+"activerc");
    file.copy(activeDir+"hmmdefs_loc", cachedModelDir+"hmmdefs_loc");
    file.copy(activeDir+"julius.log", cachedModelDir+"julius.log");
    file.copy(activeDir+"julius.jconf", cachedModelDir+"julius.jconf");
}

void ContextAdapter::clearCache()
{
    QDir dir;
    QFile file;
    QStringList cacheDirs;
    QStringList cachedFiles;

    kDebug() << "Clearing model cache";

    m_modelCache.clear();

    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QDir initialDir = dir.current();
    dir.setCurrent(cacheDir);

    cacheDirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (QString dirInfo, cacheDirs)
    {
        dir.setCurrent(dirInfo);
        //kDebug() << "Clearing out directory " + dir.absoluteFilePath(dir.dirName());
        cachedFiles = dir.entryList(QDir::Files);
        foreach (QString fileInfo, cachedFiles)
        {
            //kDebug() << "Deleting file " + fileInfo;
            file.remove(fileInfo);
        }
        dir.setCurrent(cacheDir);
        dir.rmdir(dirInfo);
    }

    dir.setCurrent(initialDir.dirName());
}
