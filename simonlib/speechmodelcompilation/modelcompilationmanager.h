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

#ifndef SIMON_MODELCOMPILATIONMANAGER_H_18BCB183592A48D3962815FFDEA7C214
#define SIMON_MODELCOMPILATIONMANAGER_H_18BCB183592A48D3962815FFDEA7C214

#include <QThread>
#include <QProcess>
#include <QMutex>
#include <QString>
#include "simonmodelcompilationmanagement_export.h"

class AudioCopyConfig;
class ReestimationConfig;

/**
 *	@class ModelManager
 *	@brief Manages the Acoustic-Model
 *
 *	Provides the ability to modify/recompile or otherwise maintain the
 *	Acoustic model
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationManager : public QThread
{
  Q_OBJECT
    signals:
  void status(QString, int progressNow, int progressTotal=2600);
  void error(QString);

  //	void userReadableError(const QString&);

  void wordUndefined(const QString&);
  void classUndefined(const QString&);
  void phonemeUndefined(const QString&);

  void modelCompiled();
  void activeModelCompilationAborted();

  public:
    enum CompilationType
    {
      CompileLanguageModel=1,
      CompileSpeechModel=2,
      AdaptSpeechModel=4
    };

    explicit ModelCompilationManager(const QString& userName, QObject *parent=0);

    void run();
    bool startCompilation(ModelCompilationManager::CompilationType compilationType,
      const QString& hmmDefsPath, const QString& tiedListPath,
      const QString& dictPath, const QString& dfaPath,
      const QString& baseHmmDefsPath, const QString& baseTiedlistPath,
      const QString& baseStatsPath, const QString& baseMacrosPath,
      const QString& samplePath,
      const QString& lexiconPath, const QString& grammarPath,
      const QString& vocabPath, const QString& promptsPath,
      const QString& treeHedPath, const QString& wavConfigPath,
      const QString& scriptBasePrefix);
    bool hasBuildLog();
    QString getGraphicBuildLog();
    QString getBuildLog();

    void abort();

    ~ModelCompilationManager();

    static QString information(bool condensed=false);

    //helper functions from outside
    bool codeAudioDataFromScp(const QString& path);
    bool reestimate(const QString& command);

  private:
    bool keepGoing;
    bool catchUndefiniedPhonemes;
    QByteArray undefinedPhoneme;

    QMutex buildLogMutex;
    QByteArray buildLog;

    CompilationType compilationType;

    QString userName;
    QString samplePath;
    QString tempDir;
    QString lexiconPath, grammarPath, vocabPath, promptsPath, treeHedPath, wavConfigPath;
    QString baseHmmDefsPath, baseTiedlistPath, baseMacrosPath, baseStatsPath;
    QString hmmDefsPath, tiedListPath, dictPath, dfaPath;
    QString scriptBasePrefix;

    //config options
    QString hDMan, hLEd, hCopy, hCompV, hERest, hHEd, hVite, mkfa, dfaMinimize;

    QList<QProcess*> activeProcesses;

    QString htkIfyPath(const QString& in);
    QString createHMMPath(int number);
    bool pruneScp(const QString& inMlf, const QString& inScp, const QString& outScp);

    void analyseError(QString readableError);
    bool processError();

    bool createDirs();

    bool execute(const QString& command);

    bool parseConfiguration();

    bool generateInputFiles();
    bool generateWlist();
    bool makeMonophones();

    bool makeTranscriptions();
    bool generateMlf();

    bool codeAudioData();
    bool generateCodetrainScp(QStringList &codeTrainScps);

    bool buildHMM();

    bool createMonophones();
    bool buildHMM0();
    bool buildHMM1();
    bool buildHMM2();
    bool buildHMM3();

    bool fixSilenceModel();
    bool buildHMM4();
    bool buildHMM5();
    bool buildHMM6();
    bool buildHMM7();

    bool realign();
    bool makeDict1();
    bool realignHMM7();
    bool buildHMM8();
    bool buildHMM9();

    bool makeTriphones();
    bool makeMkTriHed();
    bool buildHMM10();
    bool buildHMM11();
    bool buildHMM12();

    bool tieStates();
    bool makeFulllist();
    bool makeTreeHed();
    bool buildHMM13();
    bool buildHMM14();
    bool buildHMM15();

    bool increaseMixtures();
    bool shouldIncreaseMixtures();
    bool doesIncreaseMixtures(const QString& script);
    
    QString getScriptFile(const QString& id);
    QStringList getScriptFiles(const QString& id);
    
    int getBaseModelMixtureCount();
    bool prepareGlobalConfig();

    bool adaptBaseModel();
    bool realignToBaseModel();
    bool makeRegTreeHed();
    bool createRegressionClassTree();
    bool staticAdaption();

    bool compileGrammar();
    bool generateReverseGrammar();
    bool makeTempVocab();
    bool makeDfa();
    bool generateDict();

    static QString htkInformation(bool condensed);
    static QString juliusInformation(bool condensed);

    QStringList getMixtureConfigs();

    bool reestimate(const QString& mlf, bool useStats, const QString& scp, const QString& inputMacros, 
        const QString& inputHMMs, const QString& outputDirectory, const QString& phoneList, 
        const QStringList& additionalConfigs=QStringList(), const QString& additionalParameters="");
    
    bool splitScp(const QString& scpIn, const QString& outputDirectory, const QString& fileNamePrefix, QStringList& scpFiles);

    bool removePhoneme(const QByteArray& phoneme);

  private slots:
    void addStatusToLog(const QString&);
};
#endif
