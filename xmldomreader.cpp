//
// C++ Implementation: xmldomreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "xmldomreader.h"
#include <QMessageBox>

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
}

/**
 * \brief Saves the opened file to the given path
 * \author Peter Grasch
 * \param QString path
 * If no path is given, we use the path given in the path-member
 */
int XMLDomReader::save(QString path)
{
	if (path.isEmpty()) path = this->path;
	QFile file(path);
	if(!file.open(QIODevice::WriteOnly ) )
		return -1;
	QTextStream ts(&file);
	ts << doc->toString(); 
	emit(written());
	file.close();
	emit(closed());
	return 0;
}



/**
 * \brief Opens the file at the given path
 * \author Peter Grasch
 * \param QString path
 * If no path is given, we use the path given in the path-member
 */
void XMLDomReader::load(QString path)
{
	if (path.isEmpty())
        path = this->path;
	doc= new QDomDocument();
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
		return;
	if(!doc->setContent(&file))
	{
		file.close();
		return;
	}
	file.close();
	emit (loaded());
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLDomReader::~XMLDomReader()
{
	
}
