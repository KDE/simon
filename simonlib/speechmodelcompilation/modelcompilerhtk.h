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

#ifndef SIMON_MODELCOMPILERHTK_H_18BCB183592A48D3962815FFDEA7C214
#define SIMON_MODELCOMPILERHTK_H_18BCB183592A48D3962815FFDEA7C214

#include <QProcess>
#include <QMutex>
#include <QString>
#include "modelcompiler.h"
#include "simonmodelcompilationmanagement_export.h"

class AudioCopyConfig;
class ReestimationConfig;

class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilerHTK : public ModelCompiler
{
  Q_OBJECT
  public:
    explicit ModelCompilerHTK(const QString& userName, QObject *parent=0);

    bool startCompilation(ModelCompiler::CompilationType compilationType, const QString& modelDestination, 
                          const QString& baseModelPath,const QHash<QString, QString>& args);
    bool hasBuildLog() const;
    QString getGraphicBuildLog() const;
    QString getBuildLog() const;

    void abort();

    QString information(bool condensed=false) const;

    //helper functions from "outside" for multithreaded processes
    bool codeAudioDataFromScp(const QString& path);
    bool reestimate(const QString& command);
    
  protected:
    bool compile(ModelCompiler::CompilationType compilationType,
      const QString& destinationPath,
      const QString& baseModelPath,
      const QString& samplePath,
      const QString& lexiconPath, const QString& grammarPath,
      const QString& vocabPath, const QString& promptsPath,
      const QString& scriptBasePrefix);
    
    bool unpack(const QString& archive, const QString& targetDir);
    bool pack(const QString& targetArchive);

  private:
    bool keepGoing;
    bool catchUndefiniedPhonemes;
    QByteArray undefinedPhoneme;

    QMutex buildLogMutex;
    QByteArray buildLog;

    CompilationType compilationType;

    QString samplePath;
    QString tempDir;
    QString lexiconPath, grammarPath, vocabPath, promptsPath, treeHedPath, wavConfigPath;
    QString baseHmmDefsPath, baseTiedlistPath, baseMacrosPath, baseStatsPath;
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

    QString htkInformation(bool condensed) const;
    QString juliusInformation(bool condensed) const;

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
