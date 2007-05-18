#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
#include "xmldomreader.h"
#include <QList>
#include <QString>
#include <QtXml>
#include <QDomNode>
#include <QDomDocument>
#include <QMessageBox>

/**
 *  @class XMLTrainingText
 *  @brief To handle with xml-documents
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner, Peter Grasch
 *  @todo implementing
 */
class XMLTrainingText : public XMLDomReader
{
private:
	QString title;
	QStringList pages;
	QStringList labels;
public: 
	XMLTrainingText(QString path);
	
	void save(QStringList pages, QString path, QString title);
	
	int getPageCount();
	
	void load(QString path="");
	
	QString getTitle();
	
	QString getPage(int index);
	QString getLabel(int index);
	
	QStringList getAllPages();
	QStringList getAllLabels();
	
	
	~XMLTrainingText();
};

#endif
