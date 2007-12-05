//
// C++ Interface: modelmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QThread>
#include <QProcess>
class QProgressDialog;

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
 *	@todo Implementing
 */
class ModelManager : public QThread{
Q_OBJECT
signals:
	void status(QString);
	void error(QString);
	void progress(int now, int total=2000);
private:
	QProgressDialog *processDialog;
	QProcess *proc;
	QString tmpDir;

	bool generateDirectoryStructure();
	
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
	
private slots:
	void setStatus(QString);
	void displayError(QString);
	void setProgress(int now, int max);

	void logError();
	void logInfo();
public slots:
	void cancel();
	
public:
    ModelManager(QWidget *parent=0);

	void run();
	bool compileModel();

    ~ModelManager();

};

#endif
