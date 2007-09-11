//
// C++ Implementation: desktopreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "desktopreader.h"
#include "program.h"
#include "programcategory.h"
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QProcess>

DesktopReader::DesktopReader(QString path) : IniReader(path)
{
	iconname = "";
}

Program* DesktopReader::readDesktopFile(QString path)
{
	if (!load(path)) return NULL;
	
	QString name = getKey("Name");
	QString command = getKey("Exec");
	QString desc = getKey("GenericName");
	QString cats = getKey("Categories");
	QString workingDir = getKey("Path");
	this->iconname = getKey("Icon");

	this->strCategories = cats.split(";");
	
	return new Program(name, command, desc, workingDir);
}


DesktopReader::~DesktopReader()
{
}


