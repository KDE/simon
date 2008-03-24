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
#include <QLocale>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param path The path to initialize the inireader with
 */
DesktopReader::DesktopReader(QString path) : IniReader(path)
{
	iconname = "";
}

/**
 * \brief Reads the desktopfile from the given path
 * \author Peter Grasch
 * @param path The path of the desktopfile
 * @return The read program
 */
Program* DesktopReader::readDesktopFile(QString path)
{
	if (!load(path)) return NULL;

	QString locale = QLocale::system().name().remove(2,3);

	QString name;

	QString localename = QString("Name[%1]").arg(locale);
	if (!getKey(localename).isEmpty())
		name = getKey(localename);
	else name = getKey("Name");

	QString command = getKey("Exec");

	QString desc;
	QString localeGenDesc = QString("GenericName[%1]").arg(locale);
	if (!getKey(localename).isEmpty())
		desc = getKey(localeGenDesc);
	else
		desc = getKey("GenericName");


	QString cats = getKey("Categories");
	QString workingDir = getKey("Path");
	this->iconname = getKey("Icon");

	this->strCategories = cats.split(";");
	
	return new Program(name, command, desc, workingDir);
}
