#include "contextadapter.h"
#include <KDebug>
#include <QDir>
#include <KStandardDirs>
#include <QTextStream>
#include <KDateTime>
#include <KConfigGroup>

ContextAdapter::ContextAdapter(QString username, QObject *parent) :
    QObject(parent)
{
    m_modelCompilationManager = new ModelCompilationManager(username, this);
    m_modelCompilationAdapter = new ModelCompilationAdapterHTK(username, this);
    m_username = username;
    m_deactivatedScenarios = QStringList();
    m_currentScenarioSet = QStringList();
    m_newAcousticModel = false;

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
    QString deactivatedList = m_deactivatedScenarios.join(",");

    if (m_newAcousticModel)
    {
        deactivatedList.clear();
    }

    cachedModelDir.setNum(m_modelCache.count());
    kDebug() << "Adding the following entry to cache '" + deactivatedList + "'";
    m_modelCache.insert(deactivatedList, cachedModelDir);
    cachedModelDir = cacheDir + cachedModelDir + "/";
    dir.mkpath(cachedModelDir);
    kDebug() << "With the following cache directory " + cachedModelDir;

    kDebug() << "Storing files in cache";

    QFile::copy(activeDir+"lexicon", cachedModelDir+"lexicon");
    QFile::copy(activeDir+"model.grammar", cachedModelDir+"model.grammar");
    QFile::copy(activeDir+"simple.voca", cachedModelDir+"simple.voca");
    QFile::copy(activeDir+"model.dict", cachedModelDir+"model.dict");
    QFile::copy(activeDir+"model.dfa", cachedModelDir+"model.dfa");
    QFile::copy(activeDir+"activerc", cachedModelDir+"activerc");
    QFile::copy(activeDir+"julius.log", cachedModelDir+"julius.log");
    QFile::copy(activeDir+"julius.jconf", cachedModelDir+"julius.jconf");

    //save model cache lookup
    QFile lookupFile(cacheDir + "CacheLookup");
    lookupFile.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream lookupStream(&lookupFile);
    lookupStream << m_modelCache;
    lookupFile.close();
    kDebug() << "Model cache lookup saved: " << m_modelCache;

    //check to see if this was an initial acoustic model compilation
    //if so, the language model may need to be recompiled
    if (m_newAcousticModel)
    {
        m_newAcousticModel = false;

        kDebug() << "The speech model which includes all scenarios has been compiled.  Now the language model will be recompiled to only include the active scenarios.";
        emit forceModelRecompilation();
    }
}

void ContextAdapter::clearCache()
{
    QDir dir;
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
            QFile::remove(fileInfo);
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

    //check to see if the base scenarios are different (in which case the cache would need to be cleared)
    if (m_currentScenarioSet.join(",") != scenarioPathsIn.join(","))
    {
        kDebug() << "There is a new base scenario list!";
        clearCache();

        m_currentScenarioSet = scenarioPathsIn;

        //if the new scenario set does not have any deactivated scenarios, the m_deactivatedScenarios list will not be refreshed,
        //so this just clears it if that is the case
        bool deactivationChanged = false;
        foreach(QString deactivatedScenario, m_deactivatedScenarios)
        {
            if (!m_currentScenarioSet.contains(deactivatedScenario))
            {
                deactivationChanged = true;
                break;
            }
        }
        if (deactivationChanged)
        {
            m_deactivatedScenarios.clear();
        }

        //we need to generate a new acoustic model
        if (ModelCompilationAdapter::AdaptAcousticModel & adaptionType)
        {
            kDebug() << "A new acoustic model must be generated";
            m_newAcousticModel = true;
        }


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

        QFile::remove(activeDir+"lexicon");
        QFile::copy(cachedModelDir+"lexicon", activeDir+"lexicon");
        QFile::remove(activeDir+"model.grammar");
        QFile::copy(cachedModelDir+"model.grammar", activeDir+"model.grammar");
        QFile::remove(activeDir+"simple.voca");
        QFile::copy(cachedModelDir+"simple.voca", activeDir+"simple.voca");
        QFile::remove(activeDir+"model.dict");
        QFile::copy(cachedModelDir+"model.dict", activeDir+"model.dict");
        QFile::remove(activeDir+"model.dfa");
        QFile::copy(cachedModelDir+"model.dfa", activeDir+"model.dfa");
        QFile::remove(activeDir+"activerc");
        QFile::copy(cachedModelDir+"activerc", activeDir+"activerc");
        QFile::remove(activeDir+"julius.log");
        QFile::copy(cachedModelDir+"julius.log", activeDir+"julius.log");
        QFile::remove(activeDir+"julius.jconf");
        QFile::copy(cachedModelDir+"julius.jconf", activeDir+"julius.jconf");

        //update the date of the current model
        KConfig config( activeDir+"activerc", KConfig::SimpleConfig );
        KConfigGroup cGroup(&config, "");
        cGroup.writeEntry("Date", KDateTime::currentUtcDateTime().dateTime());
        config.sync();

        emit modelLoadedFromCache();
        return true;
    }

    //otherwise, adapt the model
    if (m_newAcousticModel)
    {
        //when the acoustic model is recompiled, all scenarios are used in the speech model
        //and then the language model is recompiled with only the active scenarios via the forceModelRecompilation() signal
        success =  m_modelCompilationAdapter->startAdaption(adaptionType, lexiconPathOut,
                                                            grammarPathOut, simpleVocabPathOut,
                                                            promptsPathOut, scenarioPathsIn,
                                                            promptsIn);
    }
    else
    {
        success =  m_modelCompilationAdapter->startAdaption(ModelCompilationAdapter::AdaptLanguageModel, lexiconPathOut,
                                                            grammarPathOut, simpleVocabPathOut,
                                                            promptsPathOut, activeScenarioPathsIn,
                                                            promptsIn);
    }

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
    if (m_newAcousticModel)
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
    else
    {
        return m_modelCompilationManager->startCompilation(ModelCompilationManager::CompileLanguageModel,
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
}
