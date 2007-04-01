#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
#include "xmlreader.h"
#include <QList>
#include <QString>
#include <QtXml>
#include <QDomNode>
#include <QDomDocument>
/**
 *  @class XMLDocument
 *  @brief To handle with xml-documents
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner
 *  @todo implementing
 */


typedef QList<QString> TextPerPage;


class XMLDocument:public XMLReader
{
private:
	QString title;
	TextPerPage textperpage;
public: 
	XMLDocument(QString path);
	
	void save(TextPerPage textperpage, QString path, QString title);
	
	void load();
	
	QString getTitle();
	
	QString getPage(int index);
	
	TextPerPage getTextPerPage();
	
	~XMLDocument();
};

#endif