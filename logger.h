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

#include <QString>
/**
 \class Logger
 \author Peter Grasch
 \brief Logs messages to the logfile with static functions
*/

class QTextStream;
class QFile;

class Logger{
private:
	static QTextStream *logFile;
	static QFile * logF;
	
public:
	/**
	 * \brief Initialises the file handle and the stream
	 * \author Peter Grasch
	 * @param path 
	 * The path to the file - will be created if it doesn't exist
	 * @return 
	 * success
	 */
	static bool init(QString path="log/simon.log");
	
	/**
	 * \brief Logs the given string to the file (adds a timecode)
	 * \author Peter Grasch
	 * @param message 
	 * The message to log
	 */
	static void log(QString message);
	
	/**
	 *        \brief Closes and flushes the buffer
	 *        \author Peter Grasch
	 */
	static void close();
	

};

#endif
