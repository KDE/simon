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

#include "xmltrainingtext.h"
#include <QDomDocument>



XMLTrainingText::XMLTrainingText( QString name, QString path, QStringList pages ):XMLDomReader(path), TrainingText(name, path, pages)
{
	
}

XMLTrainingText::XMLTrainingText( QString path ) : XMLDomReader(path), TrainingText("", path, QStringList())
{
}

void XMLTrainingText::save(QString path)
{
	if (!doc) doc = new QDomDocument();
	doc->clear();
	
	QDomElement root = doc->createElement("text");
	QDomElement page = QDomElement();
	QDomNode text = QDomElement();
	QDomText textc;
	
	root.setAttribute("title", name);
	
	for (int i=0; i < this->pages.count(); i++)
	{
		page = doc->createElement("page");
		text = doc->createElement("text");
		textc = doc->createTextNode("text");
		
		textc.setNodeValue(pages.at(i));
		
		text.appendChild(textc);
		page.appendChild(text.toElement());
		root.appendChild(page);
	}
	
	doc->appendChild(root);
	XMLDomReader::save(path);
}

bool XMLTrainingText::save()
{
	save(TrainingText::path);
	return true;
}

void XMLTrainingText::load(QString path)
{
	XMLDomReader::load(path);
	QDomElement root = doc->documentElement();
	this->name = root.attribute("title");
	
	QDomElement newnode = root.firstChildElement();
	
	QDomElement page = root.firstChild().toElement();
	while(!page.isNull()) 
	{
		QDomElement text = page.firstChildElement();
		
		QString textcontent =text.text();
		pages.append(textcontent.trimmed());
		page = page.nextSiblingElement();
	}
	TrainingText::path=path;
}

void XMLTrainingText::addPages(QStringList pages)
{
	for (int i=0; i < pages.count(); i++) this->pages << pages.at(i);
}

XMLTrainingText::~XMLTrainingText()
{
}
