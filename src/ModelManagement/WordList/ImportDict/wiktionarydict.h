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

#include <QXmlDefaultHandler>
#include "dict.h"


#define WDNONE 0
#define WDWORD 1
#define WDTEXT 2

class XMLSAXReader;

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
	
	bool startElement(const QString &,
			  const QString &,
			  const QString &qName,
			  const QXmlAttributes &attributes);
	
	bool endElement(const QString &, const QString &,
			const QString &qName);

	bool characters (const QString &str);
	
	QStringList findIPAs(QString haystack);
	void insertWords(QString word, QString terminal, QStringList pronunciations);
	
	
    ~WiktionaryDict();

};

#endif
