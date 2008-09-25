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

#ifndef BUNZIP_H
#define BUNZIP_H

 #include "compression.h"

class QString;
class QProcess;

/**
 * \class Bunzip
 * \brief Extracts *.bz2 / *.bzip2 files
 * \author Peter Grasch
 * \note This uses the program bzip2 which might not be installed
 * \version 0.1
 * \date 10.08.2007
 */
class Bunzip : public Compression {
Q_OBJECT
private:
	QProcess *proc;
	QString filename;

private slots:
	void readError();
	void extractingFinishing(int exitCode);

public slots:
	void cancel();
	void extract(QString filename);
public:
    Bunzip(QObject *parent=0);
    ~Bunzip();


};

#endif
