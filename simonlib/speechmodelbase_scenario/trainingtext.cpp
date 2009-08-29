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


#include "trainingtext.h"
// #include "xmltrainingtext.h"

/**
 * TODO: Remove me!
 * @brief Constructor
 *
 *	@author Peter Grasch
 * \param QString name
 * The name of the text
 * \param QStringList pages
 * The pages of the text
 * \param float relevance
 * The relevance of the text - the higher the better it'd be to train the text
 */
TrainingText::TrainingText( QString name, QString path, QStringList pages ) : ScenarioObject("")
{
	this->name = name;
	this->path = path;
	this->pages = pages;
}

TrainingText::TrainingText( const QString& scenarioId ) : ScenarioObject(scenarioId)
{
}


TrainingText* TrainingText::createTrainingText(const QString& scenarioId, const QDomElement& elem)
{
	TrainingText *t = new TrainingText(scenarioId);
	if (!t->deSerialize(elem)) {
		delete t;
		t=NULL;
	}
	return t;
}

bool TrainingText::deSerialize(const QDomElement& elem)
{
	if (elem.isNull()) return false;

	this->name = elem.attribute("name");

	QDomElement pageElem = elem.firstChildElement();
	pages.clear();
	while (!pageElem.isNull()) {
		pages << pageElem.text().trimmed();
		pageElem = pageElem.nextSiblingElement();
	}

	//TODO: remove path...
	this->path=QString();
	return true;
}

QDomElement TrainingText::serialize(QDomDocument *doc)
{
	//TODO: Implement
	QDomElement textElem = doc->createElement("trainingstexts");
	textElem.setAttribute("name", name);
	foreach (const QString& page, pages) {
		QDomElement pageElem = doc->createElement("page");
		pageElem.appendChild(doc->createTextNode(page));
		textElem.appendChild(pageElem);
	}

	return textElem;
}

// bool TrainingText::save()
// {
// 	XMLTrainingText *text = new XMLTrainingText(path);
// 	text->setTitle(name);
// 	text->addPages(pages);
// 	text->save(path);
// 	delete text;
// 
// 	return true;
// }

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingText::~TrainingText()
{
}


