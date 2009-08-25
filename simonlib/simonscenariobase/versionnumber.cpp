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

#include "versionnumber.h"
#include <QRegExp>
#include <QStringList>

VersionNumber::VersionNumber(const QString& scenarioId) : ScenarioObject(scenarioId)
{
	m_majorNumber = m_minorNumber = m_patchLevel = -1;
}

VersionNumber::VersionNumber(const QString& scenarioId, const QString& version) : ScenarioObject(scenarioId)
{
	parseString(version);
}

bool VersionNumber::parseString(const QString& version)
{
	QString v = version;

	//0.2-beta1
	//0.2rc1
	//0.2.2-alpha-1
	v.remove(QRegExp("/([a-z]|-)([a-z]|-)*[0-9]*/i"));

	//0.2
	//0.2
	//0.2.2

	bool valid=true;

	QStringList elements = v.split(".", QString::SkipEmptyParts);

	if (elements.count() == 3) {
		m_patchLevel = elements[2].toInt(&valid);
		if (valid)
			m_majorNumber = elements[0].toInt(&valid);
		if (valid)
			m_minorNumber = elements[1].toInt(&valid);
	} else {
		m_patchLevel = 0;
		if (elements.count() == 2) { 
			m_majorNumber = elements[0].toInt(&valid);
			if (valid)
				m_minorNumber = elements[1].toInt(&valid);
		} else 
			valid=false;
	}

	if (!valid) {
		m_majorNumber = m_minorNumber = m_patchLevel = -1;
		return false;
	} else return true;
}

bool VersionNumber::isValid()
{
	return ((m_majorNumber >= 0) && (m_minorNumber >= 0) && (m_patchLevel >= 0));
}


bool VersionNumber::operator<=(const VersionNumber& v2) const
{
	if (m_majorNumber < v2.majorNumber()) return true;
	if (m_majorNumber > v2.majorNumber()) return false;

	//majorNumber is identicle

	if (m_minorNumber < v2.minorNumber()) return true;
	if (m_minorNumber > v2.minorNumber()) return false;

	//minorNumber is identicle

	return (patchLevel() <= v2.patchLevel());
}

bool VersionNumber::operator<(const VersionNumber& v2) const
{
	return ((operator<=(v2)) && (!(operator==(v2))));
}

bool VersionNumber::operator==(const VersionNumber& v2) const
{
	return ((majorNumber() == v2.majorNumber()) && (minorNumber() == v2.minorNumber()) && (patchLevel() == v2.patchLevel()));
}

bool VersionNumber::operator>=(const VersionNumber& v2) const
{
	if (majorNumber() < v2.majorNumber()) return false;
	if (majorNumber() > v2.majorNumber()) return true;

	//majorNumber is identicle

	if (minorNumber() < v2.minorNumber()) return false;
	if (minorNumber() > v2.minorNumber()) return true;

	//minorNumber is identicle

	return (patchLevel() >= v2.patchLevel());
}

/**
 * Factory function
 * \author Peter Grasch
 */
VersionNumber* VersionNumber::createVersionNumber(const QString& scenarioId, const QDomElement& elem)
{
	VersionNumber *v = new VersionNumber(scenarioId);
	if (!v->deSerialize(elem)) {
		delete v;
		v=NULL;
	} 
	return v;
}

bool VersionNumber::deSerialize(const QDomElement& versionElem)
{
	if (versionElem.isNull())
		return false;

	return parseString(versionElem.text());
}

QDomElement VersionNumber::serialize(QDomDocument *doc)
{
	QDomElement elem = doc->createElement("author");

	//TODO: fill it with data

	return elem;
}

