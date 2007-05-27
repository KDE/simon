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


void XMLTrainingText::save(QStringList pages, QString path, QString title)
{
/*	QDomDocument doc("mydocument");
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
	}*/
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
		
		QString textcontent = text.text();
		pages.append(textcontent.trimmed());
		
		QDomElement label = text.nextSiblingElement();
		
		labels.append(label.text().trimmed());
		
		page = page.nextSiblingElement();
	}
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
 * \brief Returns the label of the given page index
 * \author Peter Grasch
 * \param int index
 * The index of the label we want to return
 * \return QString
 * The label
 */
QString XMLTrainingText::getLabel(int index)
{
	return labels.at(index);
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

/**
 * \brief Returns all labels of the currently loaded document in a QStringList
 * 
 * Each page is one element in the list
 * This does not, however, load the data if it isn't already stored.
 * Please use load() before using that function
 * 
 * \see load()
 * 
 * \author Peter Grasch
 * \return QStringList
 * The labels of the pages of the text
 */
QStringList XMLTrainingText::getAllLabels()
{
	return labels;
}


XMLTrainingText::~XMLTrainingText()
{
}
