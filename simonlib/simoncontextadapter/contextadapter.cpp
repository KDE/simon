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
    m_currentModelDeactivatedScenarios = QStringList("unknown");
    m_requestedDeactivatedScenarios = QStringList("unknown");
    m_currentScenarioSet = QStringList("unknown");
    m_newAcousticModel = true;
    m_currentSampleGroup = "default";
    m_requestedSampleGroup = "default";
    m_modelCache = QHash<QString, QString>();
    m_acousticModelCache = QHash<QString, QString>();
    m_currentActivity = ContextAdapter::NoActivity;

    connect(m_modelCompilationManager, SIGNAL(modelCompiled()),
            this, SLOT(hasNewlyGeneratedModel()));

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
    //connect(m_modelCompilationManager, SIGNAL(modelCompiled()),
    //        this, SIGNAL(modelCompiled()));
    connect(m_modelCompilationManager, SIGNAL(activeModelCompilationAborted()),
            this, SIGNAL(activeModelCompilationAborted()));

    //make sure that the cache directory and cache info files exist
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString acousticDir = cacheDir + "/acoustic models/";
    QString languageDir = cacheDir + "/language models/";
    QDir dir;

    if (!dir.exists(acousticDir))
    {
        kDebug() << "making acoustic cache directory";
        dir.mkpath(acousticDir);
    }
    if (!dir.exists(languageDir))
    {
        kDebug() << "making language cache directory";
        dir.mkpath(languageDir);
    }

    //load the base scenario list
    QFile scenariosFile(languageDir + "ScenarioList");
    if (scenariosFile.exists())
    {
        scenariosFile.open(QFile::ReadOnly);
        QDataStream fileStream(&scenariosFile);
        fileStream >> m_currentScenarioSet;
        scenariosFile.close();

        kDebug() << "Loaded base scenario list: " << m_currentScenarioSet;
    }

    //load the language model cache lookup hash table
    QFile lookupFile(languageDir + "CacheLookup");
    if (lookupFile.exists())
    {
        lookupFile.open(QFile::ReadOnly);
        QDataStream lookupStream(&lookupFile);
        lookupStream >> m_modelCache;
        lookupFile.close();

        kDebug() << "Loaded language model cache lookup: " << m_modelCache;
    }

    //load the deactivated scenarios list of the current model
    QFile deactivatedFile(languageDir + "DeactivatedList");
    if (scenariosFile.exists())
    {
        deactivatedFile.open(QFile::ReadOnly);
        QDataStream fileStream(&deactivatedFile);
        fileStream >> m_currentModelDeactivatedScenarios;
        deactivatedFile.close();

        kDebug() << "Loaded deactivated scenario list for current model: " << m_currentModelDeactivatedScenarios;
        m_requestedDeactivatedScenarios = m_currentModelDeactivatedScenarios;
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
        m_requestedSampleGroup = m_currentSampleGroup;
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

void ContextAdapter::aborted()
{
    m_currentActivity = ContextAdapter::NoActivity;

    //TODO: account for all of the possible reasons for compilation aborting.  There is probably no model or a partially complete model in the active folder...
    //the most common reason for this seems to be when there is no grammar (all scenarios disabled due to context), so this should probably trigger a silent deactivation of recognition.

//    m_currentModelDeactivatedScenarios = QStringList("unknown");

    if (m_currentlyCompilingDeactivatedScenarios.join(",") != m_requestedDeactivatedScenarios.join(","))
    {
        if (shouldRecompileModel())
        {
            kDebug() << "Forcing model recompilation after compilation was aborted due to need for recompilation.";
            emit forceModelRecompilation();
        }
    }
}

void ContextAdapter::updateDeactivatedScenarios(QStringList deactivatedScenarios)
{
    m_requestedDeactivatedScenarios = deactivatedScenarios;

    //if there is no compilation in progress, force a recompile, otherwise, one will be forced upon completion of the current compilation
    if (shouldRecompileModel())
    {
        kDebug() << "Forcing model recompilation due to a new deactivated scenarios list.";
        emit forceModelRecompilation();
    }
}

void ContextAdapter::updateAcousticModelSampleGroup(QString sampleGroup)
{
    m_requestedSampleGroup = sampleGroup;

    //TODO: setup recompiles similar to the ones done when the deactivated scenarios change
}

void ContextAdapter::storeLanguageModelInCache(QStringList deactivatedScenarios)
{
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString languageDir = cacheDir + "/language models/";
    QString cachedModelDir;
    QDir dir;
    QString deactivatedList = deactivatedScenarios.join(",");

    if (deactivatedList == "unknown")
    {
        kDebug() << "The current language model configuration is unknown, it cannot be stored.";
        return;
    }

    if (!QFile::exists(activeDir + "lexicon"))
    {
        kDebug() << "There is no language model to store";
        return;
    }

    if (m_modelCache.value(deactivatedList, "").isEmpty())
    {
        cachedModelDir.setNum(m_modelCache.count());
        kDebug() << "Adding the following entry to cache '" + deactivatedList + "'";
        m_modelCache.insert(deactivatedList, cachedModelDir);
        cachedModelDir = languageDir + cachedModelDir + "/";
        dir.mkpath(cachedModelDir);
        kDebug() << "With the following cache directory " + cachedModelDir;
    }
    else
    {
        kDebug() << "RE-Adding the following entry to cache '" + deactivatedList + "'";
        cachedModelDir = languageDir + m_modelCache.value(deactivatedList, "") + "/";
    }

    //make sure cache directory exists
    dir.mkpath(cachedModelDir);

    kDebug() << "Storing files in cache";

    dir.rename(activeDir+"lexicon", cachedModelDir+"lexicon");
    dir.rename(activeDir+"model.grammar", cachedModelDir+"model.grammar");
    dir.rename(activeDir+"simple.voca", cachedModelDir+"simple.voca");
    dir.rename(activeDir+"model.dict", cachedModelDir+"model.dict");
    dir.rename(activeDir+"model.dfa", cachedModelDir+"model.dfa");
    dir.rename(activeDir+"julius.log", cachedModelDir+"julius.log");
    dir.rename(activeDir+"julius.jconf", cachedModelDir+"julius.jconf");

    //save model cache lookup
    QFile lookupFile(languageDir + "CacheLookup");
    lookupFile.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream lookupStream(&lookupFile);
    lookupStream << m_modelCache;
    lookupFile.close();
    kDebug() << "Language model cache lookup saved: " << m_modelCache;
}


void ContextAdapter::storeAcousticModelInCache(QString sampleGroup)
{
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString acousticDir = cacheDir + "acoustic models/";
    QString cachedModelDir;
    QDir dir;

    kDebug() << "Storing acoustic model for sample group '" << sampleGroup << "' in cache.";
    cachedModelDir = acousticDir + sampleGroup + "/";

    //make sure cache directory exists
    dir.mkpath(cachedModelDir);

    dir.rename(activeDir+"hmmDefs", cachedModelDir+"hmmDefs");
    dir.rename(activeDir+"tiedList", cachedModelDir+"tiedList");

    //this might be completely stupid (having a hash that stores the key and value as the same thing)
    //but it was done as a quick change, and it could potentially allow the key or value to be derived differently if so desired
    m_acousticModelCache.insert(sampleGroup, sampleGroup);

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
}


void ContextAdapter::hasNewlyGeneratedModel()
{
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString languageDir = cacheDir + "/language models/";

    if (m_newAcousticModel)
    {
        m_newAcousticModel = false;
    }

    emit modelCompiled();
    m_currentModelDeactivatedScenarios = m_currentlyCompilingDeactivatedScenarios;
    m_currentActivity = ContextAdapter::NoActivity;

    //save the current model's deativated scenario set
    QFile lookupFile(languageDir + "DeactivatedList");
    lookupFile.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream lookupStream(&lookupFile);
    lookupStream << m_currentModelDeactivatedScenarios;
    lookupFile.close();
    kDebug() << "Current model deactivated scenario list saved: " << m_currentModelDeactivatedScenarios;

    if (shouldRecompileModel())
    {
        kDebug() << "Forcing model recompilation after completed compilation due to determination by ContextAdapter::shouldRecompileModel.";
        emit forceModelRecompilation();
    }
}

bool ContextAdapter::loadLanguageModelFromCache(QStringList deactivatedScenarios)
{
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString languageDir = cacheDir + "/language models/";
    QString cachedModelDir;
    QString deactivatedList = deactivatedScenarios.join(",");
    QDir dir;

    kDebug() << "Checking cache for the following entry: " + deactivatedList;

    if (!m_modelCache.value(deactivatedList, "").isEmpty())
    {
        kDebug() << "Entry found!  Retrieving files from cache location " << m_modelCache.value(deactivatedList);

        //if so, replace the active files with the cached ones and return
        cachedModelDir = languageDir + m_modelCache.value(deactivatedList) + "/";

        dir.rename(cachedModelDir+"lexicon", activeDir+"lexicon");
        dir.rename(cachedModelDir+"model.grammar", activeDir+"model.grammar");
        dir.rename(cachedModelDir+"simple.voca", activeDir+"simple.voca");
        dir.rename(cachedModelDir+"model.dict", activeDir+"model.dict");
        dir.rename(cachedModelDir+"model.dfa", activeDir+"model.dfa");
        dir.rename(cachedModelDir+"julius.log", activeDir+"julius.log");
        dir.rename(cachedModelDir+"julius.jconf", activeDir+"julius.jconf");

        return true;
    }

    return false;
}

bool ContextAdapter::loadAcousticModelFromCache(QString sampleGroup)
{
    QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/active/");
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString acousticDir = cacheDir + "acoustic models/";
    QDir dir;

    if (!m_acousticModelCache.value(sampleGroup, "").isEmpty())
    {
        QString cachedDir = acousticDir + m_acousticModelCache.value(sampleGroup);

        dir.rename(cachedDir+"hmmdefs", activeDir+"hmmdefs");
        dir.rename(cachedDir+"tiedlist", activeDir+"tiedlist");

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
    QString acousticDir = cacheDir + "/acoustic models/";
    QString languageDir = cacheDir + "/language models/";


    kDebug() << "Clearing cache";

    m_modelCache.clear();
    QFile lookupFile(languageDir + "CacheLookup");
    if (lookupFile.exists())
    {
        lookupFile.open(QFile::Truncate | QFile::WriteOnly);
        QDataStream lookupStream(&lookupFile);
        lookupStream << m_modelCache;
        lookupFile.close();

        kDebug() << "Language model cache lookup refreshed";
    }

    m_acousticModelCache.clear();
    lookupFile.setFileName(acousticDir + "CacheLookup");
    if (lookupFile.exists())
    {
        lookupFile.open(QFile::Truncate | QFile::WriteOnly);
        QDataStream lookupStream(&lookupFile);
        lookupStream << m_acousticModelCache;
        lookupFile.close();

        kDebug() << "Acoustic model cache lookup refreshed";
    }

    QDir initialDir = dir.current();

    //clear the acoustic model cache
    dir.setCurrent(acousticDir);
    cacheDirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (QString dirInfo, cacheDirs)
    {
        dir.setCurrent(dir.absolutePath() + "/" + dirInfo);
        cachedFiles = dir.entryList(QDir::Files);
        foreach (QString fileInfo, cachedFiles)
        {
            QFile::remove(fileInfo);
        }
        dir.setCurrent(acousticDir);
        dir.rmdir(dirInfo);
    }

    //clear the language model cache
    dir.setCurrent(languageDir);
    cacheDirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (QString dirInfo, cacheDirs)
    {
        kDebug() << "clearing " + dirInfo;
        dir.setCurrent(dir.absolutePath() + "/" + dirInfo);
        cachedFiles = dir.entryList(QDir::Files);
        foreach (QString fileInfo, cachedFiles)
        {
            kDebug() << "removing file " + fileInfo;
            QFile::remove(fileInfo);
        }
        dir.setCurrent(languageDir);
        dir.rmdir(dirInfo);
    }

    dir.setCurrent(initialDir.dirName());

    m_currentModelDeactivatedScenarios = QStringList("unknown");
}

bool ContextAdapter::shouldRecompileModel()
{
    if (m_requestedDeactivatedScenarios.join(",") == m_currentModelDeactivatedScenarios.join(","))
    {
        kDebug() << "requested deactivated list is the same as the current model's!";
        return false;
    }
    else if (m_currentActivity != ContextAdapter::NoActivity)
    {
        kDebug() << "contextAdapter is busy, recompile will occur when it is finished";
        return false;
    }

    return true;
}

bool ContextAdapter::startAdaption(ModelCompilationAdapter::AdaptionType adaptionType, const QString& lexiconPathOut,
                                   const QString& grammarPathOut, const QString& simpleVocabPathOut,
                                   const QString& promptsPathOut, const QStringList& scenarioPathsIn,
                                   const QString& promptsIn)
{
    m_currentActivity = ContextAdapter::CompilingModel;

    //Get paths for caching
    QString cacheDir = KStandardDirs::locateLocal("appdata", "models/"+m_username+"/cached/");
    QString languageDir = cacheDir + "/language models/";

    //check to see if the base scenarios are different (in which case the cache would need to be cleared)
    if (m_currentScenarioSet.join(",") != scenarioPathsIn.join(","))
    {
        kDebug() << "There is a new base scenario list!";
        clearCache();

        m_currentScenarioSet = scenarioPathsIn;

        //do we need to generate a new acoustic model?
        if (ModelCompilationAdapter::AdaptAcousticModel & adaptionType)
        {
            kDebug() << "A new acoustic model must be generated";
            m_newAcousticModel = true;
        }

        //save the new base scenario list
        QFile scenariosFile(languageDir + "ScenarioList");
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

    //if for some reason newAcousticModel is true (right now a cache refresh will do it)
    //but a static model is being used, set it to false
    if (!ModelCompilationAdapter::AdaptAcousticModel & adaptionType && m_newAcousticModel)
    {
        m_newAcousticModel = false;
    }

    //get the list of NOT deactivated scenario paths
    m_currentlyCompilingDeactivatedScenarios = m_requestedDeactivatedScenarios;
    QStringList activeScenarioPathsIn;
    QRegExp slashes = QRegExp("/+");
    bool hasDeactivatedScenarios = false;
    foreach (QString path, scenarioPathsIn)
    {
        if (!m_currentlyCompilingDeactivatedScenarios.contains(path.split(slashes).back()))
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

    //if the new scenario set does not have any deactivated scenarios, the m_requestedDeactivatedScenarios list will not be refreshed,
    //so this just clears it if that is the case (it will also clear it if it is just empty anyways)
    if (!hasDeactivatedScenarios)
    {
        m_requestedDeactivatedScenarios.clear();
        m_currentlyCompilingDeactivatedScenarios.clear();
    }

    //adapt the model
    if (m_newAcousticModel)
    {
        m_compilingSampleGroup = m_requestedSampleGroup;

        storeLanguageModelInCache(m_currentModelDeactivatedScenarios);
        //storeAcousticModelInCache(m_currentSampleGroup);
        m_currentlyCompilingDeactivatedScenarios.clear();
        //when the acoustic model is recompiled, all scenarios are used in the speech model
        //and then the language model is recompiled with only the active scenarios via the forceModelRecompilation() signal
        kDebug() << "Compiling acoustic model as well as language model with all scenarios";
        return m_modelCompilationAdapter->startAdaption(adaptionType, lexiconPathOut,
                                                            grammarPathOut, simpleVocabPathOut,
                                                            promptsPathOut, scenarioPathsIn,
                                                            promptsIn);
    }
    //else if (m_currentSampleGroup != m_activeSampleGroup)
    //{
    //  //check the cache for that sample group and start an adaption like above otherwise
    //}
    else
    {
        storeLanguageModelInCache(m_currentModelDeactivatedScenarios);
        //see if these are in the cache
        if (loadLanguageModelFromCache(m_currentlyCompilingDeactivatedScenarios))
        {
            emit modelLoadedFromCache();
            m_currentModelDeactivatedScenarios = m_currentlyCompilingDeactivatedScenarios;
            m_currentActivity = ContextAdapter::NoActivity;

            //save the current model's deativated scenario set
            QFile lookupFile(languageDir + "DeactivatedList");
            lookupFile.open(QFile::WriteOnly | QFile::Truncate);
            QDataStream lookupStream(&lookupFile);
            lookupStream << m_currentModelDeactivatedScenarios;
            lookupFile.close();
            kDebug() << "Current model deactivated scenario list saved: " << m_currentModelDeactivatedScenarios;

            if (shouldRecompileModel())
            {
                emit forceModelRecompilation();
            }

            return true;
        }

        kDebug() << "Compiling just the language model with the currently active scenarios";
        return m_modelCompilationAdapter->startAdaption(ModelCompilationAdapter::AdaptLanguageModel, lexiconPathOut,
                                                            grammarPathOut, simpleVocabPathOut,
                                                            promptsPathOut, activeScenarioPathsIn,
                                                            promptsIn);
    }
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
