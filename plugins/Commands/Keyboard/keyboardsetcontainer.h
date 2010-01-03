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

#ifndef SIMON_KEYBOARDSETCONTAINER_H_810E188B84254EA9BEE2EABBF057A6B8
#define SIMON_KEYBOARDSETCONTAINER_H_810E188B84254EA9BEE2EABBF057A6B8

#include "keyboardset.h"
#include <QList>
#include <QString>
#include <QStringList>
#include <QDomElement>

class KeyboardSetContainer
{
	private:
		QList<KeyboardSet *> setList;

	public:
		KeyboardSetContainer();

		QStringList getAvailableSets();
		QStringList getAvailableTabs(const QString& set);
		KeyboardTab* getTab(const QString& set, const QString& tab);

		void clear();

		bool createSet(const QString& name);
		bool deleteSet(const QString& name);

		bool createTab(const QString& set, const QString& name);
		bool deleteTab(const QString& set, const QString& name);
		bool moveTabUp(const QString& set, const QString& tabName);
		bool moveTabDown(const QString& set, const QString& tabName);

		bool addButton(const QString& set, const QString& tab, KeyboardButton *button);
		bool deleteButton(const QString& set, const QString& tab, KeyboardButton *button);
		bool moveButtonUp(const QString& set, const QString& tab, KeyboardButton *button);
		bool moveButtonDown(const QString& set, const QString& tab, KeyboardButton *button);

		KeyboardSet* findSet(const QString& setName);

		bool loadFromFile(const QString& path);
		bool deSerialize(const QDomElement&);
		bool parseElement(const QDomElement&);
		QDomElement serialize(QDomDocument *doc);
		~KeyboardSetContainer();

};

#endif
