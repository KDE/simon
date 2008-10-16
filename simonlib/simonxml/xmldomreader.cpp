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


#include "xmldomreader.h"
#include <QObject>
#include <QDomDocument>
#include <QIODevice>
#include <QTextStream>
#include <QTextCodec>
#include <KFilterDev>
#include <KMimeType>

/**
 * \brief Constructor
 * \author Peter Grasch
 * \param QString path
 * The path to the document
 * \param QObject* parent
 * The parent of the object
 */
XMLDomReader::XMLDomReader(QString path, QObject* parent) : XMLReader(path, parent)
{
	this->doc = 0;
}

/**
 * \brief Saves the opened file to the given path
 * \author Peter Grasch
 * \param QString path
 * If no path is given, we use the path given in the path-member
 */
bool XMLDomReader::save(QString path)
{
	if (path.isEmpty()) path = this->path;


	QIODevice *file = KFilterDev::deviceForFile(path,
							KMimeType::findByFileContent(path)->name());
	if((!file) || (!file->open(QIODevice::WriteOnly)))
	{
		return false;
	}

	QTextStream ts(file);
	ts.setCodec("UTF-8");
	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	ts << doc->toString();
	emit(written());
	file->close();
	file->deleteLater();
	emit(closed());
	return true;
}



/**
 * \brief Opens the file at the given path
 * \author Peter Grasch
 * \param QString path
 * If no path is given, we use the path given in the path-member
 */
bool XMLDomReader::load(QString path)
{
	if (path.isEmpty())
        path = this->path;
	
	if (doc) delete doc;
	doc= new QDomDocument();


	QIODevice *file = KFilterDev::deviceForFile(path,
							KMimeType::findByFileContent(path)->name());
	
	if((!file) || (!file->open(QIODevice::ReadOnly)))
		return false;

	if (!doc->setContent(file))
		return false;

	file->close();
	file->deleteLater();

	emit (loaded());
	return true;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLDomReader::~XMLDomReader()
{
	delete doc;
}
