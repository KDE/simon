//
// C++ Interface: quickunpacker
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QUICKUNPACKER_H
#define QUICKUNPACKER_H

#include "bunzip.h"
#include <QObject>

/**
 \class QuickUnpacker
 \author Peter Grasch
 \date 06.08.2007
 \brief Displays a nice progressdialog and unpacks the file
 \note Uses the Bunzip class
*/

class QProgressDialog;
class QString;
class QObject;

class QuickUnpacker : public QObject
{
Q_OBJECT

signals:
	void unpacking(QString);
	void unpackedTo(QString);
	void status(QString);
	void progress(int);
	void canceled();
	void error(QString);

private slots:
	void setStatus(QString status);
	void setProgress(int currentProg);
	void errorOccured(QString err);
	void cancel();
	
public slots:
	void unpack(QString path);
private:
	QProgressDialog *prog;
	Bunzip *bunzip;
public:
    QuickUnpacker(QObject* parent);

    ~QuickUnpacker();

};

#endif
