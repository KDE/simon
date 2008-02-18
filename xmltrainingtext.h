#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
#include "xmldomreader.h"
#include <QStringList>

/**
 *  @class XMLTrainingText
 *  @brief To handle with xml-documents
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner, Peter Grasch
 */
class XMLTrainingText : public XMLDomReader
{
private:
	QString title;
	QStringList pages;
public: 
	XMLTrainingText(QString path);
	
	int getPageCount();
	void save(QString path="");
	
	void load(QString path="");
	
	QString getTitle();
	
	QString getPage(int index);
	
	QStringList getAllPages();
	
	void addPages(QStringList pages);
	void setTitle(QString title) { this->title =title; }
	
	
	~XMLTrainingText();
};

#endif
