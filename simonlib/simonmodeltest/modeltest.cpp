/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include "modeltest.h"
#include "recognizerresult.h"
#include "testresult.h"
#include "fileresultmodel.h"
#include "testresultmodel.h"
#include "testresultleaf.h"
#include "testresultinstance.h"

#include <simonlogging/logger.h>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>

#include <KUrl>
#include <KConfig>
#include <KConfigGroup>
#include <KStandardDirs>
#include <KLocalizedString>
#include <KComponentData>
#include <KAboutData>
#include <KDebug>
#include <KLocale>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <locale.h>

ModelTest::ModelTest(const QString& user_name, QObject* parent) : QThread(parent),
recog(0),
logFile(0),
userName(user_name)
{
  m_recognizerResultsModel = new FileResultModel(this);
  m_wordResultsModel = new TestResultModel(this);
  m_sentenceResultsModel = new TestResultModel(this);
  connect(this, SIGNAL(status(const QString&, int, int)), this, SLOT(addStatusToLog(const QString&)));
  connect(this, SIGNAL(recognitionInfo(const QString&)), this, SLOT(addRecognitionInfoToLog(const QString&)));
}


bool ModelTest::createDirs()
{
  tempDir = KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName()+'/'+userName+"/test/");

  if (tempDir.isEmpty()) return false;

  QFile::remove(tempDir+"julius.log");

  QDir tempDirHandle(tempDir);
  if (!tempDirHandle.exists())
    return false;

  tempDirHandle.mkdir("samples");

  return true;
}


bool ModelTest::parseConfiguration()
{
  KConfig config( KStandardDirs::locateLocal("config", "simonmodeltestrc"), KConfig::FullConfig );
  KConfigGroup programGroup(&config, "Programs");

  sox = programGroup.readEntry("SOX", KUrl(KStandardDirs::findExe("sox"))).toLocalFile();
  julius = programGroup.readEntry("Julius", KUrl(KStandardDirs::findExe("juliusexe"))).toLocalFile();

  if (!QFile::exists(sox) ||
    !QFile::exists(julius)) {
    QString errorMsg = i18n("SOX or Julius can not be found. Please make sure it is installed correctly.\n\n");
    #ifdef Q_OS_WIN32
    errorMsg += i18n("More information: http://www.cyber-byte.at/wiki/index.php/English:_Setup#Windows");
    #else
    errorMsg += i18n("More information: http://www.cyber-byte.at/wiki/index.php/English:_Setup#HTK_Installation");
    #endif
    emitError(errorMsg);
    return false;
  }

  return true;
}


bool ModelTest::execute(const QString& command, const QString& outputFilePath, const QString& errorFilePath)
{
  QProcess proc;
  proc.setWorkingDirectory(QCoreApplication::applicationDirPath());
  proc.start(command);

  buildLog += "<p><span style=\"font-weight:bold; color:#00007f;\">"+command+"</span></p>";
  proc.waitForFinished(-1);

  QByteArray errByte = proc.readAllStandardError();
  QByteArray outByte = proc.readAllStandardOutput();
  QString err = QString::fromLocal8Bit(errByte);
  QString out = QString::fromLocal8Bit(outByte);

  if (!out.isEmpty()) {
    QString htmlOut = out;
    htmlOut.remove("<s>");
    htmlOut.remove("</s>");
    buildLog += "<p>"+htmlOut+"</p>";
  }

  if (!err.isEmpty()) {
    QString htmlErr = err;
    htmlErr.remove("<s>");
    htmlErr.remove("</s>");
    buildLog += "<p><span style=\"color:#aa0000;\">"+htmlErr+"</span></p>";
  }

  if (!outputFilePath.isEmpty()) {
    QFile outFile(outputFilePath);
    if (!outFile.open(QIODevice::WriteOnly))
      return false;

    outFile.write(outByte);
  }

  if (!errorFilePath.isEmpty()) {
    QFile outFile(errorFilePath);
    if (!outFile.open(QIODevice::WriteOnly))
      return false;

    outFile.write(errByte);
  }

  if (proc.exitCode() != 0)
    return false;
  else return true;
}


void ModelTest::addStatusToLog(const QString& status)
{
  buildLog += "<p><span style=\"font-weight:bold; color:#358914;\">"+status+"</span></p>";
}


void ModelTest::addRecognitionInfoToLog(const QString& status)
{
  buildLog += "<p>"+status+"</p>";
}


bool ModelTest::hasLog()
{
  return (buildLog.count() > 0);
}


QString ModelTest::getGraphicLog()
{
  QString htmlLog = buildLog;
  htmlLog=htmlLog.replace('\n', "<br />");
  return "<html><head /><body>"+htmlLog+"</body></html>";
}


QString ModelTest::getLog()
{
  QString plainLog = buildLog;
  plainLog.remove("<p>");
  plainLog.remove("</p>");
  plainLog.remove("<span style=\"color:#aa0000;\">");
  plainLog.remove("<span style=\"font-weight:bold; color:#358914;\">");
  plainLog.remove("<span style=\"font-weight:bold; color:#00007f;\">");
  plainLog.remove("</span>");
  return plainLog;
}


void ModelTest::abort()
{
  if (isRunning()) {
    keepGoing=false;

    if (recog) {
      j_request_terminate(recog);
      j_close_stream(recog);
    }
    //terminate();

    emit testAborted();
  }
}


bool ModelTest::startTest(const QString& hmmDefsPath, const QString& tiedListPath,
const QString& dictPath, const QString& dfaPath,
const QString& samplePath, const QString& promptsPath,
int sampleRate, const QString& juliusJConf)
{
  abort();
  wait();

  this->hmmDefsPath = hmmDefsPath;
  this->tiedListPath = tiedListPath;
  this->dictPath = dictPath;
  this->dfaPath = dfaPath;

  this->samplePath = samplePath;

  this->promptsPath = promptsPath;

  this->sampleRate = sampleRate;
  this->juliusJConf = juliusJConf;

  keepGoing=true;

  buildLog="";

  if (!parseConfiguration())
    return false;

  start();
  return true;
}

void ModelTest::deleteAllResults()
{
  m_wordResultsModel->clear();
  m_sentenceResultsModel->clear();
  m_recognizerResultsModel->clear();

  promptsTable.clear();
  qDeleteAll(resultLeafes);
  qDeleteAll(sentenceResults);
  qDeleteAll(wordResults);
  qDeleteAll(recognizerResults.values());
  recognizerResults.clear();
  wordResults.clear();
  sentenceResults.clear();
  resultLeafes.clear();
}

void ModelTest::run()
{
  if (!createDirs())
    emitError(i18n("Could not generate temporary folders.\n\nPlease check your permissions for \"%1\".", tempDir));

  deleteAllResults();

  if (!keepGoing) return;
  Logger::log(i18n("[INF] Testing model..."));
  emit status(i18n("Preperation"), 0,100);

  if (!recodeAudio()) return;
  if (!generateMLF()) return;
  if (!recognize()) return;
  if (!analyzeResults()) return;

  //if (!keepGoing) return;

  emit status(i18nc("The model test has completed", "Finished"), 100, 100);
  emit testComplete();
}


bool ModelTest::recodeAudio()
{
  emit status(i18n("Recoding audio..."), 5, 100);

  QFile promptsF(promptsPath);
  if (!promptsF.open(QIODevice::ReadOnly)) {
    emitError(i18n("Could not open prompts file for reading: %1", promptsPath));
    return false;
  }

  QFile wavListF(tempDir+"wavlist");
  if (!wavListF.open(QIODevice::WriteOnly)) {
    emitError(i18n("Could not open wavlist file for writing: %1", tempDir+"wavlist"));
    return false;
  }

  QDir d;
  while (!promptsF.atEnd() && keepGoing) {
    QString line = QString::fromUtf8(promptsF.readLine (1024));
    if (line.trimmed().isEmpty()) continue;
    int splitter = line.indexOf(' ');
    QString fileName = line.left(splitter)+".wav";
    QString prompt = line.mid(splitter+1).trimmed();
    QString fullPath = samplePath+QDir::separator()+fileName;
    QString targetPath = tempDir+"samples"+QDir::separator()+fileName;
    QString targetDirectory = targetPath.left(targetPath.lastIndexOf(QDir::separator()));

    if (!d.mkpath(targetDirectory)) {
      kDebug() << "Could not make path: " << targetDirectory;
      continue;
    }

    execute(QString("%1 -2 -s -L %2 %3").arg(sox).arg(fullPath).arg(targetPath));
    wavListF.write(targetPath.toUtf8()+'\n');

    promptsTable.insert(targetPath, prompt);

    recodedSamples.insert(targetPath, fullPath);
  }

  promptsF.close();
  wavListF.close();
  return keepGoing;
}


bool ModelTest::generateMLF()
{
  if (!keepGoing) return false;

  emit status(i18n("Generating MLF..."), 10, 100);
  //echo "Step 1 of 7: Generating MLF"
  //perl `dirname $0`/prompts2mlf testref.mlf prompts

  QFile promptsFile(promptsPath);
  QFile mlf(tempDir+"/testref.mlf");

  if (!promptsFile.open(QIODevice::ReadOnly))
    return false;
  if (!mlf.open(QIODevice::WriteOnly))
    return false;

  mlf.write("#!MLF!#\n");
  QStringList lineWords;
  QString line;
  while (!promptsFile.atEnd()) {
    line = QString::fromUtf8(promptsFile.readLine(3000));
    if (line.trimmed().isEmpty()) continue;
    lineWords = line.split(QRegExp("( |\n)"), QString::SkipEmptyParts);
                                                  //ditch the file-id
    QString labFile = "\"*/"+lineWords.takeAt(0)+".lab\"";
    #ifdef Q_OS_WIN
    mlf.write(labFile.toLatin1()+'\n');
    #else
    mlf.write(labFile.toUtf8()+'\n');
    #endif
    for (int i=0; i < lineWords.count(); i++)
    #ifdef Q_OS_WIN
      mlf.write(lineWords[i].toLatin1()+'\n');
    #else
    mlf.write(lineWords[i].toUtf8()+'\n');
    #endif
    mlf.write(".\n");
  }
  promptsFile.close();
  mlf.close();
  return true;
}

QString getHypoPhoneme(WORD_ID *seq, int n, WORD_INFO *winfo)
{
  QString result;
  WORD_ID w;
  static char buf[MAX_HMMNAME_LEN];

  if (seq != 0) {
    for (int i=0;i<n;i++) {
      if (i > 0) result += " |";
      w = seq[i];
      for (int j=0;j<winfo->wlen[w];j++) {
        center_name(winfo->wseq[w][j]->name, buf);
        result += ' ';
        result += QString::fromUtf8(buf);
      }
    }
  }
  return result;
}

void processRecognitionResult(Recog *recog, void *test)
{
  int i;
  WORD_INFO *winfo;
  WORD_ID *seq;
  int seqnum;
  int n;
  Sentence *s;
  RecogProcess *r;
  ModelTest *modelTest = (ModelTest*) test;
  Q_ASSERT(modelTest);

  /* all recognition results are stored at each recognition process
  instance */
  for(r=recog->process_list;r;r=r->next) {
    bool searchFailed=false;
    /* skip the process if the process is not alive */
    if (! r->live) continue;

    /* result are in r->result.  See recog.h for details */

    /* check result status */
    if (r->result.status < 0) {                   /* no results obtained */
      /* outout message according to the status code */
      switch(r->result.status) {
        case J_RESULT_STATUS_REJECT_POWER:
          printf("<input rejected by power>\n");
          break;
        case J_RESULT_STATUS_TERMINATE:
          printf("<input teminated by request>\n");
          break;
        case J_RESULT_STATUS_ONLY_SILENCE:
          printf("<input rejected by decoder (silence input result)>\n");
          break;
        case J_RESULT_STATUS_REJECT_GMM:
          printf("<input rejected by GMM>\n");
          break;
        case J_RESULT_STATUS_REJECT_SHORT:
          printf("<input rejected by short input>\n");
          break;
        case J_RESULT_STATUS_FAIL:
          printf("<search failed>\n");
          searchFailed=true;
          break;
      }
      /* continue to next process instance */
      continue;
    }

    /* output results for all the obtained sentences */
    winfo = r->lm->winfo;

    QList<RecognitionResult> recognitionResults;

    for(n = 0; n < r->result.sentnum; n++) {      /* for all sentences */
      QString result, sampa, sampaRaw;
      s = &(r->result.sent[n]);
      seq = s->word;
      seqnum = s->word_num;

      /* output word sequence like Julius */
      //       printf("sentence%d:", n+1);
      for(i=0;i<seqnum;i++) {
        result += ' ';
                                                  // printf(" %s", );
        result += QString::fromUtf8(winfo->woutput[seq[i]]);
      }
      result.remove("<s> ");
      result.remove(" </s>");
      sampaRaw = sampa = getHypoPhoneme(seq, seqnum, winfo);

      /* confidence scores */
      //       printf("cmscore%d:", n+1);
      QList<float> confidenceScores;

      for (i=1;i<seqnum-1; i++) {
        confidenceScores << s->confidence[i];
      }

      recognitionResults.append(RecognitionResult(result.trimmed(),
        sampa.trimmed(),
        sampaRaw.trimmed(), confidenceScores));
    }
    if (recognitionResults.isEmpty() && searchFailed)
      modelTest->searchFailed();
    else
      modelTest->recognized(recognitionResults);
  }
}


void ModelTest::emitError(const QString& message)
{
  closeLog();

  QFile f(tempDir+"julius.log");
  if (!f.open(QIODevice::ReadOnly)) {
    kDebug() << "Could not open file";
    emit error(message, QByteArray());
    return;
  }

  kDebug() << "Reading file!";

  QByteArray out = "<html><head /><body><p>"+f.readAll().replace('\n', "<br />")+"</p></body></html>";
  emit error(message, out);
  f.close();
}


bool ModelTest::recognize()
{
  if (!keepGoing) return false;
  emit status(i18n("Recognizing..."), 35, 100);

  QByteArray logPath = tempDir.toUtf8()+"julius.log";

  closeLog();                                     //close if open
  logFile = fopen(logPath.data(), "w");
  if (logFile == 0)
    return false;
  jlog_set_output(logFile);

  if (!QFile::exists(juliusJConf)) {
    emitError(i18n("Could not open julius jconf file: \"%1\".", juliusJConf));
    return false;
  }
  kDebug() << "Using hmm definitions: " << hmmDefsPath;

  //////BEGIN: Workaround
  //convert "." in hmmdefs to its locale specific equivalent
  lconv * localeConv = localeconv();
  char *decimalPoint = localeConv->decimal_point;

  kDebug() << "Source: " << hmmDefsPath;
  QFile hmm(hmmDefsPath);
  QFile hmmLoc(tempDir+"hmmdefs");
  if (!hmm.open(QIODevice::ReadOnly) || !hmmLoc.open(QIODevice::WriteOnly)) {
    emitError(i18n("Could not open hmm definitions"));
    return false;
  }

  while (!hmm.atEnd())
    hmmLoc.write(hmm.readLine(3000).replace('.', decimalPoint));

  hmm.close();
  hmmLoc.close();
  //////END: Workaround

  if (!keepGoing) {
    closeLog();
    return false;
  }

  QByteArray hmmDefs = tempDir.toUtf8()+"hmmdefs";
  int argc=15;

  kDebug() << juliusJConf.toLocal8Bit().data();
  kDebug() << dfaPath.toLocal8Bit().data();
  QByteArray juliusJConfByte = juliusJConf.toLocal8Bit();
  QByteArray dfaPathByte = dfaPath.toLocal8Bit();
  QByteArray dictPathByte = dictPath.toLocal8Bit();
  QByteArray tiedListPathByte = tiedListPath.toLocal8Bit();
  QByteArray tempDirByte = tempDir.toLocal8Bit();
  QByteArray hmmDefsByte = tempDirByte + "hmmdefs";
  QByteArray fileListByte = tempDirByte + "wavlist";
  QByteArray sampleRateByte = QByteArray::number(sampleRate);
  char* argv[] = {
    "sam", "-C", juliusJConfByte.data(),
    "-dfa", dfaPathByte.data(),
    "-v", dictPathByte.data(),
    "-h", hmmDefsByte.data(),
    "-hlist", tiedListPathByte.data(),
    "-input", "rawfile",
    "-filelist", fileListByte.data(),
    "smpFreq", sampleRateByte.data()
  };

  for (int i=0; i < argc; i++)
    kDebug() << argv[i];

  Jconf *jconf = j_config_load_args_new(argc, argv);
  if (!jconf) {
    emitError(i18n("Internal Jconf error"));
    return false;
  }

  recog = j_create_instance_from_jconf(jconf);
  if (!recog) {
    emitError(i18n("Could not initialize recognition"));
    j_jconf_free(jconf);
    this->recog=0;
    return false;
  }

  callback_add(recog, CALLBACK_RESULT, processRecognitionResult, this);

  /**************************/
  /* Initialize audio input */
  /**************************/
  /* initialize audio input device */
  /* ad-in thread starts at this time for microphone */
  if (j_adin_init(recog) == false) {              /* error */
    emitError(i18n("Could not start adin-thread"));
    j_recog_free(recog);
    this->recog=0;
    return false;
  }

  /* output system information to log */
  j_recog_info(recog);

  bool shouldBeRunning = true;
  while (shouldBeRunning && keepGoing) {
    switch(j_open_stream(recog, 0)) {
      case 0:
        //	fprintf(stderr, "j_open_stream returned 0\n");
        //	emit recognitionStarted();
        break;
      case -1:
        //	fprintf(stderr, "j_open_stream returned -1\n");
        break;
        //	emit recognitionError(i18n("Unknown error"));
        //return false;
        //
        //skipping to -2 for freeing...
      case -2:
        //	fprintf(stderr, "j_open_stream returned -2\n");
        shouldBeRunning = false;
        //filelist input somehow returns -2 on finish
        //	emitError(i18n("Could not recognize samples"));
        //	if (isLocal)
        //		emit recognitionError(i18n("Could not initialize microphone"));
        //	else emit recognitionError(i18n("Error with the audio stream"));
        break;
    }

    if (!shouldBeRunning)
      break;

    /**********************/
    /* Recognization Loop */
    /**********************/
    /* enter main loop to recognize the input stream */
    /* finish after whole input has been processed and input reaches end */
    int ret = j_recognize_stream(recog);
    switch (ret) {
      case 0:
        //client exited
        //shouldBeRunning=false;
        break;
      case -1:
        //emit recognitionError("recognize_stream: -1");
        shouldBeRunning=false;
        break;
    }
    //usleep(300);
  }

  j_recog_free(recog);
  recog = 0;
  closeLog();

  return keepGoing;
}


void ModelTest::closeLog()
{
  if (!logFile) return;

  jlog_flush();
  fclose(logFile);
  logFile = 0;
}


void ModelTest::searchFailed()
{
  QString fileName = QString::fromUtf8(QByteArray(j_get_current_filename(recog)));

  emit recognitionInfo(i18n("Search failed for: %1", fileName));

  QString prompt = promptsTable.value(fileName);
  QStringList promptWordList = prompt.split(' ');

  TestResult *sentenceResult = getResult(sentenceResults, prompt);
  QList<TestResultLeaf*> sentenceLeafes;

  foreach (const QString& label, promptWordList)
  {
    TestResultLeaf *dummyLeaf = new TestResultLeaf(label, "", 0.0);
    dummyLeaf->setDeletionError(true);
    resultLeafes << dummyLeaf;

    sentenceLeafes << dummyLeaf;

    TestResult *wordResult = getResult(wordResults, label);
    if (!wordResult->registerChild(dummyLeaf))
      kWarning() << "Failed to process dummy word result";
  }

  if (!sentenceResult->registerChildren(sentenceLeafes))
    kWarning() << "Could not process dummy sentence result";
  
  recognizerResults.insert(fileName, new RecognizerResult(prompt, RecognitionResultList()));
}

/**
 * Finds the result in the given list or creates a new one and adds that to the 
 * list if it doesn't yet exist
 */
TestResult* ModelTest::getResult(QList<TestResult*>& list, const QString& prompt)
{
  foreach (TestResult* result, list)
    if (result->matchesLabel(prompt))
      return result;

  TestResult *result = new TestResult(prompt);
  list << result;
  return result;
}

void ModelTest::recognized(RecognitionResultList results)
{
  QString fileName = QString::fromUtf8(QByteArray(j_get_current_filename(recog)));

  QString prompt = promptsTable.value(fileName);
  recognizerResults.insert(fileName, new RecognizerResult(prompt, results));

  emit recognitionInfo(i18n("Prompts entry: %1", prompt));
  if (results.count() > 0)
  {
    emit recognitionInfo(i18n("Received recognition result for: %1: %2", fileName, results.at(0).sentence()));

    RecognitionResult& highestRatedResult = results.first();

    TestResult *sentenceResult = getResult(sentenceResults, prompt);

    QList<TestResultLeaf*> leafs = TestResultInstance::parseResult(highestRatedResult);
    resultLeafes << leafs;

    if (!sentenceResult->registerChildren(leafs))
      kWarning() << "Could not process sentence result";

    foreach (TestResultLeaf* leaf, leafs)
    {
      TestResult *wordResult = getResult(wordResults, leaf->originalLabel());
      if (!wordResult->registerChild(leaf))
        kWarning() << "Could not process word result";
    }
  }
}

FileResultModel* ModelTest::recognizerResultsModel()
{
  return m_recognizerResultsModel;
}

float ModelTest::getOverallConfidence()
{
  float overallConfidence = 0;
  foreach (TestResultLeaf *leaf, resultLeafes)
    if (leaf->correct())
      overallConfidence += leaf->confidence();

  overallConfidence /= (float) resultLeafes.count();
  return overallConfidence;
}

float ModelTest::getOverallAccuracy()
{
  float overallAccuracy = 0;
  foreach (TestResultLeaf *leaf, resultLeafes)
    overallAccuracy += leaf->accuracy();
  overallAccuracy /= (float) resultLeafes.count();
  return overallAccuracy;
}

float ModelTest::getOverallWER()
{
  float overallWER = 0;
  foreach (TestResultLeaf *leaf, resultLeafes)
    overallWER += leaf->wordErrorRate();
  overallWER /= (float) resultLeafes.count();
  return overallWER;
}

int ModelTest::getTotalSampleCount()
{
  return promptsTable.count();
}

TestResultModel* ModelTest::wordResultsModel()
{
  return m_wordResultsModel;
}

TestResultModel* ModelTest::sentenceResultsModel()
{
  return m_sentenceResultsModel;
}

bool ModelTest::analyzeResults()
{
  if (!keepGoing) return false;

  emit status(i18n("Analyzing recognition results..."), 90, 100);
  m_recognizerResultsModel->setResults(recognizerResults);
  m_sentenceResultsModel->setResults(sentenceResults);
  m_wordResultsModel->setResults(wordResults);
  return true;
}


ModelTest::~ModelTest()
{
  deleteAllResults();
  delete m_wordResultsModel;
  delete m_sentenceResultsModel;
  delete m_recognizerResultsModel;
}
