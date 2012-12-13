/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "modeltest.h"
#include "recognizerresult.h"
#include "testresult.h"
#include "fileresultmodel.h"
#include "testresultmodel.h"
#include "testresultleaf.h"
#include "testresultinstance.h"

#include <simonlogging/logger.h>
#include <simonrecognizer/recognizer.h>
#include <simonrecognizer/juliusrecognitionconfiguration.h>
#include <simonrecognizer/juliusrecognizer.h>

#ifdef BACKEND_TYPE_BOTH
#include <simonrecognizer/sphinxrecognizer.h>
#include <simonrecognizer/sphinxrecognitionconfiguration.h>
#endif

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

ModelTest::ModelTest(const QString& user_name, QObject* parent) : QThread(parent),
  userName(user_name),
  config(0)
{
  m_recognizerResultsModel = new FileResultModel(this);
  m_wordResultsModel = new TestResultModel(this);
  m_sentenceResultsModel = new TestResultModel(this);
  connect(this, SIGNAL(status(QString,int,int)), this, SLOT(addStatusToLog(QString)));
  connect(this, SIGNAL(recognitionInfo(QString)), this, SLOT(addRecognitionInfoToLog(QString)));
}


bool ModelTest::createDirs()
{
  tempDir = KStandardDirs::locateLocal("tmp", KGlobal::mainComponent().aboutData()->appName()+'/'+userName+"/test/");

  if (tempDir.isEmpty()) return false;

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

  if (!QFile::exists(sox)) {
    QString errorMsg = i18n("SOX cannot be found. Please make sure it is installed correctly.");
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

    recog->uninitialize();

    emit testAborted();
  }
}


//bool ModelTest::startTest(const QString& hmmDefsPath, const QString& tiedListPath,
//const QString& dictPath, const QString& dfaPath,
//const QString& samplePath, const QString& promptsPath,
//int sampleRate, const QString& juliusJConf)
//{
//  abort();
//  wait();

//  this->hmmDefsPath = hmmDefsPath;
//  this->tiedListPath = tiedListPath;
//  this->dictPath = dictPath;
//  this->dfaPath = dfaPath;

//  this->samplePath = samplePath;

//  this->promptsPath = promptsPath;

//  this->sampleRate = sampleRate;
//  this->juliusJConf = juliusJConf;

//  keepGoing=true;

//  buildLog="";

//  if (!parseConfiguration())
//    return false;

//  start();
//  return true;
//}

void ModelTest::deleteAllResults()
{
  m_wordResultsModel->clear();
  m_sentenceResultsModel->clear();
  m_recognizerResultsModel->clear();

  promptsTable.clear();
  qDeleteAll(resultLeafes);
  qDeleteAll(sentenceResults);
  qDeleteAll(wordResults);
  qDeleteAll(recognizerResults);
  recognizerResults.clear();
  wordResults.clear();
  sentenceResults.clear();
  resultLeafes.clear();
}

void ModelTest::run()
{
  if (!createDirs())
    emitError(i18nc("%1 is temporary folder path", "Could not generate temporary folders.\n\nPlease check your permissions for \"%1\".", tempDir));

  deleteAllResults();

  if (!keepGoing) return;
  Logger::log(i18n("Testing model..."));
  emit status(i18n("Preparation"), 0,100);
  
  QStringList audioFilesToRecognize; //FIXME: fill
  if (!prepareTestSet(audioFilesToRecognize)) return;
  if (!recognize(audioFilesToRecognize, config)) return;
  if (!analyzeResults()) return;

  emit status(i18nc("The model test has completed", "Finished"), 100, 100);
  emit testComplete();
}

bool ModelTest::prepareTestSet(QStringList& samples)
{
  if (!keepGoing) return false;

  emit status(i18n("Preparing test set..."), 10, 100);

  QFile promptsFile(promptsPath);

  if (!promptsFile.open(QIODevice::ReadOnly)) {
    //FIXME: Make translatable after string freeze
    emit error("Failed to open prompts file at: " + promptsPath, QByteArray());
    return false;
  }

  QStringList lineWords;
  QString line;
  while (!promptsFile.atEnd()) {
    line = QString::fromUtf8(promptsFile.readLine(3000)).trimmed();
    if (line.isEmpty()) continue;
    lineWords = line.split(' ', QString::SkipEmptyParts);

    QString fileName = lineWords.takeAt(0);

    QString fullPath = samplePath+QDir::separator()+fileName+".wav";
    samples << fullPath;
    promptsTable.insert(fullPath, lineWords.join(" "));
  }
  return true;
}

void ModelTest::emitError(const QString& message)
{
  QByteArray log;
  if (recog)
    log = recog->getLog();
  
  QByteArray out = "<html><head /><body><p>"+log.replace('\n', "<br />")+"</p></body></html>";
  emit error(message, out);
}


bool ModelTest::recognize(const QStringList& fileNames, RecognitionConfiguration *cfg)
{
  if (!keepGoing) return false;
  emit status(i18n("Recognizing..."), 35, 100);

  if (!recog->init(cfg)) {
    emitError(i18nc("%1 is the detailed error message from the Julius recognizer", "Could not initialize recognition: %1.", recog->getLastError()));
    return false;
  }

  if (!keepGoing) {
    return false;
  }

  foreach (const QString& file, fileNames) {
    if (!keepGoing)
      break;
    
    QList<RecognitionResult> recognitionResults = recog->recognize(file);
    
    if (recognitionResults.isEmpty())
      searchFailed(file);
    else
      recognized(file, recognitionResults);
  }
  
  recog->uninitialize();

  return keepGoing;
}


void ModelTest::searchFailed(const QString& fileName)
{
  emit recognitionInfo(i18nc("%1 is file name", "Search failed for: %1", fileName));

  QString prompt = promptsTable.value(fileName);
  QStringList promptWordList = prompt.split(' ');

  TestResult *sentenceResult = getResult(sentenceResults, prompt);
  QList<TestResultLeaf*> sentenceLeafes;

  foreach (const QString& label, promptWordList)
  {
    kDebug() << "Test result dummy leaf: " << label;
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

void ModelTest::recognized(const QString& fileName, RecognitionResultList results)
{
  QString prompt = promptsTable.value(fileName);
  recognizerResults.insert(fileName, new RecognizerResult(prompt, results));

  emit recognitionInfo(i18nc("%1 is prompt", "Prompts entry: %1", prompt));
  if (results.count() > 0)
  {
    emit recognitionInfo(i18nc("%1 is filename, %2 is sentence", "Received recognition result for: %1: %2", fileName, results.at(0).sentence()));

    RecognitionResult& highestRatedResult = results.first();

    TestResult *sentenceResult = getResult(sentenceResults, prompt);

    QList<TestResultLeaf*> leaves = TestResultInstance::parseResult(highestRatedResult);
    resultLeafes << leaves;

    TestResult::parseChildren(prompt, leaves);

    if (!sentenceResult->registerChildren(leaves))
      kWarning() << "Could not process sentence result";

    foreach (TestResultLeaf* leaf, leaves)
    {
      TestResult *wordResult = getResult(wordResults, leaf->originalLabel());
      if (!wordResult->registerChild(leaf))
        kWarning() << "Could not process word result";
    }
  }
}

ModelTest::~ModelTest()
{
  deleteAllResults();
  delete m_wordResultsModel;
  delete m_sentenceResultsModel;
  delete m_recognizerResultsModel;
  delete recog;
  delete config;
}

//////////////////////////////
// Result handling below
//////////////////////////////

FileResultModel* ModelTest::recognizerResultsModel()
{
  return m_recognizerResultsModel;
}

int ModelTest::aggregateLeafDetail(bool(TestResultLeaf::*function)(void)const) const
{
  int out = 0;
  foreach (TestResultLeaf *leaf, resultLeafes)
    if ((leaf->*function)())
      ++out;

  return out;
}

template <typename T>
T ModelTest::aggregateLeafDetail(T(TestResultLeaf::*function)(void)const, bool onlyCorrect, bool average) const
{
  T out = 0;
  foreach (TestResultLeaf *leaf, resultLeafes)
    if (!onlyCorrect || leaf->correct())
      out += (leaf->*function)();

  if (average)
    out /= (T) resultLeafes.count();
  return out;
}

float ModelTest::getOverallConfidence()
{
  float overallConfidence = aggregateLeafDetail<float>(&TestResultLeaf::confidence, 
      true /* only correct */, true /*average*/);
  return overallConfidence;
}

float ModelTest::getOverallAccuracy()
{
  return aggregateLeafDetail<float>(&TestResultLeaf::accuracy, false /* only correct*/, true /*avg*/);
}

float ModelTest::getOverallWER()
{
  return aggregateLeafDetail<float>(&TestResultLeaf::wordErrorRateF, false /* only correct*/, true /*avg*/);
}

int ModelTest::getSubstitutionErrors()
{
  return aggregateLeafDetail(&TestResultLeaf::substitutionError);
}

int ModelTest::getCorrect()
{
  return aggregateLeafDetail(&TestResultLeaf::correct);
}

int ModelTest::getInsertionErrors()
{
  return aggregateLeafDetail(&TestResultLeaf::insertionError);
}

int ModelTest::getDeletionErrors()
{
  return aggregateLeafDetail(&TestResultLeaf::deletionError);
}

int ModelTest::getSentenceCount()
{
  return m_sentenceResultsModel->rowCount();
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
