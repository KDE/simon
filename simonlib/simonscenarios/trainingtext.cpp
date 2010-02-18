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
#include "trainingmanager.h"

TrainingText::TrainingText( Scenario *parent ) : ScenarioObject(parent)
{
}

TrainingText::TrainingText(const QString& _name, const QStringList& _pages) : ScenarioObject(0),
	name(_name), pages(_pages)
{
}

TrainingText* TrainingText::createTrainingText(Scenario *parent, const QDomElement& elem)
{
	TrainingText *t = new TrainingText(parent);
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

	return true;
}

QDomElement TrainingText::serialize(QDomDocument *doc)
{
	QDomElement textElem = doc->createElement("trainingtext");
	textElem.setAttribute("name", name);
	foreach (const QString& page, pages) {
		QDomElement pageElem = doc->createElement("page");
		pageElem.appendChild(doc->createTextNode(page));
		textElem.appendChild(pageElem);
	}

	return textElem;
}


float TrainingText::getRelevance() 
{
	return TrainingManager::getInstance()->calcRelevance(this);
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingText::~TrainingText()
{
}


