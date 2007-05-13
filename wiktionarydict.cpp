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
		
		int pluralstart = text.indexOf(QRegExp("(Plurarl|Pl\\.)"),
					       indexOfStartIPA);
		
		QString ipasingle; //only the pronunciation of singular for now
		QString ipaplural;
		if (pluralstart != -1)
		{
			ipasingle = text.mid(indexOfStartIPA, pluralstart-indexOfStartIPA);
			ipaplural = text.mid(pluralstart);
		}
		else ipasingle = text.mid(indexOfStartIPA);
		
		QStringList ipas = findIPAs(ipasingle);
		insertWords(word.trimmed(), terminal.trimmed(), ipas);
		
		
		if (pluralstart != -1)
		{
			int ptitlestart;
			//the first space after (Mehrzahl)= is the start of the word
			//e.g.: (Mehrzahl)=die Melonen
			ptitlestart = text.indexOf(" ", text.indexOf(QString("(Mehrzahl)=")));
			QString pluraltitle = text.mid(ptitlestart, 
					text.indexOf(QRegExp("(\n|\\|)"), 
					ptitlestart)-ptitlestart);
			
			pluraltitle.remove(QRegExp("&lt;.*&gt;"));
			
			QStringList ipap = findIPAs(ipaplural);
			
			//deal with ipa
			if (!pluraltitle.trimmed().isEmpty() && 
					(pluraltitle.indexOf(QString("(Einzahl)")) == -1))
				insertWords(pluraltitle.trimmed(), terminal.trimmed(), ipap);
		}
		
		emit progress(round(((double)pos/(double)maxpos)*1000));
	}
	return true;
}


/**
 * \brief Insert words for the given data into the members
 * \author Peter Grasch
 * \param QString word
 * The name of the word
 * \param QString terminal
 * The terminal
 * \param QStringList pronunciations
 * The found "pronunciations" (most certainly from findIPAs());
 * \see findIPAs() processFoundIPA()
 */
void WiktionaryDict::insertWords(QString word, QString terminal, QStringList pronunciations)
{
	for (int prons=0; prons<pronunciations.count(); prons++)
	{
		int found = processFoundIPA(pronunciations.at(prons));
		for (int i=0; i < found; i++)
		{
			words.append(word.trimmed());
			terminals.append(terminal);
		}
	}
}

/**
 * \brief Searches the given strings for the wiktionary indicators that an ipa follows
 * \author Peter Grasch
 * \param QString haystack
 * The wikipedia xml string
 * \return QStringList
 * every found ipa. Please note that this extracts the wikipedia "IPAs" not the real ipas; they may still contain other/wrong information
 * \see processFoundIPA()
 */
QStringList WiktionaryDict::findIPAs(QString haystack)
{
	
	int start = haystack.indexOf(QString("{{Lautschrift|"));
	int end;
	QStringList ipas;
	while (start != -1)
	{
		start += 14;
		end = haystack.indexOf("}}", start);
		
		ipas << haystack.mid(start, end-start);
		
		haystack = haystack.mid(end);
		start = haystack.indexOf(QString("{{Lautschrift|"));
	}
	
	return ipas;
}


/**
 * \brief Parses the given IPA string and inserts it (if valid) into the pronunciations
 * \author Peter Grasch
 * \param QString ipa
 * We assume that this is wiktionary input so we skip stuff like "betont" and make a "fuzzy" conversion (e.g. interpret also ' instead of the correct unicode char. 0x02C8
 * \return int
 * How many found sampas were in the given IPA string?
 */
int WiktionaryDict::processFoundIPA(QString ipa)
{
	int inserted = 0;
	
	//if we determine that the pronunciation is not finished/the writer
		//was not so sure, we ignore it
	if ((ipa.indexOf("?") != -1) || (ipa.indexOf("/") != -1)) return false;
		
		
	ipa.remove(' ');
	ipa.remove("betont", Qt::CaseInsensitive);
	QStringList IPAs = ipa.split(QRegExp("(;|,)"), QString::SkipEmptyParts);
		
	for (int i=0; i<IPAs.count(); i++)
	{
		if ((IPAs.at(i).startsWith("-"))&&(i>0))
		{
				//a syllable seperator is here out of
				//order, so we guess that it means "in-addition-to-the-prior"
			IPAs.replace(i, QString(IPAs.at(i)).remove(0,1)); 
				//remove the "-"
			IPAs.replace(i, QString(IPAs.at(i)).insert(0, IPAs.at(i-1)));
				//so we take the preceding element and insert it
		}
				
		if ((!IPAs.at(i).trimmed().isEmpty()) && 
				    (IPAs.at(i).trimmed() != "...")) //if everything seems alright
		{
			pronunciations.append(ipaToXSampa(IPAs.at(i).trimmed()));
			inserted++;
		}
	}
	return inserted;
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


