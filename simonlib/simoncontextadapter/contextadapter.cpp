#include "contextadapter.h"
#include <KDebug>
#include <QDir>
#include <KStandardDirs>
#include <QTextStream>

ContextAdapter::ContextAdapter(QString username, QObject *parent) :
    QObject(parent)
{
    m_modelCompilationManager = new ModelCompilationManager(username, this);
    m_modelCompilationAdapter = new ModelCompilationAdapterHTK(username, this);
    m_username = username;
    m_deactivatedScenarios = QStringList();
    m_currentScenarioSet = QStringList();

    connect(m_modelCompilationManager, SIGNAL(modelCompiled()),
            this, SLOT(storeModelInCache()));

    //connect compilation adapter signals
    connect(m_modelCompilationAdapter, SIGNAL(status(QString,int)),
            this, SIGNAL(adaptStatus(QString,int)));
    connect(m_modelCompilationAdapter, SIGNAL(error(QString)),
            this, SIGNAL(adaptError(QString)));
    connect(m_modelCompilationAdapter, SIGNAL(adaptionComplete()),
            this, SIGNAL(adaptionComplete()));
    connect(m_modelCompilationAdapter, SIGNAL(adaptionAborted()),
            this, SIGNAL(adaptionAborted()));

    //connect compilation manager signals
    connect(m_modelCompilationManager, SIGNAL(status(QString,int,int)),
            this, SIGNAL(manageStatus(QString,int,int)));
    connect(m_modelCompilationManager, SIGNAL(error(QString)),
            this, SIGNAL(manageError(QString)));
    connect(m_modelCompilationManager, SIGNAL(wordUndefined(QString)),
            this, SIGNAL(wordUndefined(QString)));
    connect(m_modelCompilationManager, SIGNAL(classUndefined(QString)),
            this, SIGNAL(classUndefined(QString)));
    connect(m_modelCompilationManager, SIGNAL(phonemeUndefined(QString)),
            this, SIGNAL(phonemeUndefined(QString)));
    connect(m_modelCompilationManager, SIGNAL(modelCompiled()),
            this, SIGNAL(modelCompiled()));
    connect(m_modelCompilationManager, SIGNAL(activeModelCompilationAborted()),
            this, SIGNAL(activeModelCompilationAborted()));

    //make sure that the cache directory and cache info files exist
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QDir dir;
    if (!dir.exists(cacheDir))
    {
        kDebug() << "making cache directory";
        dir.mkpath(cacheDir);
    }     

    //load the base scenario list
    QFile scenariosFile(cacheDir + "ScenarioList");
    if (scenariosFile.exists())
    {
        scenariosFile.open(QFile::ReadOnly);
        QDataStream fileStream(&scenariosFile);
        fileStream >> m_currentScenarioSet;
        scenariosFile.close();

        kDebug() << "Loaded base scenario list: " << m_currentScenarioSet;
    }

    //load the model cache lookup hash table
    QFile lookupFile(cacheDir + "CacheLookup");
    if (lookupFile.exists())
    {
        lookupFile.open(QFile::ReadOnly);
        QDataStream lookupStream(&lookupFile);
        lookupStream >> m_modelCache;
        lookupFile.close();

        kDebug() << "Loaded model cache lookup: " << m_modelCache;
    }
}

ContextAdapter::~ContextAdapter()
{
    if (m_modelCompilationManager)
        m_modelCompilationManager->deleteLater();
    if (m_modelCompilationAdapter)
        m_modelCompilationAdapter->deleteLater();
}

void ContextAdapter::updateDeactivatedScenarios(QStringList deactivatedScenarios)
{
    m_deactivatedScenarios = deactivatedScenarios;
}

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

    //save model cache lookup
    QFile lookupFile(cacheDir + "CacheLookup");
    lookupFile.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream lookupStream(&lookupFile);
    lookupStream << m_modelCache;
    lookupFile.close();
    kDebug() << "Model cache lookup saved: " << m_modelCache;
}

void ContextAdapter::clearCache()
{
    QDir dir;
    QFile file;
    QStringList cacheDirs;
    QStringList cachedFiles;
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");

    kDebug() << "Clearing model cache";

    m_modelCache.clear();

    QFile lookupFile(cacheDir + "CacheLookup");
    if (lookupFile.exists())
    {
        lookupFile.open(QFile::Truncate);
        QDataStream lookupStream(&lookupFile);
        lookupStream >> m_modelCache;
        lookupFile.close();

        kDebug() << "Model cache lookup refreshed";
    }

    QDir initialDir = dir.current();
    dir.setCurrent(cacheDir);

    cacheDirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (QString dirInfo, cacheDirs)
    {
        dir.setCurrent(dirInfo);
        cachedFiles = dir.entryList(QDir::Files);
        foreach (QString fileInfo, cachedFiles)
        {
            file.remove(fileInfo);
        }
        dir.setCurrent(cacheDir);
        dir.rmdir(dirInfo);
    }

    dir.setCurrent(initialDir.dirName());
}

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

    //check to see if the base scenarios are different (in which case the cache would need to be cleared)
    if (m_currentScenarioSet.join(",") != scenarioPathsIn.join(","))
    {
        kDebug() << "There is a new base scenario list!";
        clearCache();

        m_currentScenarioSet = scenarioPathsIn;

        //save the new base scenario list
        QFile scenariosFile(cacheDir + "ScenarioList");
        scenariosFile.open(QFile::Truncate | QFile::WriteOnly);
        QDataStream fileStream(&scenariosFile);
        fileStream << scenarioPathsIn;
        scenariosFile.close();
         kDebug() << "Saved new base scenario list: " << m_currentScenarioSet;
    }
    else
    {
        kDebug() << "Still using base scenario list: " << m_currentScenarioSet;
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

bool ContextAdapter::startCompilation(ModelCompilationManager::CompilationType compilationType,
  const QString& hmmDefsPath, const QString& tiedListPath,
  const QString& dictPath, const QString& dfaPath,
  const QString& baseHmmDefsPath, const QString& baseTiedlistPath,
  const QString& baseStatsPath, const QString& baseMacrosPath,
  const QString& samplePath,
  const QString& lexiconPath, const QString& grammarPath,
  const QString& vocabPath, const QString& promptsPath,
  const QString& treeHedPath, const QString& wavConfigPath,
  const QString& scriptBasePrefix)
{
    return m_modelCompilationManager->startCompilation(compilationType,
                                                       hmmDefsPath,
                                                       tiedListPath,
                                                       dictPath,
                                                       dfaPath,
                                                       baseHmmDefsPath,
                                                       baseTiedlistPath,
                                                       baseStatsPath,
                                                       baseMacrosPath,
                                                       samplePath,
                                                       lexiconPath,
                                                       grammarPath,
                                                       vocabPath,
                                                       promptsPath,
                                                       treeHedPath,
                                                       wavConfigPath,
                                                       scriptBasePrefix);
}
