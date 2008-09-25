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


#include "xmlsaxreader.h"
#include <QFile>
#include <QXmlInputSource>
#include <QXmlDefaultHandler>
#include <QXmlSimpleReader>

/**
 * \brief Constructor
 * \author Peter Grasch
 * \param QString path
 * The path to the document
 * \param QObject* parent
 * The parent of the object
 */
XMLSAXReader::XMLSAXReader(QString path, QObject* parent) : XMLReader(path, parent)
{
}



/**
 * \brief Opens the file at the given path
 * \author Peter Grasch
 * \param QString path
 * If no path is given, we use the path given in the path-member
 * \param QXmlDefaultHandler* handler
 * This handler is used to parse the content. It is adviced to subclass QXmlDefaultHandler to provide the functionality
 */
void XMLSAXReader::load(QXmlDefaultHandler* handler, QString path)
{
	if (!handler) return;
	if (path.isEmpty()) path = this->path;
	
	QFile *sourcefile = new QFile(path);
	if (!sourcefile->open(QIODevice::ReadOnly|QIODevice::Text))
		return;
	
	QXmlInputSource source(sourcefile);
	QXmlSimpleReader *reader = new QXmlSimpleReader();
	
	reader->setContentHandler(handler);
	reader->setErrorHandler(handler);
	
	emit (loaded());
	
	reader->parse(source);
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLSAXReader::~XMLSAXReader()
{
	
}
