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
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QDebug>

DesktopReader::DesktopReader(QString path) : IniReader(path)
{ }

Program* DesktopReader::readDesktopFile(QString path)
{
	if (!load(path)) return NULL;
	
	QString name = getKey("Name");
	QString command = getKey("Exec");
	QString desc = getKey("GenericName");
	QString cats = getKey("Categories");
// 	ProgramCategories *categories = new ProgramCategories();
	QStringList categories = cats.split(";");
	
	qDebug() << name << command << desc;
	return new Program(name, command, desc);
}


DesktopReader::~DesktopReader()
{
}


