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
 \class Logger
 \author Peter Grasch
 \brief Logs messages to the logfile with static functions
*/
class Logger{
private:
	static QTextStream *logFile;
	static QFile * logF;
	
public:
    	Logger();
	/**
	 * \brief Initialises the file handle and the stream
	 * \author Peter Grasch
	 * @param path 
	 * The path to the file - will be created if it doesn't exist
	 * @return 
	 * success
	 */
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
	
	/**
	 * \brief Logs the given string to the file (adds a timecode)
	 * \author Peter Grasch
	 * @param message 
	 * The message to log
	 */
	static void log(QString message)
	{
		*(Logger::logFile) << QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss] ") << message << endl;
	}
	
	/**
	 *        \brief Closes and flushes the buffer
	 *        \author Peter Grasch
	 */
	static void close()
	{
		(Logger::logFile)->flush();
	}
	
	
    ~Logger();

};

#endif
