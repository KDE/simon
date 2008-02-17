//
// C++ Implementation: xmltrainingtextlist
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "xmltrainingtextlist.h"
#include "logger.h"
#include <QMessageBox>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param path 
 * Path to the textlist - default NULL
 */
XMLTrainingTextList::XMLTrainingTextList(QString path) : XMLDomReader(path)
{
	
}



/**
 * \brief Loads the document
 * Calls the XMLDomReader::load with the given path
 * \author Peter Grasch
 * @param path 
 * Path to the list - default NULL
 */
bool XMLTrainingTextList::load(QString path)
{
	if (path.isEmpty()) path = this->path;
	Logger::log(tr("Laden einer Liste von Trainingstexten von %1").arg(path));
	if (!XMLDomReader::load(path)) return false;
	
	trainingtexts.clear();
	
	QDomElement root = doc->documentElement();
	
	QDomElement text = root.firstChildElement();
	while(!text.isNull()) 
	{
		trainingtexts.insert(text.attribute("name"), text.attribute("url"));
		text = text.nextSiblingElement();
	}
	return true;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLTrainingTextList::~XMLTrainingTextList()
{
}


