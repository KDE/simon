//
// C++ Implementation: xmlsaxreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
