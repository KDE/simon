#ifndef XMLREADER_H
#define XMLREADER_H
#include <QtXml>
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
 
typedef QList<Word*> WordList;
 
class XMLReader{
 
 private:
 	
 	QDomNode root;
 	WordList wordlist;
 
public:
	QDomDocument doc;
	XMLReader(QString name);	
	
	void save(WordList wordlist, QString path);
	
	void load();
	
	WordList getWords();
	
	~XMLReader();
};

#endif
