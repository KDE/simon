#include "xmlreader.h"
/**
 *  @class XMLReader.cpp
 *  
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner
 *  @todo Implementing
 */
XMLReader::XMLReader(QString name)
{
		QDomDocument doc("mydocument");
		this->doc=doc;
		QFile file(name);
		if(!file.open(QIODevice::ReadOnly))
			return;
		if(!doc.setContent(&file))
		{
			file.close();
			return;
		}
		file.close();
}


void XMLReader::save(WordList wordlist, QString path)
{
	QDomDocument doc("mywords");
	QDomElement root = doc.createElement("wordlist");
	doc.appendChild(root);
	
	QDomNode n;

	for(int i=0; i<=wordlist.size(); i++)
	{
		QStringList pronuns = wordlist[i]->getPronunciations();
		QString allpronuns = "";
		for(int j=0; j<pronuns.count(); j++)
		{
			allpronuns = allpronuns + *(wordlist[i]->getPronunciation(j)) + " | ";
		}

		n.setNodeValue(allpronuns);
		
		QDomElement word = n.toElement();

		word.setTagName("word");
		
		word.setAttribute("word", wordlist[i]->getWord());
		word.setAttribute("terminal", wordlist[i]->getTerminal());
		word.setAttribute("propability", QString().setNum(wordlist[i]->getPropability()));
		
		root.appendChild(word);
	}
}


void XMLReader::load()
{
	QDomElement root = this->doc.documentElement();
	
	QDomNode n = root.firstChild();
	QDomElement word = n.toElement();

	while(!word.isNull())
	{
		QString sword = word.attribute("word");
		QString terminal = word.attribute("terminal");
		int propability = (word.attribute("propability")).toInt();
		QString allpronuns = word.text();
					
		QStringList pronunciations = allpronuns.split("|");
		wordlist.append(new Word(sword, pronunciations, terminal, propability));
		
		word = n.nextSiblingElement();
	}
}

QList<Word*> XMLReader::getWords()
{
	return this->wordlist;
}


XMLReader::~XMLReader()
{
}
