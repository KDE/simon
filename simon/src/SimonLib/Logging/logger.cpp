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
