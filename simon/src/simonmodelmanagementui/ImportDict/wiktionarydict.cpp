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


#include "wiktionarydict.h"

#include <QStringList>
#include <QString>
#include <QFile>
#include <QRegExp>
#include <simonxml/xmlsaxreader.h>

/**
 * \brief Constructor
 * \author Peter Grasch
 * \param QString path
 * The path to the dict
 */
WiktionaryDict::WiktionaryDict(QString path, QObject* parent) :QXmlDefaultHandler(), Dict(parent)
{
	buildTranslationTables();
	allowedChars = "-abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890äÄüÜöÖ";
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
 * Success. (in the current implementation this returns always true)
 */
bool WiktionaryDict::startElement(const QString&,
			      const QString&,
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
		currentTag = WDWORD;
	} else if (qName == "text")
			currentTag = WDTEXT;
		else currentTag = WDNONE;
	
		
	//adding the length of the xml data to the position
	//this is actually quite useless and slows down the process
	//but it's still there to get a better estimation of the current progress
	//even with this workaround we still loose a bit of accuracy (which is why
	//the progressbar is quite "jumpy" at the end
	pos += qName.count()+4;
	for(int i=0; i<attributes.count(); i++)
		pos += attributes.qName(i).count() + attributes.value(i).count()+4;
	
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
 * Success. (in the current implementation this returns always true)
 */
bool WiktionaryDict::endElement(const QString&, const QString&,
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
		
		QString ipasingle;
		QString ipaplural;
		if (pluralstart != -1)
		{
			ipasingle = text.mid(indexOfStartIPA, pluralstart-indexOfStartIPA);
			ipaplural = text.mid(pluralstart);
		}
		else ipasingle = text.mid(indexOfStartIPA);
		
		QStringList ipas = findIPAs(ipasingle);
		
		if (!cleanTitle(word).isEmpty())
			insertWords(cleanTitle(word), terminal.trimmed(), ipas);
		
		
		if (pluralstart != -1)
		{
			int ptitlestart;
			//the first space after (Mehrzahl)= is the start of the word
			//e.g.: (Mehrzahl)=die Melonen
			ptitlestart = text.indexOf(" ", text.indexOf(QString("(Mehrzahl)=")));
			QString pluraltitle = text.mid(ptitlestart, 
					text.indexOf(QRegExp("(\n|\\|)"), 
					ptitlestart)-ptitlestart);
			
			pluraltitle = cleanTitle(pluraltitle);
			
			QStringList ipap = findIPAs(ipaplural);
			
			//deal with ipa
			if (!pluraltitle.isEmpty() && 
					(pluraltitle.indexOf(QString("(Einzahl)")) == -1))
				insertWords(pluraltitle, terminal.trimmed(), ipap);
		}
		
		emit progress(qRound(((double)pos/(double)maxpos)*1000));
	}
	return true;
}


/**
 * \brief Cleans the title from unwanted spaces, html tags, etc.
 * \author Peter Grasch
 */
QString WiktionaryDict::cleanTitle ( QString title )
{
	//strip <...>
	while (title.indexOf("<") != -1)
	{
		title.remove(title.indexOf("<"), title.indexOf(">", title.indexOf("<")));
	}
	//strip (...)
	while (title.indexOf("(") != -1)
	{
		title.remove(title.indexOf("("), title.indexOf(")", title.indexOf("(")));
	}
	title.remove("&nbsp;");
	title= title.trimmed();
	title.remove(QRegExp(".*>"));
	title.remove("*");
	//if some parenthesis isn't closed it isn't caught by the stripping algorithm above
	title.remove("(");
	title.remove(")");
	title.remove(QRegExp("^(-|=).*"));
	if (title.contains(QRegExp("[^"+allowedChars+"]"))) return "";
	
	title = title.trimmed();
	if (title.indexOf(" ")) //more words than one
	{
		//kill generic stuff
		title.remove(QRegExp("^(der|den|dem|das|des|ein|eine|eines|einer|einem|die|the)"));
	}
	
	return title.trimmed();
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
	QStringList ipas;
	int inserted=0;
	
	ipas = ipa.split(QRegExp("(;|,|\\/)"), QString::SkipEmptyParts);
	for (int i= 0; i < ipas.size(); i++)
	{
		ipa=ipas.at(i);
		//removing bogus
		ipa.remove("...");
		
		//if we start with a "-" it cannot be a syllable break IPA-char so we guess that
		//it is used like "test/-s" or "test, -s" and replace acordingly
		//we use the local variable "ipas" instead of the pronunciations because if the
		//preceding ipa was empty it wouldn't have been added to the list and prepending
		//the last recorded pronunciation would cause a faulty pronunciation in that 
		//case
		if (ipa.startsWith("-"))
		{
			if (i>0) {
				ipa.insert(0, ipas.at(i-1));
			} else
				ipa.remove(QRegExp("-.*"));
		}
		
		//with the substitution taken care of, we can safely remove all syllable brakes
		ipa.remove("-");
		
		
// 		ipa.left(ipa.indexOf("/")); //this is needed because we could get stuff like:
// 			//"test/-e" and we can _not_ in any way distinguish the "-"
// 			//from the IPA key-char "-" (its the same, obviously) so we
// 			//strip the hole second part and sacrifice stuff like test/tist
		
		ipa.remove(0x2026); //UTF-8 for the horizontal ellipsis 
				//(the character looking like "...")
		ipa.remove("-");
		ipa.remove("{{fehlend}}", Qt::CaseInsensitive);
		ipa.remove("betont", Qt::CaseInsensitive);
		ipa.remove(QRegExp("\\(.*\\)"));
		
		ipa = ipa.trimmed();
		ipas.replace(i, ipa); //for easy access for the "-" substitution
		
		if (!ipa.isEmpty())
		{
			this->pronunciations << ipaToXSampa(ipa);
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
	if (currentTag == WDWORD)
		word += str;
	if (currentTag == WDTEXT)
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
	reader->deleteLater();
}


