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
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <KLocalizedString>
#include "../Logging/logger.h"

IniReader::IniReader(QString path)
{
	this->path = path;
}

bool IniReader::load(QString path)
{
	this->document.clear();
	
	if (path.isEmpty()) path = this->path;
	if (path.isEmpty()) {
		Logger::log(i18n("[ERR] Kein Dateiname angegeben - konnte INI Datei nicht lesen"));
		return false;
	}
	
	QFile doc(path);
	if (!doc.open(QIODevice::ReadOnly|QIODevice::Text)) {
		Logger::log(i18n("[ERR] Konnte INI-Datei %1 nicht zum lesen öffnen").arg(path));
		return false;
	}
	
	QTextStream ts(&doc);
	int lineC=1;
	while (!ts.atEnd())
	{
		QString line = ts.readLine(1024);
		QStringList pair = line.split("=");
		if (pair.count() == 2) 
// 			Logger::log(i18n("[INF] Ignoriere Zeile %1 in Datei %2: \"%3\"").arg(lineC).arg(path).arg(line));
			this->document.insert(pair[0], pair[1]);
		
		lineC++;
	}
	return true;
}

QString IniReader::getKey(QString key)
{
	return document.value(key);
}

IniReader::~IniReader()
{
}


