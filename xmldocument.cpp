#include "xmldocument.h"
/**
 *  @class XMLDocument.cpp
 *  
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner
 *  @todo implementing
 */

XMLDocument::XMLDocument(QString path):XMLReader(path)
{
	//XMLReader reader(path);
	
}


void XMLDocument::save(QStringList pages, QString path, QString title)
{
	QDomDocument doc("mydocument");
	QDomElement root = doc.createElement("document");
	doc.appendChild(root);
	
	QDomNode newnode;
	newnode.setNodeValue(title);
	
	for(int i=0; i<=pages.size(); i++)
	{
		QDomNode page;
		page.setNodeValue(pages.value(i));
		newnode.appendChild(page);

		root.appendChild(newnode);
	}
}

/**
 * \brief Returns the count of pages of the currently loaded text
 * 
 * This does not, however, load the data if it isn't already stored.
 * Please use load() before using that function
 * 
 * \see load()
 * \author Peter Grasch
 * \return int
 * pages
 */
int XMLDocument::getPageCount()
{
	return pages.count();
}
		

void XMLDocument::load(QString path)
{
     XMLReader::load(path);
	QDomElement root = doc->documentElement();
	
	this->title = root.attribute("title");
	QDomElement newnode = root.firstChildElement();
	
	QDomNode n = root.firstChild();
	QDomElement page = n.toElement();
	int i=0;
	while(!page.isNull() && (i<10)) 
	{
		QString text = page.text();
		pages.append(text.trimmed());
		page = page.nextSiblingElement();
		i++;
	}
}


QString XMLDocument::getTitle()
{
	return this->title;
}


QString XMLDocument::getPage(int index)
{
	return pages[index];
}



/**
 * \brief Returns all pages of the currently loaded document in a QStringList
 * 
 * Each page is one element in the list
 * This does not, however, load the data if it isn't already stored.
 * Please use load() before using that function
 * 
 * \see load()
 * 
 * \author Peter Grasch
 * \return QStringList
 * The pages of the text
 */
QStringList XMLDocument::getAllPages()
{
	return pages;
}


XMLDocument::~XMLDocument()
{
}
