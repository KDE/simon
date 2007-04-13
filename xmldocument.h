#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
#include "xmlreader.h"
#include <QList>
#include <QString>
#include <QtXml>
#include <QDomNode>
#include <QDomDocument>
#include <QMessageBox>
/**
 *  @class XMLDocument
 *  @brief To handle with xml-documents
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner
 *  @todo implementing
 */




class XMLDocument : public XMLReader
{
private:
	QString title;
	QStringList pages;
public: 
	XMLDocument(QString path);
	
	void save(QStringList pages, QString path, QString title);
	
	int getPageCount();
	
	void load(QString path="");
	
	QString getTitle();
	
	QString getPage(int index);
	
	QStringList getAllPages();
	
	~XMLDocument();
};

#endif
