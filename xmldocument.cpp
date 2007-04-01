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


void XMLDocument::save(TextPerPage textperpage, QString path, QString title)
{
	QDomDocument doc("mydocument");
	QDomElement root = doc.createElement("document");
	doc.appendChild(root);
	
	QDomNode newnode;
	newnode.setNodeValue(title);
	
	for(int i=0; i<=textperpage.size(); i++)
	{
		QDomNode page;
		page.setNodeValue(textperpage.value(i));
		newnode.appendChild(page);

		root.appendChild(newnode);
	}
}

void XMLDocument::load()
{
	QDomElement root = doc.documentElement();
	
	QDomElement newnode = root.firstChildElement();
	this->title = newnode.text();
	
	QDomNode n = newnode.firstChild();
	QDomElement page = n.toElement();
	
	while(!page.isNull()) 
	{
		QString text = page.text();
			
		textperpage.append(text);
		page = n.nextSiblingElement();
	}
}


QString XMLDocument::getTitle()
{
	return this->title;
}


QString XMLDocument::getPage(int index)
{
	return textperpage[index];
}


TextPerPage XMLDocument::getTextPerPage()
{
	return textperpage;
}


XMLDocument::~XMLDocument()
{
}