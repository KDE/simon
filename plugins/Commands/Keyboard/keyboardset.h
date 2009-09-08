/*
 *   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
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

#ifndef KEYBOARDSET_H
#define KEYBOARDSET_H

#include "keyboardtab.h"
#include <QList>
#include <QString>
#include <QStringList>
#include <QDomElement>

class QDomDocument;

class KeyboardSet
{
	private:

		QList<KeyboardTab *> tabList;
		QString setName;
		bool m_isNull;

		KeyboardTab* findTab(const QString& tabName);

		void tabLeft(int index);
		void tabRight(int index);
		void addTab(KeyboardTab* t);
		void delTab(int index);
		
	public:
		bool isNull() { return m_isNull; }
		KeyboardSet(const QDomElement& elem);
		KeyboardSet(QString name, QList<KeyboardTab *> tList=QList<KeyboardTab*>());

		QStringList getAvailableTabs();

		KeyboardTab* getTab(const QString tabName);
		bool createTab(const QString& name);
		bool deleteTab(const QString& name);
		bool addButton(const QString& tabName, KeyboardButton *button);
		bool deleteButton(const QString& tabName, KeyboardButton *button);

		QString getSetName();

		QDomElement serialize(QDomDocument* doc);
		~KeyboardSet();
};

#endif
