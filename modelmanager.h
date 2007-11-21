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
	void progress(int now, int total=1000);
private:
	QProgressDialog *processDialog;
	QProcess *proc;
	bool generateDirectoryStructure();
	
	bool generateInputFiles();
		bool generateWlist();
	
private slots:
	void setStatus(QString);
	void displayError(QString);
	void setProgress(int now, int max);
public slots:
	void cancel();
	
public:
    ModelManager(QWidget *parent=0);

	void run();
	bool compileModel();

    ~ModelManager();

};

#endif
