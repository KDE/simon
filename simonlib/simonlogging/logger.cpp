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
#include <QFileInfo>
#include <QDebug>
#include <KDebug>

QTextStream * Logger::logFile = 0;
QFile * Logger::logF = 0;

QTextStream* Logger::init()
{
  //remove logs older than 2 months
  QDate currentDate = QDate::currentDate();
  QDate purgeToDate = currentDate.addMonths(-2);
  
  QString protocolDir = KStandardDirs::locateLocal("appdata", "logs/");
  QStringList files = QDir(protocolDir).entryList(QStringList() << "*.log", QDir::NoDotAndDotDot|QDir::Files);
  foreach (const QString& file, files)
  {
    QString dateStr = file;
    dateStr.remove("protocol-").remove(".log");
    QDate logDate = QDate::fromString(dateStr, Qt::ISODate);
    if (logDate < purgeToDate)
    {
      QString logPath = protocolDir+QDir::separator()+file;
      if (!QFile::remove(logPath))
	kWarning() << "Couldn't remove old log at " << logPath;
    }
  }
  
  QString path = KStandardDirs::locateLocal("appdata", QString("logs/protocol-%1.log").arg(currentDate.toString(Qt::ISODate)));

  Logger::logF = new QFile(path);
  if (!Logger::logF->open(QIODevice::WriteOnly|QIODevice::Append)) return 0;
  Logger::logFile = new QTextStream(Logger::logF);

  return logFile;
}


void Logger::log(QString message, Logger::LogType type)
{
  if (!logFile) logFile = Logger::init();

  QString tag;
  switch (type)
  {
    case Logger::Info:
      tag = "[INF] ";
      break;
    case Logger::Warning:
      tag = "[WRN] ";
      break;
    case Logger::Upgrade:
      tag = "[UPG] ";
      break;
  }
  *(Logger::logFile) << QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss] ") << tag << message << endl;
}


void Logger::close()
{
  (Logger::logFile)->flush();
  delete (Logger::logFile);
  if (Logger::logF)
    Logger::logF->deleteLater();
}
