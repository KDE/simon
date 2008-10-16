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
#include <KStandardDirs>
#include <QDebug>

QTextStream * Logger::logFile = 0;
QFile * Logger::logF = 0;


QTextStream* Logger::init()
{
	QString path = KStandardDirs::locateLocal("appdata", "protocol.log");

	Logger::logF = new QFile(path);
	if (!Logger::logF->open(QIODevice::WriteOnly|QIODevice::Append)) return 0;
	Logger::logFile = new QTextStream(Logger::logF);

	return logFile;
}


void Logger::log(QString message)
{
	if (!logFile) logFile = Logger::init();

	*(Logger::logFile) << QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss] ") << message << endl;
}

void Logger::close()
{
	(Logger::logFile)->flush();
	delete (Logger::logFile);
}
