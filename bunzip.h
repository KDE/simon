//
// C++ Interface: bunzip
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUNZIP_H
#define BUNZIP_H

 #include <QObject>
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class QString;
class QProcess;
class Bunzip : public QObject {
Q_OBJECT

signals:
	void extractionFinished(QString filename);
	void errorOccured(QString);
	void extracting(QString filename);
	void progress(int);
	void canceled();

private:
	QProcess *proc;
	QString filename;

private slots:
	void readError();

public slots:
	void cancel();
	void extract(QString filename);
	void extractingFinishing(int exitCode);
public:
    Bunzip(QObject *parent=0);

    ~Bunzip();

};

#endif
