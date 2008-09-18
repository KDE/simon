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

#include <QWidget>

/**
 \class QuickUnpacker
 \author Peter Grasch
 \date 06.08.2007
 \version 0.1
 \brief Displays a nice progressdialog and unpacks the file
 \note Uses the Bunzip class
*/

class KProgressDialog;
class QString;
class QObject;
class Compression;

class QuickUnpacker : public QWidget
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
	void unpacked(QString to);
	void setStatus(QString status);
	void setProgress(int currentProg);
	void errorOccured(QString err);
	void cancel();
	
public slots:
	void unpack(QString path);
private:
	KProgressDialog *prog;
	Compression *compression;
public:
    QuickUnpacker(QWidget* parent);

    ~QuickUnpacker();

};

#endif
