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
    m_currentSampleGroup = "default";
    m_modelCache = QHash<QString, QString>();
    m_acousticModelCache = QHash<QString, QString>();

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
    QString acousticDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/acoustic models/");
    QDir dir;
    if (!dir.exists(cacheDir))
    {
        kDebug() << "making cache directory";
        dir.mkpath(cacheDir);
    }     
    if (!dir.exists(acousticDir))
    {
        kDebug() << "making acoustic cache directory";
        dir.mkpath(acousticDir);
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

    //load the language model cache lookup hash table
    QFile lookupFile(cacheDir + "CacheLookup");
    if (lookupFile.exists())
    {
        lookupFile.open(QFile::ReadOnly);
        QDataStream lookupStream(&lookupFile);
        lookupStream >> m_modelCache;
        lookupFile.close();

        kDebug() << "Loaded model cache lookup: " << m_modelCache;
    }

    //load the current Sample Group
    QFile sampleGroupFile(acousticDir + "SampleGroup");
    if (sampleGroupFile.exists())
    {
        sampleGroupFile.open(QFile::ReadOnly);
        QDataStream fileStream(&sampleGroupFile);
        fileStream >> m_currentSampleGroup;
        sampleGroupFile.close();

        kDebug() << "Loaded current sample group: " << m_currentSampleGroup;
    }

    //load the acoustic model cache lookup hash table
    lookupFile.setFileName(acousticDir + "CacheLookup");
    if (lookupFile.exists())
    {
        lookupFile.open(QFile::ReadOnly);
        QDataStream lookupStream(&lookupFile);
        lookupStream >> m_acousticModelCache;
        lookupFile.close();

        kDebug() << "Loaded acoustic model cache lookup: " << m_acousticModelCache;
    }
}

ContextAdapter::~ContextAdapter()
{
    if (m_modelCompilationManager)
        m_modelCompilationManager->deleteLater();
    if (m_modelCompilationAdapter)
        m_modelCompilationAdapter->deleteLater();
}

bool ContextAdapter::updateDeactivatedScenarios(QStringList deactivatedScenarios)
{
    if (m_deactivatedScenarios.join(",") != deactivatedScenarios.join(","))
    {
        m_deactivatedScenarios = deactivatedScenarios;
        return true;
    }

    return false;
}

bool ContextAdapter::updateAcousticModelSampleGroup(QString sampleGroup)
{
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");

    if (m_currentSampleGroup != sampleGroup)
    {
        m_currentSampleGroup = sampleGroup;

        return true;
    }

    return false;
}

void ContextAdapter::storeModelInCache()
{
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString acousticDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/acoustic models/");
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
    kDebug() << "Language model cache lookup saved: " << m_modelCache;

    //check to see if this was an initial acoustic model compilation
    //if so, the acoustic model needs to be cached, and the language model may need to be recompiled
    if (m_newAcousticModel)
    {
        m_newAcousticModel = false;

        kDebug() << "Storing acoustic model for sample group '" << m_currentSampleGroup << "' in cache.";
        cachedModelDir = acousticDir + m_currentSampleGroup + "/";
        QFile::copy(activeDir+"hmmDefs", cachedModelDir+"hmmDefs");
        QFile::copy(activeDir+"tiedList", cachedModelDir+"tiedList");

        m_acousticModelCache.insert(m_currentSampleGroup, cachedModelDir);
        //save model cache lookup
        QFile lookupFile(acousticDir + "CacheLookup");
        lookupFile.open(QFile::WriteOnly | QFile::Truncate);
        QDataStream lookupStream(&lookupFile);
        lookupStream << m_acousticModelCache;
        lookupFile.close();
        kDebug() << "Acoustic model cache lookup saved: " << m_acousticModelCache;

        //save current sample group
        QFile sampleGroupFile(acousticDir + "SampleGroup");
        sampleGroupFile.open(QFile::WriteOnly | QFile::Truncate);
        QDataStream sampleGroupStream(&sampleGroupFile);
        sampleGroupStream << m_currentSampleGroup;
        sampleGroupFile.close();

        kDebug() << "The speech model which includes all scenarios has been compiled.  Now the language model will be recompiled to only include the active scenarios.";
        emit forceModelRecompilation();
    }
}

bool ContextAdapter::loadLanguageModelFromCache()
{
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString cachedModelDir;
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

        return true;
    }

    return false;
}

bool ContextAdapter::loadAcousticModelFromCache()
{
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");

    if (!m_acousticModelCache.value(m_currentSampleGroup, "").isEmpty())
    {
        QString cachedDir = m_acousticModelCache.value(m_currentSampleGroup);

        QFile::remove(activeDir+"hmmdefs");
        QFile::copy(cachedDir+"hmmdefs", activeDir+"hmmdefs");
        QFile::remove(activeDir+"tiedlist");
        QFile::copy(cachedDir+"tiedlist", activeDir+"tiedlist");

        return true;
    }

    return false;
}

void ContextAdapter::clearCache()
{
    QDir dir;
    QStringList cacheDirs;
    QStringList cachedFiles;
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString acousticDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/acoustic models/");

    kDebug() << "Clearing language model cache";

    m_modelCache.clear();


    QFile lookupFile(cacheDir + "CacheLookup");
    if (lookupFile.exists())
    {
        lookupFile.open(QFile::Truncate);
        QDataStream lookupStream(&lookupFile);
        lookupStream >> m_modelCache;
        lookupFile.close();

        kDebug() << "Language model cache lookup refreshed";
    }


    QDir initialDir = dir.current();

    //clear the acoustic model cache
    dir.setCurrent(acousticDir);
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

    //clear the language model cache
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

    //remake the acoustic model cache directory (this should probably be changed so that the language cache is in a subdirectory on the same level as the acoustic model cache)
    dir.mkpath(acousticDir);

    lookupFile.setFileName(acousticDir + "CacheLookup");
    if (lookupFile.exists())
    {
        lookupFile.open(QFile::Truncate);
        QDataStream lookupStream(&lookupFile);
        lookupStream >> m_acousticModelCache;
        lookupFile.close();

        kDebug() << "Acoustic model cache lookup refreshed";
    }
}

bool ContextAdapter::startAdaption(ModelCompilationAdapter::AdaptionType adaptionType, const QString& lexiconPathOut,
                                   const QString& grammarPathOut, const QString& simpleVocabPathOut,
                                   const QString& promptsPathOut, const QStringList& scenarioPathsIn,
                                   const QString& promptsIn)
{
    //Get paths for caching
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");

    //check to see if the base scenarios are different (in which case the cache would need to be cleared)
    if (m_currentScenarioSet.join(",") != scenarioPathsIn.join(","))
    {
        kDebug() << "There is a new base scenario list!";
        clearCache();

        m_currentScenarioSet = scenarioPathsIn;

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
        fileStream << m_currentScenarioSet;
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
    bool hasDeactivatedScenarios = false;
    foreach (QString path, scenarioPathsIn)
    {
        if (!m_deactivatedScenarios.contains(path.split(slashes).back()))
        {
            kDebug() << path.split(slashes).back() + " is active";
            activeScenarioPathsIn << path;
        }
        else
        {
            kDebug() << path.split(slashes).back() + " is not active";
            hasDeactivatedScenarios = true;
        }
    }

    //if the new scenario set does not have any deactivated scenarios, the m_deactivatedScenarios list will not be refreshed,
    //so this just clears it if that is the case (it will also clear it if it is just empty anyways)
    if (!hasDeactivatedScenarios)
    {
        m_deactivatedScenarios.clear();
    }

    //see if these are in the cache
    if (loadLanguageModelFromCache())
    {
        emit modelLoadedFromCache();
        return true;
    }

    //otherwise, adapt the model
    if (m_newAcousticModel)
    {
        //when the acoustic model is recompiled, all scenarios are used in the speech model
        //and then the language model is recompiled with only the active scenarios via the forceModelRecompilation() signal
        kDebug() << "Compiling acoustic model as well as language model with all scenarios";
        success =  m_modelCompilationAdapter->startAdaption(adaptionType, lexiconPathOut,
                                                            grammarPathOut, simpleVocabPathOut,
                                                            promptsPathOut, scenarioPathsIn,
                                                            promptsIn);
    }
    else
    {
        kDebug() << "Compiling just the language model with the currently active scenarios";
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
