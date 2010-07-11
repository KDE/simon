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
#include "simonmodeltest_export.h"

#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif

#ifdef bzero
#undef bzero
#endif

extern "C"
{
  #include <julius/julius.h>
}


class TestResult;

typedef QList<float> FloatList;

class MODELTEST_EXPORT ModelTest : public QThread
{
  Q_OBJECT
    signals:
  void status(const QString&, int progressNow, int progressTotal=100);
  void error(const QString&, const QByteArray& log);
  void recognitionInfo(const QString&);

  void testComplete();
  void testAborted();

  private:
    bool keepGoing;

    Recog *recog;
    FILE *logFile;

    QString buildLog;
    QString lastOutput;

    QString userName;
    QString samplePath;
    QString tempDir;
    QString promptsPath;

    int sampleRate;
    QString hmmDefsPath, tiedListPath, dictPath, dfaPath;
    QString juliusJConf;

    //config options
    QString sox, julius, hResults;

    QHash<QString, QString> promptsTable;

    //QHash<QString /*filename*/, RecognitionResultList /*recognitionresults*/> fileResults;
    QHash<QString /*filename*/, TestResult*> testResults;
    QHash<QString, FloatList> wordRates;
    QHash<QString, FloatList> sentenceRates;

    QHash<QString, QString> recodedSamples;

    void closeLog();

    bool createDirs();

    bool execute(const QString& command, const QString& outputFilePath=QString(),
      const QString& errorFilePath=QString());

    bool parseConfiguration();

    bool recodeAudio();
    bool generateMLF();
    bool recognize();
    bool processJuliusOutput();
    bool analyzeResults();
    void emitError(const QString& message);

  private slots:
    void addStatusToLog(const QString&);
    void addRecognitionInfoToLog(const QString&);

  public:
    explicit ModelTest(const QString& userName, QObject *parent=0);

    void run();
    void abort();

    bool startTest(const QString& hmmDefsPath, const QString& tiedListPath,
      const QString& dictPath, const QString& dfaPath,
      const QString& samplePath, const QString& promptsPath,
      int sampleRate, const QString& juliusJConf);

    bool hasLog();
    QString getGraphicLog();
    QString getLog();

    void recognized(RecognitionResultList);
    void searchFailed();

    QHash<QString, FloatList> getWordRates() {
      return this->wordRates;
    }

    QHash<QString, FloatList> getSentenceRates() {
      return this->sentenceRates;
    }

    TestResult* getTestResult(const QString& fileName) {
      return testResults.value(fileName);
    }

    QHash<QString,TestResult*> getTestResults() {
      return testResults;
    }

    //	QHash<QString, QString> getPrompts() {
    //		return this->promptsTable;
    //	}

    //	QHash<QString, RecognitionResultList> getFileResults() {
    //		return this->fileResults;
    //	}

    //	QString getFileNameByIndex(int index) {
    //		return fileResults.keys().at(index);
    //	}

    //	RecognitionResultList getFileResults(const QString& fileName) {
    //		return fileResults.value(fileName);
    //	}

    //	QString getPromptOfFile(const QString& fileName) {
    //		return promptsTable.value(fileName);
    //	}

    QString getOriginalFilePath(const QString& fileName) {
      return recodedSamples.value(fileName);
    }

    ~ModelTest();

};
#endif
