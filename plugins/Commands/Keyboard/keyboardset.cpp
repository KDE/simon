/*
 *   Copyright (C) 2009 Mario Strametz <strmam06@htl-kaindorf.ac.at>
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

#include "keyboardset.h"
#include <QString>
#include <QList>

KeyboardSet::KeyboardSet(const QDomElement& elem)
	: m_isNull(false)
{
	if (elem.isNull()) m_isNull = true;
	else {
		setName = elem.attribute("name");
		QDomElement tabElement = elem.firstChildElement();
		while (!tabElement.isNull()) {
			KeyboardTab *tab = new KeyboardTab(tabElement);
			if (!tab->isNull())
				tabList << tab;
			else delete tab;
			tabElement = tabElement.nextSiblingElement();
		}
	}

}

KeyboardSet::KeyboardSet(QString name, QList<KeyboardTab *> tList)
	: tabList(tList),
	setName(name),
	m_isNull(false)
{
}

QStringList KeyboardSet::getAvailableTabs()
{
	if (m_isNull) return QStringList();

	QStringList headers;
	foreach (KeyboardTab *t, tabList) {
		headers << t->getTabName();
	}
	return headers;
}

KeyboardTab* KeyboardSet::getTab(const QString tabName)
{
	if (m_isNull) return NULL;

	foreach (KeyboardTab *t, tabList) {
		if (t->getTabName() == tabName)
			return t;
	}
	return NULL;;
}


void KeyboardSet::addTab(KeyboardTab* t)
{
	if (m_isNull) return;

	tabList.append(t);
}

void KeyboardSet::delTab(int index)
{
	if (m_isNull) return;

	tabList.removeAt(index);
}

void KeyboardSet::tabLeft(int index)
{	
	if (m_isNull) return;

	tabList.insert((index-1),tabList.takeAt(index));
}

void KeyboardSet::tabRight(int index)
{
	if (m_isNull) return;

	tabList.insert((index+1),tabList.takeAt(index));
}

QString KeyboardSet::getSetName()
{
	if (m_isNull) return QString();

	return setName;
}

QDomElement KeyboardSet::serialize(QDomDocument* doc)
{
	QDomElement setElem = doc->createElement("set");
	setElem.setAttribute("name", setName);

	foreach (KeyboardTab *tab, tabList) {
		setElem.appendChild(tab->serialize(doc));
	}
	
	return setElem;
}

KeyboardTab* KeyboardSet::findTab(const QString& tabName)
{
	foreach (KeyboardTab *tab, tabList) {
		if (tab->getTabName() == tabName)
			return tab;
	}
	return NULL;
}

bool KeyboardSet::createTab(const QString& name)
{
	if (findTab(name)) return false; //make sure the name is unique

	tabList.append(new KeyboardTab(name));
	return true;
}

bool KeyboardSet::deleteTab(const QString& name)
{
	KeyboardTab *tab = findTab(name);
	if (!tab) return false;

	tabList.removeAll(tab);
	delete tab;
	return true;
}


KeyboardSet::~KeyboardSet()
{
	qDeleteAll(tabList);
}

