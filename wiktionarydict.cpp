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
	pos=0;
}


/**
 * \brief Overwritten method of the QXmlDefaultHandler - used to react on the start of a tag
 * \author Peter Grasch
 * 
 * This function is called internally every time the parser finds the start of a new tag
 * We then decide if the tag is relevant for us (the name for instance can be determined by looking
 * at the qName attribute, and react on it
 * 
 * \param QString namespaceURI
 *  namespaceURI is the namespace URI, or an empty string if the element has no namespace URI or if no namespace processing is done
 * \param QString &localName
 * localName is the local name (without prefix), or an empty string if no namespace processing is done
 * \param QString &qName
 * qName is the qualified name (with prefix)
 * \param QXmlAttributes &attributes
 * atts are the attributes attached to the element. If there are no attributes, atts is an empty attributes object.
 * \return bool
 * Success. (in the current implementation this returns allways true)
 */
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
	
		
	//adding the length of the xml data to the position
	//this is actually quite useless and slows down the process
	//but it's still there to get a better estimation of the current progress
	//even with this workaround we still loose a bit of accuracy (which is why
	//the progressbar is quite "jumpy" at the end
	pos += qName.count()+3;
	for(int i=0; i<attributes.count(); i++)
		pos += attributes.qName(i).count() + attributes.value(i).count()+1;
	
	return true;
}


/**
 * \brief React on the closing of a tag
 * \author Peter Grasch
 * 
 * This function is called internally every time the parser sees the closing of a tag. 
 * We then react on that by parsing the data we gathered while "in" the tag.
 * We extract the terminal, the pronunciation(s) and insert this data into the 3 stringlists
 * (words, terminals, pronunciations)
 * 
 * \param QString namespaceURI
 *  namespaceURI is the namespace URI, or an empty string if the element has no namespace URI or if no namespace processing is done
 * \param QString &localName
 * localName is the local name (without prefix), or an empty string if no namespace processing is done
 * \param QString &qName
 * qName is the qualified name (with prefix)
 * \return bool
 * Success. (in the current implementation this returns allways true)
 */
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
		IPA = IPA.trimmed();
				
		if ((!IPA.isEmpty()) && (IPA != "...")) //if everything seems alright
		{
			words.append(word.trimmed());
			pronunciations.append(ipaToXSampa(IPA));
			terminals.append(terminal);
		}
		emit progress(round(((double)pos/(double)maxpos)*1000));
	}
	return true;
}


/**
 * \brief This function gatheres the "value" of the xml tags
 * \author Peter Grasch
 * 
 * This is an overloaded function of the defaulthandler;
 * We use it to gather the data between the starting and the closing of a tag;
 * First we determine if we are in a WORD, TEXT, or none of both tags.
 * We then add the data to the word (member), the text (member) or nowhere (resp.)
 * We also increase the current position (needed for the progress)
 * \param QString str
 * The gathered data
 * \return bool
 * Success (in the current implementation this always returns true)
 */
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


