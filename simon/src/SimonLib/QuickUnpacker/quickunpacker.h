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
