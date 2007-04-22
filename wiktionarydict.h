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

#include "xmlreader.h"
#include <QStringList>
#include <QString>
#include <QThread>
#include <iostream>
#include <math.h>

/**
 * \class WiktionaryDict
 * \author Peter Grasch
 * \brief Describes a Wiktionary XML database dump in
*/
class WiktionaryDict : public XMLReader {
	Q_OBJECT
signals:
	void progress(int prog);
private:
	QStringList words;
	QStringList pronunciations;
	QStringList terminals;
	
public:
    WiktionaryDict(QString path="");
	void load(QString path="");
	
	QStringList getWords() { return words; }
	QStringList getPronuncations() { return pronunciations; }
	QStringList getTerminals() { return terminals; }
	
    ~WiktionaryDict();

};

#endif
