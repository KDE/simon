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
WiktionaryDict::WiktionaryDict(QString path) : XMLReader(path)
{
}

/**
 * \brief Loads the file
 * \author Peter Grasch
 * \param QString path
 * The path to the dict
 */
void WiktionaryDict::load(QString path)
{
	XMLReader::load(path);
	
	QDomElement root = doc->documentElement();
	QDomNode meta;
	QString title;
	QString text;
	QString terminal;
	QDomElement metaInfo;
	QDomElement revision;
	short indexOfStartTerm;
	short indexOfEndTerm;
	short indexOfStartIPA;
	short indexOfCloseIPA;
	QString IPA;
	
	QDomNode el=root.firstChild();
	int max=0;
	while(!el.isNull()) 
	{ max++; el = el.nextSibling(); }
	
	
	QDomElement page = root.firstChildElement();
	int i=0;
	
	while(!page.isNull()) 
	{
		if (page.tagName()=="page")
		{
			metaInfo = page.firstChildElement();
			if (metaInfo.tagName() == "title")
				title = metaInfo.text();
			else title = metaInfo.nextSiblingElement("title").text();
			
			revision = metaInfo.nextSiblingElement("revision");
			if (revision.firstChildElement().tagName() == "text")
				text = revision.firstChildElement().text();
			else text = revision.firstChildElement().nextSiblingElement("text").text();
			
			
			indexOfStartTerm = text.indexOf("{{Wortart|");
			if (indexOfStartTerm != -1)
			{
				indexOfStartTerm+=10;
				indexOfEndTerm=text.indexOf("|",indexOfStartTerm);
				terminal = text.mid(indexOfStartTerm, 
						indexOfEndTerm-indexOfStartTerm);
				if (text.mid(indexOfEndTerm+1).startsWith("Deutsch"))
				{
					indexOfStartIPA = text.indexOf("IPA]]: {{Lautschrift|");
					if (indexOfStartIPA != -1)
					{
						indexOfStartIPA+= 21;
						indexOfCloseIPA = text.indexOf("}}",indexOfStartIPA);
					
						IPA = text.mid(indexOfStartIPA, indexOfCloseIPA-indexOfStartIPA);
					
						if ((!IPA.isEmpty()) && (IPA != "..."))
						{
							words.append(title);
							pronunciations.append(IPA);
							terminals.append(terminal);
						}
					}
				}
			}
		}
		emit progress(round((((double)i)/(((double)max)))*1000));
		i++;
		page = page.nextSiblingElement();
	}
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
WiktionaryDict::~WiktionaryDict()
{
}


