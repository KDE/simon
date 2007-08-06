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

private slots:
	void setStatus(QString status);
	void setProgress(int currentProg);
	void errorOccured(QString error);
	
public slots:
	void unpack(QString path);
private:
	QProgressDialog *prog;
public:
    QuickUnpacker(QObject* parent);

    ~QuickUnpacker();

};

#endif
