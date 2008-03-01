//
// C++ Implementation: categoryxmlreader
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "categoryxmlreader.h"
#include <QDomDocument>
#include <QVariant>
#include "settings.h"

/**
 * \brief Constructor - initializes the XMLDomReader with the paths
 * \author Peter Grasch
 * @param path The path of the file
 * @param parent Set the parent to the given parent
 */
CategoryXMLReader::CategoryXMLReader(QString path, QObject* parent): XMLDomReader(path, parent)
{
	this->categoryList = new ProgramCategoryList();
}


/**
 * \brief Parses the category xml file
 * \author Peter Grasch
 * @param path If the path is empty the path given to initialize the object is used; if it is still empty we return
 */
void CategoryXMLReader::load(QString path)
{
	if (path.isEmpty()) path = this->path;
	if (path.isEmpty()) path = Settings::get ( "PathToProgramCategories" ).toString();

    XMLDomReader::load(path);
	QDomNodeList categories = doc->elementsByTagName("category");
    
	ProgramCategoryList *catList=new ProgramCategoryList();

	for (int i=0; i < categories.count(); i++)
	{
		QDomNode category = categories.at(i);
		QString name = category.toElement().attribute("name");
		QString desc = category.firstChildElement("description").text();
		QIcon icon = QIcon(category.firstChildElement("icon").text());
		QStringList formats;
		
		QDomNode format = category.firstChildElement("formats").firstChild();
		while (!format.isNull())
		{
			formats << format.toElement().text();
			format = format.nextSibling();
		}
		
		catList->append(ProgramCategory(name, desc, icon, formats));
	}

	if (this->categoryList) {
		delete this->categoryList;
		this->categoryList=0;
	}

	this->categoryList = catList;
}

CategoryXMLReader::~CategoryXMLReader()
{
    delete this->categoryList;
}

