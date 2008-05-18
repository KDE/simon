//
// C++ Interface: compression
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <QObject>
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class Compression : public QObject
{
Q_OBJECT

signals:
	void extractionFinished(QString filename);
	void errorOccured(QString);
	void extracting(QString filename);
	void progress(int);
	void canceled();

	public slots:
		virtual void cancel()=0;
		virtual void extract(QString filename)=0;

	public:
		Compression(QObject *parent) : QObject(parent) {}
		~Compression(){}

};

#endif
