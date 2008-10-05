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

#include "logger.h"
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>

QTextStream * Logger::logFile;
QFile * Logger::logF;



bool Logger::init(QString path)
{
	QFileInfo fInfo;
	fInfo.setFile(path);
	
	QDir *dir = new QDir(fInfo.absoluteDir());
	if ((!dir->exists()) && (!dir->mkpath(path)))
		return false;
	delete dir;
	
	logF = new QFile(path);
	if (!logF->open(QIODevice::WriteOnly|QIODevice::Append)) return false;
	Logger::logFile = new QTextStream(logF);
	return true;
}


void Logger::log(QString message)
{
	*(Logger::logFile) << QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss] ") << message << endl;
}

void Logger::close()
{
	(Logger::logFile)->flush();
	delete (Logger::logFile);
}
