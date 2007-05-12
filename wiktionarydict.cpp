//
// C++ Implementation: wiktionarydict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wiktionarydict.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * \param QString path
 * The path to the dict
 */
WiktionaryDict::WiktionaryDict(QString path, QObject* parent) : QObject(parent), QXmlDefaultHandler(), Dict()
{
	this->reader = new XMLSAXReader(path);
}


bool WiktionaryDict::startElement(const QString &namespaceURI,
			      const QString &localName,
				const QString &qName,
				const QXmlAttributes &attributes)
{
	if (qName == "page")
	{
		word="";
		text="";
	}
	
	if (qName == "title")
	{
		currentTag = WORD;
	} else if (qName == "text")
			currentTag = TEXT;
		else currentTag = NONE;
		
	return true;
}

bool WiktionaryDict::endElement(const QString &namespaceURI, const QString &localName,
			    const QString &qName)
{
	if (qName == "text")
	{
		int indexOfStartTerm = text.indexOf("{{Wortart|");
		if (indexOfStartTerm == -1) return true;
		
		
		//found the terminal
		
		indexOfStartTerm+=10;
		int indexOfEndTerm=text.indexOf("|",indexOfStartTerm);
		QString terminal = text.mid(indexOfStartTerm, 
				indexOfEndTerm-indexOfStartTerm);
		
		//return if not a german word
		if (!text.mid(indexOfEndTerm+1).startsWith("Deutsch")) return true;
		
		
		int indexOfStartIPA = text.indexOf("IPA]]: {{Lautschrift|");
		if (indexOfStartIPA == -1) return true;	//return if word doesn't come
							//with a valid pronunciation
		
		indexOfStartIPA+= 21;
		int indexOfCloseIPA = text.indexOf("}}",indexOfStartIPA);
			
		QString IPA = text.mid(indexOfStartIPA, indexOfCloseIPA-indexOfStartIPA).trimmed();
		
		//takes only the first pronunciation
		//FIX THIS!
		if (IPA.indexOf(","))
			IPA = IPA.left(IPA.indexOf(","));
		IPA.remove(' ');
				
		if ((!IPA.isEmpty()) && (IPA != "...")) //if everything seems alright
		{
			words.append(word);
			pronunciations.append(ipaToXSampa(IPA));
			terminals.append(terminal);
		}
		emit progress(round(pos/maxpos*1000));
	}
	return true;
}


bool WiktionaryDict::characters(const QString &str)
{
	if (currentTag == WORD)
		word += str;
	if (currentTag == TEXT)
		text += str;
	
	pos += str.count();
	
	return true;
}


/**
 * \brief Loads the file
 * \author Peter Grasch
 * \param QString path
 * The path to the dict
 */
void WiktionaryDict::load(QString path)
{
	connect(reader, SIGNAL(loaded()), this, SIGNAL(loaded()));
	this->maxpos = QFile(path).size();
	reader->load(this, path);
	
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
WiktionaryDict::~WiktionaryDict()
{
}


