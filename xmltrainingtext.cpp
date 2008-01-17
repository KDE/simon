#include "xmltrainingtext.h"
/**
 *  @class XMLTrainingText.cpp
 *  
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner
 */

XMLTrainingText::XMLTrainingText(QString path):XMLDomReader(path)
{
	
}

void XMLTrainingText::save(QString path)
{
	if (path.isEmpty()) path = this->path;
	
	doc = new QDomDocument();
	
	QDomElement root = doc->createElement("text");
	QDomElement page = QDomElement();
	QDomNode text = QDomElement();
	QDomText textc;
	
	root.setAttribute("title", this->title);
	
	for (int i=0; i < this->pages.count(); i++)
	{
		page = doc->createElement("page");
		text = doc->createElement("text");
		textc = doc->createTextNode("text");
		
		textc.setNodeValue(pages.at(i));
		
		text.appendChild(textc);
		page.appendChild(text.toElement());
		root.appendChild(page);
	}
	
	doc->appendChild(root);
	XMLDomReader::save(path);
	
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
int XMLTrainingText::getPageCount()
{
	return pages.count();
}
		

void XMLTrainingText::load(QString path)
{
    XMLDomReader::load(path);
	QDomElement root = doc->documentElement();
	this->title = root.attribute("title");
	
	QDomElement newnode = root.firstChildElement();
	
	QDomElement page = root.firstChild().toElement();
	while(!page.isNull()) 
	{
		QDomElement text = page.firstChildElement();
		
		QString textcontent =text.text().toLatin1();
		pages.append(textcontent.trimmed());
		page = page.nextSiblingElement();
	}
}

void XMLTrainingText::addPages(QStringList pages)
{
	for (int i=0; i < pages.count(); i++) this->pages << pages.at(i);
}



QString XMLTrainingText::getTitle()
{
	return this->title;
}


/**
 * \brief Returns the page at the given index
 * \author Kirschner Christoph (?)
 * \param int index
 * The index of the page
 * \return QString
 * the text
 */
QString XMLTrainingText::getPage(int index)
{
	return pages.at(index);
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
QStringList XMLTrainingText::getAllPages()
{
	return pages;
}

XMLTrainingText::~XMLTrainingText()
{
}
