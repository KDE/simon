/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "modelcompilerhtk.h"
#include "audiocopyconfig.h"
#include "reestimationconfig.h"
#include "simonutils/fileutils.h"

#include <simonlogging/logger.h>

#include <QtCore/qmath.h>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QString>
#include <QVector>
#include <QtConcurrentMap>
#include <QMutexLocker>

#include <KUrl>
#include <KConfig>
#include <KConfigGroup>
#include <KStandardDirs>
#include <KTar>
#include <KComponentData>
#include <KAboutData>
#include <KLocale>
#include <KDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif
#ifdef Q_WS_X11
#include <X11/Xproto.h>
#endif

#ifdef Q_OS_WIN
#undef HTK_UNICODE
#endif
#ifdef Q_OS_UNIX
#define HTK_UNICODE
#endif
#undef HTK_UNICODE

#define MIN_WAV_FILESIZE 45 //44 byte is the length of the header
#define HEREST_MULTITHREADED

bool codeAudioDataFromScpHelper(AudioCopyConfig *config)
{
  return config->manager()->codeAudioDataFromScp(config->path());
}

bool reestimateHelper(ReestimationConfig *config)
{
  return config->manager()->reestimate(config->command());
}

ModelCompilerHTK::ModelCompilerHTK(const QString& user_name, QObject* parent) : 
    ModelCompiler(user_name, parent),
    catchUndefiniedPhonemes(false)
{
  connect(this, SIGNAL(status(QString,int,int)), this, SLOT(addStatusToLog(QString)));
}

QString ModelCompilerHTK::htkIfyPath(const QString& in)
{
  QString out = in;

  #ifdef Q_OS_WIN
  long     length = 0;
  char*   buffer = 0;

  // First obtain the size needed by passing 0 and 0.
  length = GetShortPathNameA(out.toLocal8Bit().data(), 0, 0);
  if (length == 0) return QByteArray();

  // Dynamically allocate the correct size
  // (terminating null char was included in length)
  buffer = new char[length];

  // Now simply call again using same long path.
  length = GetShortPathNameA(out.toLocal8Bit().data(), buffer, length);
  if (length == 0) return QByteArray();

  out = QString::fromLocal8Bit(buffer);
  delete [] buffer;
  #endif

  return out;
}


bool ModelCompilerHTK::createDirs()
{
  tempDir = KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName()+'/'+userName+"/compile/");

  if (tempDir.isEmpty()) return false;

  QDir tempDirHandle(tempDir);
  if (!tempDirHandle.exists())
    return false;

  if (!tempDirHandle.exists("xforms") && !tempDirHandle.mkdir("xforms"))
    return false;
  if (!tempDirHandle.exists("classes") && !tempDirHandle.mkdir("classes"))
    return false;

  for (int i=0; i < 16+(getMixtureConfigs().count()*3); i++) {
    if (!tempDirHandle.exists("hmm"+QString::number(i)) &&
      !tempDirHandle.mkdir("hmm"+QString::number(i)))
      return false;
  }
  if (!tempDirHandle.exists("hmmout") &&
      !tempDirHandle.mkdir("hmmout"))
      return false;
  if (!tempDirHandle.exists("base") &&
      !tempDirHandle.mkdir("base"))
      return false;

  return true;
}


bool ModelCompilerHTK::parseConfiguration()
{
  KConfig config( KStandardDirs::locateLocal("config", "simonmodelcompilationrc"), KConfig::FullConfig );
  KConfigGroup programGroup(&config, "Programs");

  if ((compilationType & ModelCompilerHTK::CompileSpeechModel)||
  (compilationType & ModelCompilerHTK::AdaptSpeechModel)) {
    hDMan = programGroup.readEntry("HDMan", KUrl(KStandardDirs::findExe("HDMan"))).toLocalFile();
    hLEd = programGroup.readEntry("HLEd", KUrl(KStandardDirs::findExe("HLEd"))).toLocalFile();
    hCopy = programGroup.readEntry("HCopy", KUrl(KStandardDirs::findExe("HCopy"))).toLocalFile();
    hCompV = programGroup.readEntry("HCompV", KUrl(KStandardDirs::findExe("HCompV"))).toLocalFile();
    hERest = programGroup.readEntry("HERest", KUrl(KStandardDirs::findExe("HERest"))).toLocalFile();
    hHEd = programGroup.readEntry("HHEd", KUrl(KStandardDirs::findExe("HHEd"))).toLocalFile();
    hVite = programGroup.readEntry("HVite", KUrl(KStandardDirs::findExe("HVite"))).toLocalFile();
  }
  if (compilationType & ModelCompilerHTK::CompileLanguageModel) {
    mkfa = programGroup.readEntry("mkfa", KUrl(KStandardDirs::findExe("mkfa"))).toLocalFile();
    dfaMinimize = programGroup.readEntry("dfa_minimize", KUrl(KStandardDirs::findExe("dfa_minimize"))).toLocalFile();
  }

  if ((compilationType & ModelCompilerHTK::CompileSpeechModel) &&
    (!QFile::exists(hDMan) || !QFile::exists(hCopy) ||
    !QFile::exists(hCompV) || !QFile::exists(hERest) ||
  !QFile::exists(hVite))) {
    //HTK not found
    #ifdef Q_OS_WIN32
    QString errorMsg = i18n("The HTK cannot be found. Please make sure it is installed correctly.\n\nMore information: http://www.cyber-byte.at/wiki/index.php/English:_Setup#Windows");
    #else
    QString errorMsg = i18n("The HTK cannot be found. Please make sure it is installed correctly.\n\nMore information: http://www.cyber-byte.at/wiki/index.php/English:_Setup#HTK_Installation");
    #endif
    emit error(errorMsg);
    return false;
  }

  if ((compilationType & ModelCompilerHTK::CompileLanguageModel) &&
  (!QFile::exists(mkfa) || !QFile::exists(dfaMinimize))) {
    //julius grammar tools not found
    emit error(i18n("The julius related grammar tools mkfa and dfa_minimize cannot be found.\n\nA reinstallation of simon could solve this problem."));
    return false;
  }

  return true;
}

bool ModelCompilerHTK::removePhoneme(const QByteArray& phoneme)
{
  //check 
  QFile f1(tempDir+"lexicon");
  QFile f2(tempDir+"lexicon2");
  if (!f1.open(QIODevice::ReadOnly) || !f2.open(QIODevice::WriteOnly))
    return false;
  while (!f1.atEnd())
  {
    QByteArray line = f1.readLine();
    if (line.contains(' '+phoneme) || line.contains(phoneme+' ') || line.contains('\t'+phoneme))
      continue;
    f2.write(line);
  }
  f1.close();
  f2.close();
  if (!f1.remove())
    return false;
  return f2.rename(tempDir+"lexicon2", tempDir+"lexicon");
}

/**
 * \brief Processes an error (reacts on it some way)
 * \author Peter Grasch
 * @return If this is true we knew what to do; if this is false you'd better throw an error message
 */
bool ModelCompilerHTK::processError()
{
  QString err = getBuildLog().trimmed();

  int startIndex=0;
  if (err.contains("ERROR [+2019]")) {
                                                  // no training samples

    emit error(i18n("No training material available.\n\nPlease train your acoustic model by recording samples."));
    return true;
  }
                                                  //word missing
  if ((startIndex = err.indexOf("ERROR [+1232]")) != -1) {
    //ERROR [+1232]  NumParts: Cannot find word DARAUFFOLGEND in dictionary
    int wordstart = 42+startIndex;
    QString word = err.mid(wordstart, err.indexOf(' ', wordstart)-wordstart);

    //this error ONLY occurs when there are samples for the word but the word itself was not added
    //to the dictionary so - ADD THE WORD!
    emit wordUndefined(word);
    return true;
  }
  if ((startIndex = err.indexOf("ERROR [+2662]")) != -1) {
    // 		"ERROR [+2662]  FindProtoModel: no proto for E in hSet
    // 		 FindProtoModel: no proto for n
    QString phoneme = err.mid(44+startIndex);
    phoneme = phoneme.left(phoneme.indexOf(' '));

    if (catchUndefiniedPhonemes)
    {
      undefinedPhoneme = phoneme.toLocal8Bit();
      buildLog.replace("ERROR [+2662]", "ERROR  [+2662]"); // marked as processed; Yes, thats an awful hack
    }
    else
      emit phonemeUndefined(phoneme);

    return true;
  }
  if ((startIndex = err.indexOf("undefined class \"")) != -1) {
  /*		Error:       undefined class "NOM"*/
    QString undefClass = err.mid(startIndex+17);
    undefClass = undefClass.left(undefClass.indexOf('"'));
    emit classUndefined(undefClass);
    return true;
  }

  return false;
}


//void ModelCompilerHTK::abort()
//{
//  kDebug() << "Compilation Manager Aborting.";

//  keepGoing=false;

//  foreach (QProcess *proc, activeProcesses)
//  //tell any running process to commit suicide
//    proc->kill();
//}

bool ModelCompilerHTK::startCompilation ( ModelCompiler::CompilationType compilationType, const QString& modelDestination,
  const QString& baseModelPath, const QHash< QString, QString >& args )
{
  QString samplePath = args.value("samples");
  QString lexiconPath = args.value("lexicon");
  QString grammarPath = args.value("grammar");
  QString vocabPath = args.value("vocab");
  QString promptsPath = args.value("prompts");
  QString scriptBasePrefix = args.value("scriptBase");
  kDebug() << modelDestination << baseModelPath << args;
  return compile(compilationType, modelDestination, baseModelPath, samplePath, lexiconPath, grammarPath, vocabPath, promptsPath, 
                 scriptBasePrefix);
}

bool ModelCompilerHTK::pack ( const QString& targetArchive, const QString& name )
{
  QHash<QString, QByteArray> fm;
  QDomDocument DomDocument;
  getMetaData(name, "HTK").serializeXml(DomDocument);
  fm.insert("metadata.xml", DomDocument.toByteArray());

  QHash<QString, QString> efm;

  QString jconfFile = KStandardDirs::locate("data", "models/"+userName+"/active/julius.jconf");
  if (!QFile::exists(jconfFile))
    jconfFile = KStandardDirs::locate("data", "simond/default.jconf");

  efm.insert(tempDir+"hmmout/hmmdefs", "hmmdefs");
  efm.insert(tempDir+"tiedlist", "tiedlist");
  efm.insert(tempDir+"hmmout/macros", "macros");
  efm.insert(tempDir+"stats", "stats");
  efm.insert(tempDir+"model.dict", "model.dict");
  efm.insert(tempDir+"model.dfa", "model.dfa");
  efm.insert(jconfFile, "julius.jconf");

  return FileUtils::pack(targetArchive, fm, efm);
}

bool ModelCompilerHTK::unpack ( const QString& archive, const QString& targetDir )
{
  return FileUtils::unpack(archive, targetDir, (QStringList() << "hmmdefs" << "tiedlist" << "macros" << "stats"));
}


bool ModelCompilerHTK::compile(ModelCompiler::CompilationType compilationType,
  const QString& destinationPath,
  const QString& baseModelPath,
  const QString& samplePath,
  const QString& lexiconPath, const QString& grammarPath,
  const QString& vocabPath, const QString& promptsPath,
  const QString& scriptBasePrefix)
{
  kDebug() << "Compiling model";
  this->compilationType = compilationType;

  this->samplePath = samplePath;

  this->lexiconPath = lexiconPath;
  this->grammarPath = grammarPath;
  this->vocabPath = vocabPath;
  this->promptsPath = promptsPath;
  
  this->scriptBasePrefix = scriptBasePrefix;
  
  this->treeHedPath = getScriptFile("tree1.hed");
  this->wavConfigPath = getScriptFile("wav_config");

  keepGoing=true;

  clearLog();

  if (!parseConfiguration())
    return false;

  if (!createDirs()) {
    analyseError(i18nc("%1 is path to the temporary folder", "Could not generate temporary folders.\n\nPlease check your permissions for \"%1\".", tempDir));
    return false;
  }
  
  QString modelName = i18nc("%1 is username", "Simond: %1", userName);
  kDebug() << "Unpacking";
  if ((compilationType &  ModelCompilerHTK::AdaptSpeechModel) ||
      !(compilationType &  ModelCompilerHTK::CompileSpeechModel)) {
    //base model needed - unpack it and fail if its not here
    if ( !unpack(baseModelPath, tempDir+"base/")) {
      analyseError(i18nc("%1 is path to the base model", "Could not open base model at \"%1\".", baseModelPath));
      return false;
    }
  }
  
  baseHmmDefsPath = tempDir+ "base/hmmdefs";
  baseTiedlistPath =tempDir+ "base/tiedlist";
  baseMacrosPath = tempDir+ "base/macros";
  baseStatsPath = tempDir+ "base/stats";

  if (!keepGoing) return false;
                       
  Logger::log("Compiling model...");
  emit status(i18n("Preparation"), 0);

  if ((compilationType & ModelCompilerHTK::CompileSpeechModel) ||
  (compilationType & ModelCompilerHTK::AdaptSpeechModel)) {
    if (!generateInputFiles()) return false;
    if (!makeTranscriptions()) return false;
    if (!codeAudioData()) return false;
  }

  if (compilationType & ModelCompilerHTK::AdaptSpeechModel)
    if (!adaptBaseModel()) return false;

  if (compilationType & ModelCompilerHTK::CompileSpeechModel)
    if (!buildHMM()) return false;

  if (compilationType & ModelCompilerHTK::CompileLanguageModel) {
    if (!compileGrammar()) return false;
  }
  
  if (!(compilationType &  ModelCompilerHTK::AdaptSpeechModel) && 
      !(compilationType &  ModelCompilerHTK::CompileSpeechModel)) {
    if ((QFile::exists(tempDir+"hmmout/hmmdefs") && !QFile::remove(tempDir+"hmmout/hmmdefs")) || 
        (QFile::exists(tempDir+"hmmout/macros") && !QFile::remove(tempDir+"hmmout/macros")) || 
        (QFile::exists(tempDir+"tiedlist") && !QFile::remove(tempDir+"tiedlist")) || 
        (QFile::exists(tempDir+"stats") && !QFile::remove(tempDir+"stats"))) {
      analyseError(i18n("Could not remove old model"));
      return false;
    }
    if (!QFile::rename(tempDir+"base/hmmdefs", tempDir+"hmmout/hmmdefs") ||
        !QFile::rename(tempDir+"base/tiedlist", tempDir+"tiedlist") ||
        !QFile::rename(tempDir+"base/macros", tempDir+"hmmout/macros") ||
        !QFile::rename(tempDir+"base/stats", tempDir+"stats")) {
      analyseError(i18n("Invalid base model"));
    }
  }
  
  if (!keepGoing) return false;
                       
  pack(destinationPath, QString("Unnamed model from user %1").arg(userName));

  emit status(i18nc("Finished the model compilation", "Finished"), 2600, 2600);
  emit modelCompiled();
  QCoreApplication::sendPostedEvents(0, 0);
  return true;
}


bool ModelCompilerHTK::compileGrammar()
{
  if (!keepGoing) return false;
  emit status(i18n("Generating reverse grammar..."), 2510);
  if (!generateReverseGrammar()) {
    analyseError(i18n("Could not create reverse grammar.\n\nDid you define a grammar?"));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating temporary vocabulary..."), 2530);
  if (!makeTempVocab()) {
    analyseError(i18n("Could not create temporary vocabulary."));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating DFA..."), 2550);
  if (!makeDfa()) {
    analyseError(i18nc("%1 is path to mkfa tool, %2 is path to dfaminimize",
                       "Could not generate dfa. Please check the paths to mkfa and dfa_minimize (%1, %2).", mkfa, dfaMinimize));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating grammar dictionary..."), 2579);
  if (!generateDict()) {
    analyseError(i18n("Could not generate grammatical lexicon."));
    return false;
  }

  return true;
}


bool ModelCompilerHTK::makeTempVocab()
{
  QFile vocab ( vocabPath );

  QString terminal;
  if ( !vocab.open ( QFile::ReadOnly ) ) return false;

  QFile tmpVocab ( tempDir+"/tempvoca" );
  if ( !tmpVocab.open ( QFile::WriteOnly ) ) return false;

  QFile term ( tempDir+"/term" );
  if ( !term.open ( QFile::WriteOnly ) ) return false;

  QString vocabEntry;

  int termid=0;
  while ( !vocab.atEnd() ) {
    vocabEntry = QString::fromUtf8(vocab.readLine ( 1024 ));
    vocabEntry.remove ( QRegExp ( "\r+$" ) );
    vocabEntry.remove ( QRegExp ( "#.*" ) );
    vocabEntry = vocabEntry.trimmed();
    if ( vocabEntry.isEmpty() ) continue;
    if ( vocabEntry.startsWith ( '%' ) ) {
      terminal = vocabEntry.mid ( 1 ).trimmed();
      tmpVocab.write ( '#'+terminal.toUtf8() +'\n' );

      term.write ( QString::number(termid).toUtf8()+'\t'+terminal.toUtf8() +'\n' );
      termid++;
    }
  }
  vocab.close();
  tmpVocab.close();
  term.close();
  return true;
}


bool ModelCompilerHTK::makeDfa()
{
  QString execStr = '"'+mkfa+"\" -e1 -fg \""+tempDir+"/reverseGrammar"+"\" -fv \""+tempDir+"/tempvoca\" -fo \""+tempDir+"/dfaTemp.tmp"+"\" -fh \""+tempDir+"/dfaTemp.h\"";
  if (!execute(execStr, tempDir)) return false;

  execStr = '"'+dfaMinimize+'"'+" \""+tempDir+"/dfaTemp.tmp\" -o \""+tempDir+"model.dfa"+'"';
  return execute(execStr, tempDir);
}


bool ModelCompilerHTK::generateReverseGrammar()
{
  QFile grammar(grammarPath);
  if (!grammar.open(QFile::ReadOnly)) return false;

  QFile reverseGrammar(tempDir+"/reverseGrammar");
  if (!reverseGrammar.open(QFile::WriteOnly)) return false;

  QString reverseGrammarEntry;
  QString grammarEntry;
  QStringList terminals;
  QString identifier;

  int structureCount=0;

  while (!grammar.atEnd()) {
    grammarEntry = QString::fromUtf8(grammar.readLine()).trimmed();
    grammarEntry.remove(QRegExp("\r+$"));
    grammarEntry.remove(QRegExp("#.*"));

    if (grammarEntry.trimmed().isEmpty()) continue;

    //example: "S:NS_B NOM NS_E"
    int splitter =grammarEntry.indexOf(':');
    if (splitter == -1) continue;
    reverseGrammarEntry = grammarEntry.left(splitter+1);
    //reverse = "S:"

    terminals = grammarEntry.mid(splitter+1).split(' ');
    for (int j=terminals.count()-1; j >= 0; j--)
      reverseGrammarEntry += terminals[j]+' ';

    structureCount++;
    // reverse = "S:NS_E NOM NS_B "
    reverseGrammar.write(reverseGrammarEntry.toUtf8()+'\n');
  }
  reverseGrammar.close();
  grammar.close();

  return (structureCount > 0);
}


bool ModelCompilerHTK::generateDict()
{
  int nowId = -1;
  QFile vocab(vocabPath);
  if (!vocab.open(QFile::ReadOnly)) return false;
  QFile dict(tempDir+"model.dict");
  if (!dict.open(QFile::WriteOnly)) return false;
  QString vocabEntry;
  QStringList entryPart;

  while (!vocab.atEnd()) {
    vocabEntry = vocab.readLine(1024);
    vocabEntry.remove(QRegExp("\r+$"));
    vocabEntry.remove(QRegExp("#.*"));
    vocabEntry = vocabEntry.trimmed();
    if (vocabEntry.isEmpty()) continue;

    if (vocabEntry.startsWith('%')) {
      nowId++;
      continue;
    } else
    {
      int splitter = vocabEntry.indexOf('\t');
      if (splitter == -1) continue;

      dict.write(QString(QString::number(nowId)+"\t["+vocabEntry.left(splitter)+"]\t"+vocabEntry.mid(splitter).trimmed()+'\n').toAscii());
    }
  }

  vocab.close();
  dict.close();
  return true;
}

bool ModelCompilerHTK::codeAudioData()
{
  if (!keepGoing) return false;
  emit status(i18n("Coding audio files..."), 150);

  //creating codetrain
  QStringList codeTrainScps;
  if (!generateCodetrainScp(codeTrainScps)) {
    analyseError(i18n("Could not create codetrain-file."));
    return false;
  }

  if (codeTrainScps.isEmpty())
    return true;

  QList<AudioCopyConfig*> configs;
  foreach (const QString& scp, codeTrainScps)
    configs << new AudioCopyConfig(scp, this);

  QList<bool> results = QtConcurrent::blockingMapped(configs, codeAudioDataFromScpHelper);
  
  qDeleteAll(configs);

  return !results.contains(false);
}

bool ModelCompilerHTK::codeAudioDataFromScp(const QString& path)
{
  //QString codetrainPath = tempDir+"/codetrain.scp";
  QString execStr = '"'+hCopy+"\" -A -D -T 1 -C \""+htkIfyPath(wavConfigPath)+"\" -S \""+htkIfyPath(path)+'"';
  if (!execute(execStr, tempDir)) {
    analyseError(i18n("Error while coding the samples!\n\nPlease check the path to HCopy (%1) and the wav config (%2)", hCopy, wavConfigPath));
    return false;
  }
  return true;
}

bool ModelCompilerHTK::reestimate(const QString& command)
{
  return execute(command, tempDir);
}

bool ModelCompilerHTK::generateCodetrainScp(QStringList &codeTrainScps)
{
  int samples = 0;
  QString trainPath = tempDir+"/train.scp";

  QFile promptsFile(promptsPath);
  if (!promptsFile.open(QIODevice::ReadOnly))
    return false;

  QString pathToMFCs =tempDir+"/mfcs";
  QDir().mkpath(pathToMFCs);

  QString codetrainPath = tempDir+"/codetrain.scp";

  QFile trainScpFile(trainPath);
  QFile scpFile(codetrainPath);
  if ((!scpFile.open(QIODevice::WriteOnly|QIODevice::Truncate))|| (!trainScpFile.open(QIODevice::WriteOnly|QIODevice::Truncate)))
    return false;

  QString fileBase;
  QString mfcFile;

  while (!promptsFile.atEnd()) {
    QString line = QString::fromUtf8(promptsFile.readLine());

    fileBase =  line.left(line.indexOf(' '));
    if (fileBase.contains("/"))
    {
      QString subDir = fileBase.left(fileBase.lastIndexOf("/"));
      QDir d(pathToMFCs+'/'+subDir);
      if (!d.exists() && !d.mkpath(subDir))
        return false;
    }
    mfcFile = htkIfyPath(pathToMFCs)+'/'+fileBase+".mfc";
    QString wavFile = htkIfyPath(samplePath)+'/'+fileBase+".wav";

    QFileInfo wavInfo(wavFile);
    if (wavInfo.size() < MIN_WAV_FILESIZE)
      continue;

    #ifdef HTK_UNICODE
    trainScpFile.write(mfcFile.toUtf8()+'\n');
    #else
    trainScpFile.write(mfcFile.toLocal8Bit()+'\n');
    #endif

    if (QFile::exists(mfcFile))
    {
      kDebug() << "MFC already exists: " << mfcFile;
      continue;
    }

    scpFile.write(QString('"'+wavFile+ "\" \"" +mfcFile+"\"\n").toLocal8Bit());
    samples++;
  }
  promptsFile.close();
  scpFile.close();
  trainScpFile.close();
  
  return splitScp(codetrainPath, tempDir, "codetrain", codeTrainScps);
}

bool ModelCompilerHTK::splitScp(const QString& scpIn, const QString& outputDirectory, const QString& fileNamePrefix, QStringList& scpFiles)
{
  QList<QFile*> scpFilesF;
  int threadCount = QThread::idealThreadCount();
  for (int i=0; i < threadCount; i++)
  {
    QString thisPath = outputDirectory+fileNamePrefix+QString::number(i)+".scp";
    QFile *f = new QFile(thisPath);
    if (!f->open(QIODevice::WriteOnly))
    {
      qDeleteAll(scpFilesF);
      return false;
    }
    scpFilesF << f;
    scpFiles << thisPath;
  }

  QFile f(scpIn);
  if (!f.open(QIODevice::ReadOnly))
    return false;
  
  int currentLine = 0;
  while (!f.atEnd())
  {
    scpFilesF[currentLine % threadCount]->write(f.readLine());
    ++currentLine;
  }

  //remove files without content
  kDebug() << currentLine << threadCount;
  kDebug() << scpFiles;
  for (int i = currentLine; i < threadCount; i++)
    scpFiles.removeAt(currentLine);
  kDebug() << scpFiles;

  qDeleteAll(scpFilesF);
  return true;
}

bool ModelCompilerHTK::generateInputFiles()
{
  if (!keepGoing) return false;
  emit status(i18n("Generating wordlist..."), 35);

  //wlist
  if (!generateWlist()) {
    analyseError(i18n("Failed to create wordlist. Please check the path to your prompts file."));
    return false;
  }

  //monophones
  if (!keepGoing) return false;
  emit status(i18n("Creating monophones..."), 40);

  if (!makeMonophones()) {
    analyseError(i18n("Failed to generate monophones. Please check that you have installed the program HDMan correctly and that the lexicon contains all words referenced by your training corpus."));
    return false;
  }

  return true;
}

QString ModelCompilerHTK::getScriptFile(const QString& id)
{
  return KStandardDirs::locate("data", KStandardDirs::locate("data", scriptBasePrefix+'/'+id));
}

QStringList ModelCompilerHTK::getScriptFiles(const QString& id)
{
  return KGlobal::dirs()->findAllResources("data", 
      scriptBasePrefix+'/'+id, KStandardDirs::NoDuplicates);
}

bool ModelCompilerHTK::makeTranscriptions()
{
  //mlf
  if (!keepGoing) return false;
  emit status(i18n("Generating Master Label File..."), 55);
  if (!generateMlf()) {
    analyseError(i18n("Could not generate Master Label File. Please check your prompts file (%1).", promptsPath));
    return false;
  }

  if (!execute('"'+hLEd+"\" -A -D -T 1 -l \"*\" -d \""+htkIfyPath(tempDir)+"/dict\" -i \""+htkIfyPath(tempDir)+"/phones0.mlf\" \""+htkIfyPath(getScriptFile("mkphones0.led"))+"\" \""+htkIfyPath(tempDir)+"/words.mlf"+"\"", tempDir) || !execute('"'+hLEd+"\" -A -D -T 1 -l \"*\" -d \""+htkIfyPath(tempDir)+"/dict"+"\" -i \""+htkIfyPath(tempDir)+"/phones1.mlf\" \""+htkIfyPath(getScriptFile("mkphones1.led"))+"\" \""+htkIfyPath(tempDir)+"/words.mlf\"", tempDir)) {
    analyseError(i18n("Generation of the transcription failed. Please check if you have correctly specified the paths to mkphones0.led and mkphons1.led. (%1, %2)", getScriptFile("mkphones0.led"), getScriptFile("mkphones1.led")));
    return false;
  }
  return true;
}


bool ModelCompilerHTK::createMonophones()
{
  if (!keepGoing) return false;
  emit status(i18n("Generating hmm0..."), 550);
  if (!buildHMM0()) {
    analyseError(i18n("Error when generating the HMM0.\n\nPlease check if there is enough training material and that the path to HCompV (%1), the config (%2) and the phoneme prototype (%3) are correct.", hCompV, getScriptFile("config"), getScriptFile("proto")));
    return false;
  }
  if (!keepGoing) return false;
  emit status(i18n("Generating hmm1..."), 800);
  if (!buildHMM1()) {
    analyseError(i18n("Could not generate the HMM1.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, getScriptFile("config")));
    return false;
  }
  if (!keepGoing) return false;
  emit status(i18n("Generating hmm2..."), 850);
  if (!buildHMM2()) {
    analyseError(i18n("Could not generate the HMM2.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, getScriptFile("config")));
    return false;
  }
  if (!keepGoing) return false;
  emit status(i18n("Generating hmm3..."), 900);
  if (!buildHMM3()) {
    analyseError(i18n("Could not generate the HMM3.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, getScriptFile("config")));
    return false;
  }
  return true;
}


bool ModelCompilerHTK::fixSilenceModel()
{
  if (!keepGoing) return false;
  emit status(i18n("Generating Pause-Model (hmm4)..."), 950);
  if (!buildHMM4()) {
    analyseError(i18n("Could not generate HMM4.\n\nPlease check the HMM3."));
    return false;
  }
  if (!keepGoing) return false;
  emit status(i18n("Generating hmm5..."), 1000);
  if (!buildHMM5()) {
    analyseError(i18n("Could not generate the HMM5.\n\nPlease check the paths to HHEd (%1) and to the silence-model (%2).", hHEd, getScriptFile("sil.hed")));
    return false;
  }
  if (!keepGoing) return false;
  emit status(i18n("Generating hmm6..."), 1080);
  if (!buildHMM6()) {
    analyseError(i18n("Could not generate the HMM6.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, getScriptFile("config")));
    return false;
  }
  if (!keepGoing) return false;
  emit status(i18n("Generating hmm7..."), 1150);
  if (!buildHMM7()) {
    analyseError(i18n("Could not generate the HMM6.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, getScriptFile("config")));
    return false;
  }

  return true;
}


bool ModelCompilerHTK::realign()
{
  if (!keepGoing) return false;
  emit status(i18n("Generating dict1..."), 1160);
  if (!makeDict1()) {
    analyseError(i18n("Could not generate the dict1"));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Re-aligning hmm7..."), 1160);
  if (!realignHMM7()) {
    analyseError(i18n("Could not re-align hmm7. Please check your paths to HVite (%1), the config (%2) and to the HMM7.", hVite, getScriptFile("config")));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating hmm8..."), 1230);
  if (!buildHMM8()) {
    analyseError(i18n("Could not generate the HMM8.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, getScriptFile("config")));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating hmm9..."),1300);
  if (!buildHMM9()) {
    analyseError(i18n("Could not generate the HMM9.\n\nPlease check the path to HERest (%1) and to the config (%2)", hERest, getScriptFile("config")));
    return false;
  }

  return true;
}


bool ModelCompilerHTK::tieStates()
{
  if (!keepGoing) return false;
  emit status(i18n("Generating triphone..."), 1700);

  //start watching triphones
  catchUndefiniedPhonemes = true;

  do
  {
    undefinedPhoneme = QByteArray();
    if (!execute('"'+hDMan+"\" -A -D -T 1 -b sp -n \""+htkIfyPath(tempDir)+"/fulllist" +"\" -g \""+htkIfyPath(getScriptFile("global.ded"))+"\" \""+htkIfyPath(tempDir)+"/dict-tri\" \""+htkIfyPath(tempDir)+"/lexicon\"", tempDir)) {
      analyseError(i18n("Could not bind triphones.\n\nPlease check the paths to HDMan (%1), global.ded (%2) and to the lexicon (%3).", hDMan, getScriptFile("global.ded"), lexiconPath));
      return false;
    }
  
    if (!keepGoing) return false;
    emit status(i18n("Generating list of triphones..."),1705);
    if (!makeFulllist()) {
      analyseError(i18n("Could not generate list of triphones."));
      return false;
    }
    if (!keepGoing) return false;
    emit status(i18n("Generating tree.hed..."), 1750);
    if (!makeTreeHed()) {
      analyseError(i18n("Could not generate tree.hed."));
      return false;
    }

    if (!keepGoing) return false;
    emit status(i18n("Generating hmm13..."),1830);
    if (!buildHMM13()) {
      analyseError(i18n("Could not generate HMM13.\n\nPlease check the path to HHEd (%1).", hHEd));
      if (!undefinedPhoneme.isEmpty())
      {
        if (!removePhoneme(undefinedPhoneme))
        {
          catchUndefiniedPhonemes = false;
          analyseError(i18n("Failed to remove undefined phoneme."));
          return false;
        }
      }
      else
        return false;
    }
  }
  while (!undefinedPhoneme.isEmpty());

  catchUndefiniedPhonemes = false;

  if (!keepGoing) return false;
  emit status(i18n("Generating hmm14..."),1900);
  if (!buildHMM14()) {
    analyseError(i18n("Could not generate the HMM14.\n\nPlease check the paths to HERest (%1), the config (%2) and to the stats-file (%3).", hERest, getScriptFile("config"), tempDir+"/stats"));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating hmm15..."),1990);
  if (!buildHMM15()) {
    analyseError(i18n("Could not generate the HMM15.\n\nPlease check the path to HERest (%1), to the config (%2) and to the stats-file (%3).", hERest, getScriptFile("config"), tempDir+"/stats"));
    return false;
  }

  return true;
}


bool ModelCompilerHTK::buildHMM13()
{
  return execute('"'+hHEd+"\" -A -D -T 1 -H \""+htkIfyPath(tempDir)+"/hmm12/macros\" -H \""+htkIfyPath(tempDir)+"/hmm12/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm13/\" \""+htkIfyPath(tempDir)+"/tree.hed\" \""+htkIfyPath(tempDir)+"/triphones1\"", tempDir);
}


bool ModelCompilerHTK::buildHMM14()
{
  return reestimate(htkIfyPath(tempDir)+"/wintri.mlf", true, htkIfyPath(tempDir)+"/aligned.scp", createHMMPath(13)+"macros", 
      createHMMPath(13)+"hmmdefs", createHMMPath(14), htkIfyPath(tempDir)+"/tiedlist");
}


bool ModelCompilerHTK::buildHMM15()
{
  return reestimate(htkIfyPath(tempDir)+"/wintri.mlf", true, htkIfyPath(tempDir)+"/aligned.scp", createHMMPath(14)+"macros", 
      createHMMPath(14)+"hmmdefs", createHMMPath(15), htkIfyPath(tempDir)+"/tiedlist");
}

QStringList ModelCompilerHTK::getMixtureConfigs()
{
  QStringList mixtureConfigs = getScriptFiles("gmm*.hed");
  //sorting mixtureConfigs
  QStringList sortedConfigs;
  QStringList thisConfigs;
  QString lastPath;
  foreach (const QString& config, mixtureConfigs)
  {
    QFileInfo fi(config);
    QDir configDir = fi.absoluteDir();
    thisConfigs << config;
    if (!lastPath.isEmpty() && (lastPath != configDir.absolutePath()))
    {
      thisConfigs.sort();
      sortedConfigs << thisConfigs;
    }
    lastPath = configDir.absolutePath();
  }
  thisConfigs.sort();
  sortedConfigs << thisConfigs;
  return sortedConfigs;
}

bool ModelCompilerHTK::shouldIncreaseMixtures()
{
  QStringList mixtureConfigs = getMixtureConfigs();
  foreach (const QString& config, mixtureConfigs)
    if (doesIncreaseMixtures(config))
      return true;
  return false;
}

bool ModelCompilerHTK::doesIncreaseMixtures(const QString& script)
{
  QFile f(script);
  if (!f.open(QIODevice::ReadOnly)) return false;

  while (!f.atEnd())
  {
    QByteArray line = f.readLine().trimmed();
    if (!line.startsWith("MU ")) continue; // krazy:exclude=strings
    line.remove(0, 2);
    line = line.trimmed();
    QByteArray number = line.left(line.indexOf(" "));

    bool ok = true;
    int newCount = number.toInt(&ok);
    if (!ok) return false;
    kDebug() << number << ok << newCount << script;

    return ok && (newCount > 0);
  }
  return false;
}

QString ModelCompilerHTK::createHMMPath(int number)
{
  return htkIfyPath(tempDir)+"/hmm"+QString::number(number)+'/';
}

bool ModelCompilerHTK::increaseMixtures()
{
  if (!keepGoing) return false;

  //if we don't want to increase mixtures, the mixture count in the gmm files will be 0;
  //If it is, skip this

  QFile::remove(tempDir+"/hmmout/macros");
  QFile::remove(tempDir+"/hmmout/hmmdefs");
  if (!shouldIncreaseMixtures())
    //copy hmm15 to hmmout
    return QFile::copy(tempDir+"/hmm15/macros", tempDir+"/hmmout/macros") && 
            QFile::copy(tempDir+"/hmm15/hmmdefs", tempDir+"/hmmout/hmmdefs");

  //emit status(i18n("Increasing mixtures..."),2000);

  QStringList mixtureConfigs = getMixtureConfigs();
  int currentState = 2000;
  int steps = mixtureConfigs.count()*3;
  int progressPerStep = qRound(500.0 /* full range */ / ((float) steps));

  int currentHMMNumber = 15;
  kDebug() << "Increasing mixtures: " << mixtureConfigs;
  for (int i=0; i < mixtureConfigs.count(); i++)
  {
    QString config = mixtureConfigs[i];

    bool succ = true;
    emit status(i18n("Increasing mixtures..."),currentState);
    succ = execute('"'+hHEd+"\" -A -D -T 1 -H \""+createHMMPath(currentHMMNumber)+"macros\" -H \""+createHMMPath(currentHMMNumber)+
        "hmmdefs\" -M \""+createHMMPath(currentHMMNumber+1)+"\" \""+htkIfyPath(config)+"\" \""+htkIfyPath(tempDir)+"/tiedlist\"", tempDir);
    currentState += progressPerStep;
    currentHMMNumber += 1;

    emit status(i18n("Re-estimation (1/2)..."),currentState);

    succ = succ && reestimate(htkIfyPath(tempDir)+"/wintri.mlf", true, htkIfyPath(tempDir)+"/aligned.scp", createHMMPath(currentHMMNumber)+"macros", 
      createHMMPath(currentHMMNumber)+"hmmdefs", createHMMPath(currentHMMNumber+1), htkIfyPath(tempDir)+"/tiedlist");

    currentState += progressPerStep;
    currentHMMNumber += 1;

    emit status(i18n("Re-estimation (2/2)..."),currentState);
    QString outputPath;
    succ = succ && reestimate(htkIfyPath(tempDir)+"/wintri.mlf", true, htkIfyPath(tempDir)+"/aligned.scp", createHMMPath(currentHMMNumber)+"macros", 
      createHMMPath(currentHMMNumber)+"hmmdefs", createHMMPath(currentHMMNumber+1), htkIfyPath(tempDir)+"/tiedlist");
    currentState += progressPerStep;
    currentHMMNumber += 1;

    if (!succ)
    {
      analyseError(i18n("Could not generate increase mixtures according to this config: %1.\n\n"
            "Please check the path to HHEd (%2), HERest (%3) and the content of the config file.", config, hHEd, hERest));
      return false;
    }
  }
  emit status(i18n("Done increasing mixtures."),currentState);

  return QFile::copy(createHMMPath(currentHMMNumber)+"macros", tempDir+"/hmmout/macros") && 
            QFile::copy(createHMMPath(currentHMMNumber)+"hmmdefs", tempDir+"/hmmout/hmmdefs");
}


bool ModelCompilerHTK::makeFulllist()
{
  QFile::remove(tempDir+"/fulllist-original");
  if (!QFile::copy(tempDir+"/fulllist", tempDir+"/fulllist-original")) return false;

  QFile triphones1(tempDir+"/triphones1");
  QFile fulllist(tempDir+"/fulllist");

  //copy the triphones from triphones1 to fulllist
  //BUT: omit duplicates!

  QStringList fulllistEntries;
  if (!fulllist.open(QIODevice::ReadWrite)) return false;
  while (!fulllist.atEnd()) fulllistEntries << fulllist.readLine(150);

  if (!triphones1.open(QIODevice::ReadOnly)) return false;

  while (!triphones1.atEnd()) {
    QByteArray triphone = triphones1.readLine(500);
    if (!fulllistEntries.contains(triphone))
      fulllist.write(triphone);
  }
  triphones1.close();

  return true;
}


bool ModelCompilerHTK::makeTreeHed()
{
  QFile::remove(tempDir+"/tree.hed");

  QFile tree1Hed(treeHedPath);
  QFile treeHed(tempDir+"/tree.hed");

  if ((!treeHed.open(QIODevice::WriteOnly)) ||
    (!tree1Hed.open(QIODevice::ReadOnly))) return false;

  //HTK uses Latin1 instead of UTF-8 :/
  treeHed.write("RO 100 \""+tempDir.toUtf8()+"/stats\"\n");

  //copy tree1.hed content to tree.hed
  treeHed.write(tree1Hed.readAll());

  tree1Hed.close();

  QString command = "TB";
  int threshold = 350;
  QFile hmmlist(tempDir+"/monophones0");
  if (!hmmlist.open(QIODevice::ReadOnly)) return false;

  QStringList phonemeList;
  while (!hmmlist.atEnd())
    { phonemeList << hmmlist.readLine().trimmed(); }
    hmmlist.close();

  for (int i=0; i < phonemeList.count(); i++)
    treeHed.write(QString("%1 %2 \"ST_%3_2_\" {(\"%3\",\"*-%3+*\",\"%3+*\",\"*-%3\").state[2]}\n").arg(command).arg(threshold).arg(phonemeList[i]).toUtf8());

  for (int i=0; i < phonemeList.count(); i++)
    treeHed.write(QString("%1 %2 \"ST_%3_3_\" {(\"%3\",\"*-%3+*\",\"%3+*\",\"*-%3\").state[3]}\n").arg(command).arg(threshold).arg(phonemeList[i]).toUtf8());

  for (int i=0; i < phonemeList.count(); i++)
    treeHed.write(QString("%1 %2 \"ST_%3_4_\" {(\"%3\",\"*-%3+*\",\"%3+*\",\"*-%3\").state[4]}\n").arg(command).arg(threshold).arg(phonemeList[i]).toUtf8());

  treeHed.write(QString(" \nTR 1\n \nAU "+htkIfyPath(tempDir)+"/fulllist \nCO "+htkIfyPath(tempDir)+"/tiedlist \n \nST "+htkIfyPath(tempDir)+"/trees \n").toUtf8());

  treeHed.close();

  return true;
}


bool ModelCompilerHTK::buildHMM()
{
  if (!createMonophones()) return false;
  if (!fixSilenceModel()) return false;
  if (!realign()) return false;
  if (!makeTriphones()) return false;
  if (!tieStates()) return false;
  if (!increaseMixtures()) return false;

  return true;
}


bool ModelCompilerHTK::makeTriphones()
{
  if (!keepGoing) return false;
  emit status(i18n("Generating triphone..."),1380);
  if (!execute('"'+hLEd+"\" -A -D -T 1 -n \""+htkIfyPath(tempDir)+"/triphones1\" -l * -i \""+htkIfyPath(tempDir)+"/wintri.mlf\" \""+htkIfyPath(getScriptFile("mktri.led"))+"\" \""+htkIfyPath(tempDir)+"/aligned.mlf\"", tempDir)) {
    analyseError(i18n("Could not generate triphones.\n\nPlease check your path to the files mktri.led and HLEd (%1, %2)", getScriptFile("mktri.led"), hLEd));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating mktri.hed..."),1400);
  if (!makeMkTriHed()) {
    analyseError(i18n("Could not generate mktri.hed"));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating hmm10..."),1470);
  if (!buildHMM10()) {
    analyseError(i18n("Could not generate the HMM10.\n\nPlease check the path to HHEd (%1).", hHEd));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating hmm11..."),1550);
  if (!buildHMM11()) {
    analyseError(i18n("Could not generate the HMM11.\n\nPlease check your paths to HERest (%1) and to the config (%2).", hERest, getScriptFile("config")));
    return false;
  }

  if (!keepGoing) return false;
  emit status(i18n("Generating hmm12..."),1620);
  if (!buildHMM12()) {
    analyseError(i18n("Could not generate the HMM12.\n\nPlease check the path to HERest (%1), to the config (%2) and to the stats-file (%3).", hERest, getScriptFile("config"), tempDir+"/stats"));
    return false;
  }

  return true;
}


bool ModelCompilerHTK::buildHMM12()
{
  return reestimate(htkIfyPath(tempDir)+"/wintri.mlf", true, htkIfyPath(tempDir)+"/aligned.scp", createHMMPath(11)+"macros", 
      createHMMPath(11)+"hmmdefs", createHMMPath(12), htkIfyPath(tempDir)+"/triphones1");
}


bool ModelCompilerHTK::buildHMM11()
{
  return reestimate(htkIfyPath(tempDir)+"/wintri.mlf", true, htkIfyPath(tempDir)+"/aligned.scp", createHMMPath(10)+"macros", 
      createHMMPath(10)+"hmmdefs", createHMMPath(11), htkIfyPath(tempDir)+"/triphones1");
}


bool ModelCompilerHTK::buildHMM10()
{
  return execute('"'+hHEd+"\" -A -D -T 1 -H \""+htkIfyPath(tempDir)+"/hmm9/macros\" -H \""+htkIfyPath(tempDir)+"/hmm9/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm10/\" \""+htkIfyPath(tempDir)+"/mktri.hed\" \""+htkIfyPath(tempDir)+"/monophones1\"", tempDir);
}


bool ModelCompilerHTK::makeMkTriHed()
{
  QFile monophones1(tempDir+"/monophones1");
  if (!monophones1.open(QIODevice::ReadOnly))
    return false;

  QFile mktriHed(tempDir+"/mktri.hed");
  if (!mktriHed.open(QIODevice::WriteOnly)) return false;

  mktriHed.write("CL "+htkIfyPath(tempDir).toUtf8()+"/triphones1\n");
  QByteArray phone="";
  while (!monophones1.atEnd()) {
    phone = monophones1.readLine(150).trimmed();
    mktriHed.write("TI T_"+phone+" {(*-"+phone+"+*,"+phone+"+*,*-"+phone+").transP}\n");
  }

  monophones1.close();
  mktriHed.close();
  return true;
}


bool ModelCompilerHTK::buildHMM9()
{
  return reestimate(htkIfyPath(tempDir)+"/aligned.mlf", false, htkIfyPath(tempDir)+"/aligned.scp", createHMMPath(8)+"macros", 
      createHMMPath(8)+"hmmdefs", createHMMPath(9), htkIfyPath(tempDir)+"/monophones1");
}


bool ModelCompilerHTK::buildHMM8()
{
  return reestimate(htkIfyPath(tempDir)+"/aligned.mlf", false, htkIfyPath(tempDir)+"/aligned.scp", createHMMPath(7)+"macros", 
      createHMMPath(7)+"hmmdefs", createHMMPath(8), htkIfyPath(tempDir)+"/monophones1");
}


bool ModelCompilerHTK::pruneScp(const QString& inMlf, const QString& inScp, const QString& outScp)
{
  //create aligned scp only containing files transcribed in the aligned.mlf file
  QFile alignedMlf(inMlf);
  if (!alignedMlf.open(QIODevice::ReadOnly))
    return false;

  QList<QByteArray> transcribedFiles;
  while (!alignedMlf.atEnd()) {
    QByteArray line = alignedMlf.readLine();      //ascii; no conversion
    if (!line.startsWith(QByteArray("\"*/")))
      continue;

    line = line.trimmed();
    transcribedFiles << line.mid(3, line.count()-8);
  }

  QFile trainScp(inScp);
  if (!trainScp.open(QIODevice::ReadOnly))
    return false;
  QFile alignedScp(outScp);
  if (!alignedScp.open(QIODevice::WriteOnly))
    return false;
  while (!trainScp.atEnd()) {
    QByteArray originalLine = trainScp.readLine();
    QByteArray line = originalLine.trimmed().mid(htkIfyPath(tempDir).count() + 1 /* separator*/ + 5 /* mfcs/ */);
    line = line.left(line.count() - 4 /* .mfc */);
    line = line.mid(line.lastIndexOf("/")+1);

    if (transcribedFiles.contains(line))
      alignedScp.write(originalLine);
    else
    {
      kDebug() << "Transcribed files: " << transcribedFiles;
      kDebug() << "Error decoding " << line << "; You might want to increase the beam width?";
    }

  }
  return true;
}


bool ModelCompilerHTK::realignHMM7()
{
  if (!execute('"'+hVite+"\" -A -D -T 1 -l \"*\"  -o SWT -b silence -C \""+htkIfyPath(getScriptFile("config"))+"\" -H \""+htkIfyPath(tempDir)+"/hmm7/macros\" -H \""+htkIfyPath(tempDir)+"/hmm7/hmmdefs\" -i \""+htkIfyPath(tempDir)+"/aligned.mlf\" -m -t 250.0 150.0 1000.0 -y lab -a -I \""+htkIfyPath(tempDir)+"/words.mlf\" -S \""+htkIfyPath(tempDir)+"/train.scp\" \""+htkIfyPath(tempDir)+"/dict1\" \""+htkIfyPath(tempDir)+"/monophones1\"", tempDir))
    return false;

  return pruneScp(tempDir+QDir::separator()+"aligned.mlf", tempDir+QDir::separator()+"train.scp", tempDir+QDir::separator()+"aligned.scp");
}


bool ModelCompilerHTK::makeDict1()
{
  QFile::remove(tempDir+"/dict1");
  if (!QFile::copy(tempDir+"/dict", tempDir+"/dict1")) return false;
  QFile dict1(tempDir+"/dict1");
  if (!dict1.open(QIODevice::WriteOnly|QIODevice::Append)) return false;

  dict1.write("silence  []  sil\n");
  dict1.close();
  return true;
}


bool ModelCompilerHTK::buildHMM7()
{
  return reestimate(htkIfyPath(tempDir)+"/phones1.mlf", false, htkIfyPath(tempDir)+"/train.scp", createHMMPath(6)+"macros", 
      createHMMPath(6)+"hmmdefs", createHMMPath(7), htkIfyPath(tempDir)+"/monophones1");
}


bool ModelCompilerHTK::buildHMM6()
{
  return reestimate(htkIfyPath(tempDir)+"/phones1.mlf", false, htkIfyPath(tempDir)+"/train.scp", createHMMPath(5)+"macros", 
      createHMMPath(5)+"hmmdefs", createHMMPath(6), htkIfyPath(tempDir)+"/monophones1");
}


bool ModelCompilerHTK::buildHMM5()
{
  return execute('"'+hHEd+"\" -A -D -T 1 -H \""+htkIfyPath(tempDir)+"/hmm4/macros\" -H \""+htkIfyPath(tempDir)+"/hmm4/hmmdefs\" -M \""+htkIfyPath(tempDir)+"/hmm5/\" \""+htkIfyPath(getScriptFile("sil.hed"))+"\" \""+htkIfyPath(tempDir)+"/monophones1\"", tempDir);
}


bool ModelCompilerHTK::buildHMM4()
{
  if ((QFile::exists(tempDir+"/hmm4/macros") && !QFile::remove(tempDir+"/hmm4/macros")) ||
    !QFile::copy(tempDir+"/hmm3/macros", tempDir+"/hmm4/macros"))
    return false;

  QStringList  tmp2;

  QFile hmmdefs3(tempDir+"/hmm3/hmmdefs");
  if (!hmmdefs3.open(QIODevice::ReadOnly)) return false;
  QFile hmmdefs4(tempDir+"/hmm4/hmmdefs");
  if (!hmmdefs4.open(QIODevice::WriteOnly)) return false;

  QByteArray line;
  bool withGCONST = false;
  while (!hmmdefs3.atEnd()) {
    line = hmmdefs3.readLine(3000);
    if (line.contains("\"sil\"")) {
      while ((line != "<ENDHMM>\n") && (true /*!hmmdefs3.atEnd()*/)) {
      	if (line.contains("GCONST")) withGCONST = true;
        hmmdefs4.write(line);
        tmp2 << line;
        line = hmmdefs3.readLine(3000);
      }
      hmmdefs4.write(line);
      int copyLine = 0;
      hmmdefs4.write(tmp2[copyLine++].replace("~h \"sil\"", "~h \"sp\"").toUtf8());
      hmmdefs4.write(tmp2[copyLine++].toUtf8());
      hmmdefs4.write(tmp2[copyLine].replace('5', '3').toUtf8());
      copyLine = withGCONST ? 9 : 8;
      hmmdefs4.write(tmp2[copyLine++].replace('3', '2').toUtf8());
      hmmdefs4.write(tmp2[copyLine++].toUtf8());
      hmmdefs4.write(tmp2[copyLine++].toUtf8());
      hmmdefs4.write(tmp2[copyLine++].toUtf8());
      hmmdefs4.write(tmp2[copyLine++].toUtf8());
      if (withGCONST)
	      hmmdefs4.write(tmp2[copyLine++].toUtf8());
      copyLine = withGCONST ? 21 : 18;
      hmmdefs4.write(tmp2[copyLine].replace('5', '3').toUtf8());
      hmmdefs4.write("0.000000e+000 1.000000e+000 0.000000e+000\n");
      hmmdefs4.write("0.000000e+000 0.900000e+000 0.100000e+000\n");
      hmmdefs4.write("0.000000e+000 0.000000e+000 0.000000e+000\n");
    }
    hmmdefs4.write(line);
  }
  return true;
}


bool ModelCompilerHTK::buildHMM3()
{
  return reestimate(htkIfyPath(tempDir)+"/phones0.mlf", false, htkIfyPath(tempDir)+"/train.scp", createHMMPath(2)+"macros", 
      createHMMPath(2)+"hmmdefs", createHMMPath(3), htkIfyPath(tempDir)+"/monophones0");
}


bool ModelCompilerHTK::buildHMM2()
{
  return reestimate(htkIfyPath(tempDir)+"/phones0.mlf", false, htkIfyPath(tempDir)+"/train.scp", createHMMPath(1)+"macros", 
      createHMMPath(1)+"hmmdefs", createHMMPath(2), htkIfyPath(tempDir)+"/monophones0");
}


bool ModelCompilerHTK::buildHMM1()
{
  return reestimate(htkIfyPath(tempDir)+"/phones0.mlf", false, htkIfyPath(tempDir)+"/train.scp", createHMMPath(0)+"macros", 
      createHMMPath(0)+"hmmdefs", createHMMPath(1), htkIfyPath(tempDir)+"/monophones0");
}


bool ModelCompilerHTK::buildHMM0()
{
  if (!execute('"'+hCompV+"\" -A -D -T 1 -C \""+htkIfyPath(getScriptFile("config"))+"\" -f 0.01 -m -S \""+htkIfyPath(tempDir)+"/train.scp\" -M \""+htkIfyPath(tempDir)+"/hmm0/\" \""+htkIfyPath(getScriptFile("proto"))+'"', tempDir))
    return false;

  QString protoBody="";
  QString protoHead="";
  QString line;
  QFile protoFile(tempDir+"/hmm0/proto");
  if (!protoFile.open(QIODevice::ReadOnly)) return false;

  //extracting proto
  QString protoPreamble="";
  while (!protoFile.atEnd()) {
    line = protoFile.readLine(3000);
    if (line.startsWith(QLatin1String("~h"))) { protoHead = line; break; }
    else protoPreamble += line;
  }
  while (!protoFile.atEnd())
    protoBody += protoFile.readLine(3000);
  protoFile.close();

  QStringList monophones;
  QFile monophones0(tempDir+"/monophones0");
  if (!monophones0.open(QIODevice::ReadOnly)) return false;

  while (!monophones0.atEnd())
    monophones.append(monophones0.readLine(50).trimmed());
  monophones0.close();

  Logger::log(i18n("Used Monophones of the model: %1", monophones.join(", ")));

  QFile hmmdefs(tempDir+"/hmm0/hmmdefs");
  if (!hmmdefs.open(QIODevice::WriteOnly)) return false;
  QString phoneHead;
  QString currentHead="";
  for (int i=0; i < monophones.count(); i++) {
    currentHead = protoHead;
    hmmdefs.write(currentHead.replace("proto", monophones[i]).toUtf8());
    hmmdefs.write(protoBody.toUtf8());
  }
  hmmdefs.close();

  //generating macros
  QFile macros(tempDir+"/hmm0/macros");
  if (!macros.open(QIODevice::WriteOnly)) return false;
  macros.write(protoPreamble.toUtf8());

  QFile vFloors(tempDir+"/hmm0/vFloors");
  if (!vFloors.open(QIODevice::ReadOnly)) return false;
  while (!vFloors.atEnd()) macros.write(vFloors.readLine(1000));
  vFloors.close();

  macros.close();

  return true;
}


bool ModelCompilerHTK::makeMonophones()
{
  QString latinLexiconpath = htkIfyPath(tempDir)+"/lexicon";
  if (QFile::exists(latinLexiconpath))
    if (!QFile::remove(latinLexiconpath)) return false;

  #ifndef HTK_UNICODE
  QFile utfLexicon(lexiconPath);

  QFile latinLexicon(latinLexiconpath);
  if (!utfLexicon.open(QIODevice::ReadOnly) || !latinLexicon.open(QIODevice::WriteOnly))
    return false;

  while (!utfLexicon.atEnd()) {
    QByteArray utfByte = utfLexicon.readLine(3000);
    QByteArray latinByte;
    latinByte = (QString::fromUtf8(utfByte)).toLocal8Bit();
    latinLexicon.write(latinByte);
  }

  utfLexicon.close();
  latinLexicon.close();
  #else
  if (!QFile::copy(lexiconPath, latinLexiconpath)) return false;
  #endif

  //make monophones1
  QString execStr = '"'+hDMan+"\" -A -D -T 1 -m -w \""+htkIfyPath(tempDir)+"/wlist\" -g \""+htkIfyPath(getScriptFile("global.ded"))+"\" -n \""+htkIfyPath(tempDir)+"/monophones1\" -i \""+htkIfyPath(tempDir)+"/dict\" \""+htkIfyPath(tempDir)+"/lexicon\"";
  if (!execute(execStr, tempDir)) return false;

  //add sil phoneme if not in there
  QFile monophones1(tempDir+"/monophones1");
  if (!monophones1.open(QIODevice::ReadWrite))
    return false;
  if (!monophones1.readAll().contains("sil"))
    monophones1.write("sil\n");

  monophones1.seek(0);

  //make monophones0
  //ditch the "sp" phoneme
  QFile monophones0(tempDir+"/monophones0");
  if (!monophones0.open(QIODevice::WriteOnly|QIODevice::Truncate))
    return false;

  QString phoneme;
  while (!monophones1.atEnd()) {
    phoneme = monophones1.readLine(50);
    if ((phoneme.trimmed() != "sp") && (!phoneme.trimmed().isEmpty()))
      monophones0.write(phoneme.toUtf8());
  }
  monophones1.close();
  monophones0.close();
  return true;
}


bool ModelCompilerHTK::generateWlist()
{
  QFile promptsFile(promptsPath);
  if (!promptsFile.open(QIODevice::ReadOnly))
    return false;

  QStringList words;
  QStringList lineWords;
  QString line;
  while (!promptsFile.atEnd()) {
    line = QString::fromUtf8(promptsFile.readLine(3000));
    
    lineWords = line.split(QRegExp("( |\n)"), QString::SkipEmptyParts);
    lineWords.removeAt(0);                        //ditch the file-id

    words << lineWords;
  }
  promptsFile.close();

  words << "SENT-END" << "SENT-START";
  words.sort();

  //remove doubles
  int i=1;
  while (i < words.count()) {
    if (words[i] == words[i-1])
      words.removeAt(i);
    else i++;
  }
  QFile wlistFile(tempDir+"/wlist");
  if (!wlistFile.open(QIODevice::WriteOnly))
    return false;
  for (int i=0; i < words.count(); i++) {
    wlistFile.write(words[i].toLocal8Bit()+'\n');
  }
  wlistFile.close();
  return true;
}


bool ModelCompilerHTK::generateMlf()
{
  QFile promptsFile(promptsPath);
  QFile mlf(tempDir+"/words.mlf");

  if (!promptsFile.open(QIODevice::ReadOnly))
    return false;
  if (!mlf.open(QIODevice::WriteOnly))
    return false;

  mlf.write("#!MLF!#\n");
  QStringList lineWords;
  QString line;
  QString fileName;
  while (!promptsFile.atEnd()) {
    line = QString::fromUtf8(promptsFile.readLine(3000));
    if (line.trimmed().isEmpty()) continue;

    lineWords = line.split(QRegExp("( |\n)"), QString::SkipEmptyParts);
                                                  //ditch the file-id
    fileName = lineWords.takeFirst();

    fileName = fileName.mid(fileName.lastIndexOf("/")+1);
    QString labFile = "\"*/"+fileName+".lab\"";
    #ifndef HTK_UNICODE
    mlf.write(labFile.toLocal8Bit()+'\n');
    #else
    mlf.write(labFile.toUtf8()+'\n');
    #endif
    for (int i=0; i < lineWords.count(); i++)
    #ifndef HTK_UNICODE
      mlf.write(lineWords[i].toLocal8Bit()+'\n');
    #else
    mlf.write(lineWords[i].toUtf8()+'\n');
    #endif
    mlf.write(".\n");
  }
  promptsFile.close();
  mlf.close();
  return true;
}


bool ModelCompilerHTK::adaptBaseModel()
{
  if (!keepGoing) return false;
  emit status(i18n("Re-aligning to base model..."), 550);
  if (!realignToBaseModel()) {
    analyseError(i18n("Could not re-align model to the selected base model.\n\nThis error is often caused when you are trying to adapt to a model with a different phoneme set. Ideally you should use the same dictionary as the basemodel.\n\nThis error also occurs if you have no training data to adapt to!"));
    return false;
  }
  emit status(i18n("Generating regtree.hed..."), 900);
  if (!makeRegTreeHed()) {
    analyseError(i18n("Could not create regtree.hed."));
    return false;
  }
  emit status(i18n("Creating regression class tree..."), 950);
  if (!createRegressionClassTree()) {
    analyseError(i18n("Could not create regression class tree. Please check your paths to HHEd (%1).", hHEd));
    return false;
  }
  emit status(i18n("Performing static adaption..."), 1500);
  if (!staticAdaption()) {
    analyseError(i18n("Could not adapt the base model. Please check your paths to HERest (%1).", hERest));
    return false;
  }

  emit status(i18n("Switching to new model..."), 1500);

  QString expectedHmmPath = QFileInfo(baseHmmDefsPath).fileName();
  expectedHmmPath = expectedHmmPath.left(expectedHmmPath.indexOf("."));
  QString hmmDefsPath = tempDir+"hmmout/hmmdefs";
  QString tiedListPath = tempDir+"hmmout/tiedlist";
  if ( (QFile::exists(hmmDefsPath) && (!QFile::remove(hmmDefsPath))) ||
       (!QFile::copy(tempDir+"/xforms/"+expectedHmmPath+".mfc.mllr2", hmmDefsPath)) ||
       (QFile::exists(tiedListPath) && (!QFile::remove(tiedListPath))) ||
       (!QFile::copy(baseTiedlistPath, tiedListPath)) ) {
    analyseError(i18n("Could not switch to new model."));
    return false;
  }

  return true;
}


bool ModelCompilerHTK::realignToBaseModel()
{
  if (!execute('"'+hVite+"\" -A -D -T 1 -l \"*\"  -o SWT -b SENT-END -C \""+htkIfyPath(getScriptFile("config"))+"\" -H \""+htkIfyPath(baseMacrosPath)+"\" -H \""+htkIfyPath(baseHmmDefsPath)+"\" -i \""+htkIfyPath(tempDir)+"/adaptPhones.mlf\" -m -t 250.0 150.0 1000.0 -y lab -a -I \""+htkIfyPath(tempDir)+"/words.mlf\" -S \""+htkIfyPath(tempDir)+"/train.scp\" \""+htkIfyPath(tempDir)+"/dict\" \""+htkIfyPath(baseTiedlistPath)+"\"", tempDir))
    return false;

  return pruneScp(tempDir+QDir::separator()+"adaptPhones.mlf", tempDir+QDir::separator()+"train.scp", tempDir+QDir::separator()+"aligned.scp");
}


bool ModelCompilerHTK::createRegressionClassTree()
{
  return execute('"'+hHEd+"\" -H \""+htkIfyPath(baseMacrosPath)+"\" -H \""+htkIfyPath(baseHmmDefsPath)+"\" -M \""+htkIfyPath(tempDir)+"/classes/\" \""+htkIfyPath(tempDir)+"/regtree.hed\" \""+htkIfyPath(baseTiedlistPath)+"\"", tempDir);
}


bool ModelCompilerHTK::makeRegTreeHed()
{
  QFile regTreeHed(tempDir+"/regtree.hed");
  QFile regTreeHed1(getScriptFile("regtree.hed"));

  if (!regTreeHed.open(QIODevice::WriteOnly) || !regTreeHed1.open(QIODevice::ReadOnly)) return false;

  while (!regTreeHed1.atEnd())
  {
    QByteArray line = regTreeHed1.readLine().replace("$basestats$", baseStatsPath.toUtf8());
    regTreeHed.write(line);
  }

  //RN "models"
  //LS "stats"
  //RC 32 "rtree"

  regTreeHed1.close();
  regTreeHed.close();
  return true;
}

int ModelCompilerHTK::getBaseModelMixtureCount()
{
  QFile f(baseHmmDefsPath);
  if (!f.open(QIODevice::ReadOnly))
    return 1;

  while (!f.atEnd())
  {
    QByteArray l = f.readLine().trimmed();
    if (l.startsWith("<NUMMIXES> ")) // krazy:exclude=strings
      return l.mid(11).toInt();
  }

  return 1;
}

bool ModelCompilerHTK::prepareGlobalConfig()
{
  int baseMixCount = getBaseModelMixtureCount();
  QByteArray textualMixCount;
  if (baseMixCount != 1)
    textualMixCount = QByteArray("1-") + QByteArray::number(baseMixCount);
  else
    textualMixCount = QByteArray::number(baseMixCount);

  QString staticAdaptionPath = tempDir+QDir::separator()+"classes/global";
  QFile globalAdaption(staticAdaptionPath);
  QFile globalAdaption1(getScriptFile("global"));

  if (!globalAdaption.open(QIODevice::WriteOnly) || !globalAdaption1.open(QIODevice::ReadOnly))
    return false;

  while (!globalAdaption1.atEnd())
    globalAdaption.write(globalAdaption1.readLine().replace("$basemixcount$", textualMixCount));

  return true;
}

bool ModelCompilerHTK::staticAdaption()
{
  if (!prepareGlobalConfig()) 
    return false;
  
  //This could possibly be paralellized
  if (!execute('"'+hERest+"\" -C \""+htkIfyPath(getScriptFile("config"))+"\" -C \""+htkIfyPath(getScriptFile("config.global"))+"\" -I \""+htkIfyPath(tempDir)+"/adaptPhones.mlf\" -S \""+htkIfyPath(tempDir)+"/aligned.scp\" -H \""+baseMacrosPath+"\" -u a -J \""+htkIfyPath(tempDir)+"/classes\" -K \""+htkIfyPath(tempDir)+"/xforms\" mllr1 -H \""+baseHmmDefsPath+"\" \""+baseTiedlistPath+"\"", tempDir))
    return false;

  if (!execute('"'+hERest+"\" -a -C \""+htkIfyPath(getScriptFile("config"))+"\" -C \""+htkIfyPath(getScriptFile("config.rc"))+"\" -I \""+htkIfyPath(tempDir)+"/adaptPhones.mlf\" -S \""+htkIfyPath(tempDir)+"/aligned.scp\" -H \""+baseMacrosPath+"\" -u a -J \""+htkIfyPath(tempDir)+"/xforms\" mllr1 -J \""+htkIfyPath(tempDir)+"/classes\" -K \""+htkIfyPath(tempDir)+"/xforms\" mllr2 -H \""+baseHmmDefsPath+"\" \""+baseTiedlistPath+"\"", tempDir))
    return false;


  return true;
}

QString ModelCompilerHTK::information(bool condensed) const
{
  return i18n("HTK: %1\nJulius: %2", htkInformation(condensed), juliusInformation(condensed));
}

QString ModelCompilerHTK::htkInformation(bool condensed) const
{
  QProcess proc;
  KConfig config( KStandardDirs::locateLocal("config", "simonmodelcompilationrc"), KConfig::FullConfig );
  KConfigGroup programGroup(&config, "Programs");
  QString hHEd = programGroup.readEntry("HHEd", KUrl(KStandardDirs::findExe("HHEd"))).toLocalFile();
  proc.start(hHEd, QStringList() << "-V");
  if (!proc.waitForStarted() || !proc.waitForFinished())
    return i18nc("\"version information\" for a not installed HTK", "Not available");

  QString information = proc.readAllStandardOutput().trimmed();
  if (condensed)
  {
    QStringList info = information.split('\n');
    info.removeAt(0);
    info.removeAt(0);
    QStringList versions;
    foreach (const QString& line, info)
    {
      QStringList columns = line.split(QRegExp("  +"));
      if (columns.count() == 5)
        if (!versions.contains(columns[1]))
          versions << columns[1];
    }
    return versions.join(", ");
  }

  return information;
}

QString ModelCompilerHTK::juliusInformation(bool condensed) const
{
  Q_UNUSED(condensed);
  return i18nc("refers to using a system Julius instead of an integrated one", "External");
}

bool ModelCompilerHTK::reestimate(const QString& mlf, bool useStats, const QString& scp, 
    const QString& inputMacros, const QString& inputHMMs, const QString& outputDirectory, 
    const QString& phoneList, const QStringList& additionalConfigs, const QString& additionalParameters)
{
#ifdef HEREST_MULTITHREADED
  QStringList scpFiles;
  QStringList commands;
  splitScp(scp, tempDir, "reestimate", scpFiles);
  int channel = 0;
  scpFiles.insert(0, QString());
  foreach (const QString& thisScp, scpFiles)
  {
#else
  QString thisScp = scp;
#endif
  
  QString command = '"'+hERest+"\" -C \""+htkIfyPath(getScriptFile("config"))+"\" ";

  foreach (const QString& c, additionalConfigs)
    command += "-C \""+c+"\" ";

  if (!additionalParameters.isEmpty())
    command += additionalParameters+' ';

  //Pruning threshold (was: 3000.0 / 1000.0)
  command += "-I \""+mlf+"\" -t 250.0 150.0 3000.0 ";
  if (useStats)
    command += "-s \""+htkIfyPath(tempDir)+"/stats\" ";

  if (!thisScp.isEmpty())
    command += "-S \""+thisScp+"\" " ;
  command += "-H \""+inputMacros+"\" -H \""+inputHMMs+"\" ";

  if (!outputDirectory.isEmpty())
    command += "-M \""+outputDirectory+"\" ";
  
#ifdef HEREST_MULTITHREADED
    command += "-p "+QString::number(channel++)+' ';
#endif

  command += '"'+phoneList+'"';

#ifdef HEREST_MULTITHREADED
  commands << command;
  }
//   kDebug() << commands; exit(0);
  QString mergeCmd = commands.takeFirst();
  for (int i=1; i <= commands.count(); i++)
    mergeCmd += " \""+outputDirectory+"HER"+QString::number(i)+".acc\"";
  
  //execute all commands in paralell and merge results
  QList<ReestimationConfig*> reestimationConfigs;
  foreach (const QString& c, commands)
    reestimationConfigs << new ReestimationConfig(c, this);
    
  QList<bool> results = QtConcurrent::blockingMapped(reestimationConfigs, reestimateHelper);
  qDeleteAll(reestimationConfigs);
  if (results.contains(false)) return false;
  
  if (!outputDirectory.isEmpty())
  {
    //merge
    // no -S parameter; add -p 0; add /tmp/kde-bedahr/sam/internalsamuser/compile//hmm1//*.acc
    kDebug() << "Merge command: " << mergeCmd;
    
    return execute(mergeCmd, tempDir);
  }
  return true;
  
#else
  return execute(command, tempDir);
#endif
}
