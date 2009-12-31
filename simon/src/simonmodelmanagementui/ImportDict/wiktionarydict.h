/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef SIMON_WIKTIONARYDICT_H_1DAFF5F3EC5D4B65927BCB3BB691E866
#define SIMON_WIKTIONARYDICT_H_1DAFF5F3EC5D4B65927BCB3BB691E866

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
	explicit WiktionaryDict(QString path="", QObject *parent=0);
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
