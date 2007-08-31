//
// C++ Implementation: inireader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "inireader.h"
#include <QVariant>
#include <QFile>
#include <QCoreApplication>
#include "logger.h"

IniReader::IniReader(QString path)
{
	this->path = path;
}

bool IniReader::load(QString path)
{
	this->document.clear();
	
	if (path.isEmpty()) path = this->path;
	if (path.isEmpty()) {
		Logger::log(QCoreApplication::tr("[ERR] Kein Dateiname angegeben - konnte INI Datei nicht lesen"));
		return false;
	}
	
	QFile doc(path);
	if (!doc.open(QIODevice::ReadOnly|QIODevice::Text)) {
		Logger::log(QCoreApplication::tr("[ERR] Konnte INI-Datei %1 nicht zum lesen öffnen").arg(path));
		return false;
	}
	
	QTextStream ts(&doc);
	int lineC=1;
	while (!ts.atEnd())
	{
		QString line = ts.readLine(1024);
		QStringList pair = line.split("=");
		if (pair.count() != 2) 
			Logger::log(QCoreApplication::tr("[INF] Ignoriere Zeile %1 in Datei %2: \"%3\"").arg(lineC).arg(path).arg(line));
		 else
			this->document.insert(pair[0], pair[1]);
		
		lineC++;
	}
}

QString IniReader::getKey(QString key)
{
	return document.value(key);
}

IniReader::~IniReader()
{
}


