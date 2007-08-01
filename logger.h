//
// C++ Interface: logger
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOGGER_H
#define LOGGER_H

#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class Logger{
private:
	static QTextStream *logFile;
	static QFile * logF;
	
public:
    	Logger();
	static bool init(QString path="log/simon.log")
	{
		QFileInfo fInfo;
		fInfo.setFile(path);
		
		QDir *dir = new QDir(fInfo.path());
		if ((!dir->exists()) && (!dir->mkpath(path)))
			return false;
		
		logF = new QFile(path);
		if (!logF->open(QIODevice::WriteOnly|QIODevice::Append)) return false;
		Logger::logFile = new QTextStream(logF);
		return true;
	}
	
	static void log(QString message)
	{
		*(Logger::logFile) << QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss] ") << message << endl;
	}
	
	static void close()
	{
		(Logger::logFile)->flush();
	}
    ~Logger();

};

#endif
