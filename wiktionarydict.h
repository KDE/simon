//
// C++ Interface: wiktionarydict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WIKTIONARYDICT_H
#define WIKTIONARYDICT_H

#include "xmlsaxreader.h"
#include <QStringList>
#include <QString>
#include <QFile>
#include <QXmlDefaultHandler>
#include "dict.h"
#include <QRegExp>
#include <math.h>


#define WDNONE 0
#define WDWORD 1
#define WDTEXT 2

/**
 * \class WiktionaryDict
 * \author Peter Grasch
 * \brief Describes a Wiktionary XML database dump in
*/
class WiktionaryDict : public QXmlDefaultHandler, public Dict {

	
private:
	QString allowedChars;
	XMLSAXReader *reader;
	QString word, text;
	int currentTag; //what are we currently parsing [NONE, WORD, TEXT]
	int pos; //pos in bytes
	int maxpos;
	int processFoundIPA(QString ipa);
	QString cleanTitle(QString title);
	
public:
	WiktionaryDict(QString path="", QObject *parent=0);
	void load(QString path="");
	
	bool startElement(const QString &namespaceURI,
			  const QString &localName,
			  const QString &qName,
			  const QXmlAttributes &attributes);
	
	bool endElement(const QString &namespaceURI, const QString &localName,
			const QString &qName);

	bool characters (const QString &str);
	
	QStringList findIPAs(QString haystack);
	void insertWords(QString word, QString terminal, QStringList pronunciations);
	
	
    ~WiktionaryDict();

};

#endif
