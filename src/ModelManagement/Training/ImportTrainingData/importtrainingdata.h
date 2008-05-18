//
// C++ Interface: importtrainingdata
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTTRAININGDATA_H
#define IMPORTTRAININGDATA_H

#include <QThread>

class PostProcessing;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportTrainingData : public QThread
{
Q_OBJECT
signals:
	void progress(int now, int max=-1);
	void done();
	void error(QString);
	void status(QString);
private:
	PostProcessing *pp;
	QString directory;
	int prog;
	
	QString extractSaid(QString source);
	QStringList* processSounds(QStringList files, QString destDir);
	bool createPrompts(QStringList dataFiles);
	QStringList* searchDir(QString dir);
	
public slots:
	void run();
	
public:
	ImportTrainingData(QObject* parent);

	bool import(QString directory);

    ~ImportTrainingData();

};

#endif
