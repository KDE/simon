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

#ifndef SIMON_MODELTEST_H_D0E9CBE71B3343D39663FF655B61ED20
#define SIMON_MODELTEST_H_D0E9CBE71B3343D39663FF655B61ED20

#include <QThread>
#include <QProcess>
#include <QHash>
#include <simonrecognitionresult/recognitionresult.h>
#include <simonrecognizer/recognitionconfiguration.h>
#include "simonmodeltest_export.h"

class RecognizerResult;
class TestResult;
class TestResultLeaf;
class TestResultModel;
class FileResultModel;
class Recognizer;

class MODELTEST_EXPORT ModelTest : public QThread
{
  Q_OBJECT
signals:
  void status(const QString&, int progressNow, int progressTotal=100);
  void error(const QString&, const QByteArray& log);
  void recognitionInfo(const QString&);

  void testComplete();
  void testAborted();

protected slots:
  void addStatusToLog(const QString&);
  void addRecognitionInfoToLog(const QString&);

public:
  explicit ModelTest(const QString& userName, QObject *parent=0);

  void run();
  void abort();

  //    bool startTest(const QString& hmmDefsPath, const QString& tiedListPath,
  //      const QString& dictPath, const QString& dfaPath,const QString& juliusJConf

  virtual bool startTest(const QString& samplePath, const QString& promptsPath,
                      int sampleRate, const QHash<QString, QString> params) = 0;

  bool hasLog();
  QString getGraphicLog();
  QString getLog();

  void recognized(const QString& file, RecognitionResultList);
  void searchFailed(const QString& file);

  FileResultModel* recognizerResultsModel();
  TestResultModel* wordResultsModel();
  TestResultModel* sentenceResultsModel();

  RecognizerResult* getRecognizerResult(const QString& fileName) {
    return recognizerResults.value(fileName);
  }

  QString getOriginalFilePath(const QString& fileName) {
    return recodedSamples.value(fileName);
  }

  float getOverallConfidence();
  float getOverallAccuracy();
  float getOverallWER();

  int getSubstitutionErrors();
  int getCorrect();
  int getInsertionErrors();
  int getDeletionErrors();
  int getSentenceCount();

  void deleteAllResults();

  int getTotalSampleCount();

  ~ModelTest();

protected:
  bool keepGoing;

  QString buildLog;
  QString lastOutput;

  QString userName;
  QString samplePath;
  QString tempDir;
  QString promptsPath;

  int sampleRate;


  //config options
  QString sox;

  Recognizer *recog;
  RecognitionConfiguration *config; //initialization in startTest

  QHash<QString, QString> promptsTable;

  QList<TestResultLeaf*> resultLeafes;
  QHash<QString /*filename*/, RecognizerResult*> recognizerResults;
  QList<TestResult*> wordResults;
  QList<TestResult*> sentenceResults;

  FileResultModel *m_recognizerResultsModel;
  TestResultModel *m_wordResultsModel;
  TestResultModel *m_sentenceResultsModel;

  QHash<QString, QString> recodedSamples;

  TestResult* getResult(QList<TestResult*>& list, const QString& prompt);

  bool createDirs();

  bool execute(const QString& command, const QString& outputFilePath=QString(),
               const QString& errorFilePath=QString());

  bool parseConfiguration();

  bool recodeAudio(QStringList& fileNames);
  bool generateMLF();
  bool recognize(const QStringList& fileNames, RecognitionConfiguration *cfg);
  bool analyzeResults();
  void emitError(const QString& message);

  int aggregateLeafDetail(
      bool (TestResultLeaf::*function)(void)const) const;

  template<typename T>
  T aggregateLeafDetail(
      T (TestResultLeaf::*function)(void)const,
      bool onlyCorrect, bool average) const;


};
#endif
