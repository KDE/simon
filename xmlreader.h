#ifndef XMLREADER_H
#define XMLREADER_H
#include <QtXml>
#include <QObject>
#include <QString>
#include <QList>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QDomNode>
#include <QDomDocument>
#include "word.h"
/**
 *  @class XMLReader
 *  @brief To handle with xml-files
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner
 *  @todo implementing
 */
 

 
class XMLReader : public QObject {
 	Q_OBJECT
signals:
	void loaded();
	void closed();
	void written();

protected:
 	QDomDocument *doc;
 	QString path;
 
public:
	XMLReader(QString path, QObject *parent=0);	
	QDomElement settingToNode(QDomDocument &d, QString name, QString value );
	int save(QString path="");
	
	void load(QString path="");
	
	~XMLReader();
};

#endif
