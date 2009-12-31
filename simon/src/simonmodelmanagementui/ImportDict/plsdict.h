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


#ifndef SIMON_PLSDICT_H_E6F757214F0F4DEC87C08BEACBF9A51E
#define SIMON_PLSDICT_H_E6F757214F0F4DEC87C08BEACBF9A51E

#include <QXmlDefaultHandler>
#include "dict.h"


class XMLSAXReader;

/**
 * \class PLSDict
 * \author Peter Grasch
 * \brief Describes a PLS XML database dump in
*/
class PLSDict : public QXmlDefaultHandler, public Dict {

	
private:
	enum PLSTagType {
		None=0,
		Lexicon=1, /* root element */
		Meta=2, /* element containing meta data */
		Metadata=4, /* element containing meta data */
		Lexeme=8, /* the container for a single lexical entry */
		Grapheme=16, /*Contains orthographic information for a lexeme */
		Phoneme=32, /* Contains the pronunciation of the lexeme */
		Alias=64, /* Acronym expansions and orthographic substitutions */
		Example=128 /* Example usage of the lexeme */
	};

	XMLSAXReader *reader;

	QString currentWord;
	QString currentTerminal;
	QString currentPhonemeDefinition;
	QStringList phonemeDefinitions;
	
	PLSTagType currentTagType;
	int pos; //pos in bytes
	int maxpos;

	
public:
	explicit PLSDict(QObject *parent=0);
	void load(QString path, QString encodingName);
	
	bool startElement(const QString &,
			  const QString &,
			  const QString &qName,
			  const QXmlAttributes &attributes);
	
	bool endElement(const QString &, const QString &,
			const QString &qName);

	bool characters (const QString &str);
	
	
    ~PLSDict();

};

#endif
