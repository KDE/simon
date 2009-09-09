/*
 *   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
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

#include "keyboardsetcontainer.h"
#include <QString>
#include <QList>
#include <QFile>
#include <KDebug>
#include <KStandardDirs>

KeyboardSetContainer::KeyboardSetContainer()
{
}

QStringList KeyboardSetContainer::getAvailableSets()
{
	QStringList sets;
	foreach (KeyboardSet *set, setList)
		sets << set->getSetName();
	return sets;
}

KeyboardSet* KeyboardSetContainer::findSet(const QString& setName)
{
	foreach (KeyboardSet *set, setList) {
		if (set->getSetName() == setName)
			//found set
			return set;
	}
	return NULL;
}

QStringList KeyboardSetContainer::getAvailableTabs(const QString& setName)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return QStringList();

	return set->getAvailableTabs();
}

KeyboardTab* KeyboardSetContainer::getTab(const QString& setName, const QString& tab)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return NULL;

	return set->getTab(tab);
}


void KeyboardSetContainer::clear()
{
	kDebug() << "Clearing container";
	qDeleteAll(setList);
	setList.clear();
	kDebug() << "Done clearing container";
}

bool KeyboardSetContainer::load()
{
	QFile f(KStandardDirs::locate("appdata", "conf/keyboardsets.xml"));
	if (!f.open(QIODevice::ReadOnly))
		return false;

	QDomDocument doc;
	if (!doc.setContent(&f)) {
		f.close();
		return false;
	}

	f.close();

	QDomElement setsElem = doc.documentElement();
	if (setsElem.isNull()) return false;

	QDomElement setElem = setsElem.firstChildElement();
	while (!setElem.isNull()) {
		KeyboardSet *set = new KeyboardSet(setElem);
		if (set->isNull())
			delete set;
		else
			setList << set;
		setElem = setElem.nextSiblingElement();
	}
	return true;
}

bool KeyboardSetContainer::save()
{
	QDomDocument doc;
	QDomElement setsElem = doc.createElement("keyboardsets");

	foreach (KeyboardSet *set, setList) {
		QDomElement setElem = set->serialize(&doc);
		setsElem.appendChild(setElem);
	}

	doc.appendChild(setsElem);

	QFile f(KStandardDirs::locateLocal("appdata", "conf/keyboardsets.xml"));
	if (!f.open(QIODevice::WriteOnly))
		return false;
	
	f.write(doc.toString().toUtf8());

	f.close();
	return true;
}



bool KeyboardSetContainer::createSet(const QString& name)
{
	if (findSet(name)) return false; //not unique

	setList.append(new KeyboardSet(name));
	return true;
}

bool KeyboardSetContainer::deleteSet(const QString& name)
{
	KeyboardSet *set = findSet(name);
	if (!set) return false;

	setList.removeAll(set);
	delete set;

	return true;
}

bool KeyboardSetContainer::createTab(const QString& setName, const QString& name)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return false;

	return set->createTab(name);
}

bool KeyboardSetContainer::deleteTab(const QString& setName, const QString& name)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return false;

	return set->deleteTab(name);
}

bool KeyboardSetContainer::moveTabUp(const QString& setName, const QString& tabName)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return false;

	return set->moveTabUp(tabName);
}

bool KeyboardSetContainer::moveTabDown(const QString& setName, const QString& tabName)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return false;

	return set->moveTabDown(tabName);
}

bool KeyboardSetContainer::addButton(const QString& setName, const QString& tab, KeyboardButton *button)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return false;

	return set->addButton(tab, button);
}

bool KeyboardSetContainer::deleteButton(const QString& setName, const QString& tab, KeyboardButton *button)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return false;

	return set->deleteButton(tab, button);

}

bool KeyboardSetContainer::moveButtonUp(const QString& setName, const QString& tab, KeyboardButton *button)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return false;

	return set->moveButtonUp(tab, button);
}

bool KeyboardSetContainer::moveButtonDown(const QString& setName, const QString& tab, KeyboardButton *button)
{
	KeyboardSet *set = findSet(setName);
	if (!set) return false;

	return set->moveButtonDown(tab, button);
}


KeyboardSetContainer::~KeyboardSetContainer()
{
	kDebug() << "Deleting keyboardsetcontainer";
	qDeleteAll(setList);
}

