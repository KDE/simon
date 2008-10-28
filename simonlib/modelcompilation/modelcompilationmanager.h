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


#ifndef MODELCOMPILATIONMANAGER_H
#define MODELCOMPILATIONMANAGER_H

#include <QThread>
#include "simonmodelcompilationmanagement_export.h"
class QProcess;

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
class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationManager : public QThread{
Q_OBJECT
signals:
	void status(const QString&, int progressNow, int progressTotal=2300);
	void error(const QString&);
	void missingWord(const QString&);
	void sampleWithoutWord(const QString&);
	void unknownGrammarClass(const QString&);

	void modelCompiled();
// 	void missingPhoneme(const QString&);
private:
	QString currentStatus;
	
	QProcess *proc;
	QString lastOutput;
	QString lastError;


	QString userName;
	QString samplePath;
	QString lexiconPath, grammarPath, vocabPath, promptsPath, treeHedPath, wavConfigPath;
	QString hmmDefsPath, tiedListPath, dictPath, dfaPath;



	
	bool generateInputFiles();
		bool generateWlist();
		bool makeMonophones();

	bool makeTranscriptions();
		bool generateMlf();

	bool codeAudioData();
		bool generateCodetrainScp();

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

	bool compileGrammar();
		bool generateReverseGrammar();
		bool makeTempVocab();
		bool makeDfa();
		bool generateDict();
	
private slots:
	void logInfo();
	bool processError(const QString& userError);

protected:
	
public:
    ModelCompilationManager(const QString& userName, const QString& samplePath,
			     const QString& lexiconPath, const QString& grammarPath, 
			     const QString& vocabPath, const QString& promptsPath, 
			     const QString& treeHedPath, const QString& wavConfigPath,
			     const QString& hmmDefsPath, const QString& tiedListPath,
			     const QString& dictPath, const QString& dfaPath,
			     QObject *parent=0);

	void run();
	bool startCompilation();
	
	QString getStatus() { return currentStatus; }

    ~ModelCompilationManager();

};

#endif
