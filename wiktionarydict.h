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
#include <QDebug>
#include <QRegExp>
#include <math.h>


#define NONE 0
#define WORD 1
#define TEXT 2

/**
 * \class WiktionaryDict
 * \author Peter Grasch
 * \brief Describes a Wiktionary XML database dump in
*/
class WiktionaryDict : public QObject, public QXmlDefaultHandler, public Dict {
	
	Q_OBJECT

signals:
	void loaded();
	void progress(int prog);
	
private:
	XMLSAXReader *reader;
	QString word, text;
	int currentTag; //what are we currently parsing [NONE, WORD, TEXT]
	QStringList words, terminals, pronunciations;
	int pos; //pos in bytes
	int maxpos;
	int processFoundIPA(QString ipa);
	
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
	
	QStringList getWords() { return words; }
	QStringList getPronuncations() { return pronunciations; }
	QStringList getTerminals() { return terminals; }
	
    ~WiktionaryDict();

};

#endif
