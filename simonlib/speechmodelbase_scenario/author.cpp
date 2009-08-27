/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "author.h"

/**
 * Empty, private constructor
 */
Author::Author(const QString& scenarioId) : ScenarioObject(scenarioId)
{
}

/**
 * Full, public constructor
 */
Author::Author(const QString& scenarioId, const QString& name, const QString& contact) : ScenarioObject(scenarioId)
{
	m_name = name;
	m_contact = contact;
}

/**
 * Factory function
 * \author Peter Grasch
 */
Author* Author::createAuthor(const QString& scenarioId, const QDomElement& elem)
{
	Author *a = new Author(scenarioId);
	if (!a->deSerialize(elem)) {
		delete a;
		a=NULL;
	} 
	return a;
}

bool Author::deSerialize(const QDomElement& authorElem)
{
	if (authorElem.isNull())
		return false;

	QDomElement nameElem = authorElem.firstChildElement();
	QDomElement contactElem = nameElem.nextSiblingElement();
	
	if (nameElem.isNull() || contactElem.isNull())
		return false;

	m_name = nameElem.text();
	m_contact = contactElem.text();

	return true;
}

QDomElement Author::serialize(QDomDocument *doc)
{
	QDomElement elem = doc->createElement("author");

	//TODO: fill it with data

	return elem;
}

