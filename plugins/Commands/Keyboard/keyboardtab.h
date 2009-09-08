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

#ifndef KEYBOARDTAB_H
#define KEYBOARDTAB_H

#include "keyboardbutton.h"
#include <QList>
#include <QString>
#include <QAbstractItemModel>
#include <QDomElement>


class QDomDocument;

class KeyboardTab : public QAbstractItemModel
{
	private:
		QString tabName;
		bool m_isNull;

		QList<KeyboardButton *> buttonList;
		KeyboardButton* findButton(const QString& name);
		
		
	public:
		bool isNull() { return m_isNull; }

		bool triggerButton(const QString& trigger);

		KeyboardTab(QString name, QList<KeyboardButton *> bList=QList<KeyboardButton*>());
		KeyboardTab(const QDomElement& elem);

		QString getTabName();

		bool addButton(KeyboardButton *button);
		bool deleteButton(KeyboardButton *button);

		bool moveButtonUp(KeyboardButton *button);
		bool moveButtonDown(KeyboardButton *button);

		QList<KeyboardButton*> getTabButtons() { return buttonList; }

		//Model stuff
		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant headerData(int, Qt::Orientation orientation,
					int role = Qt::DisplayRole) const;

		QModelIndex index(int row, int column,
				const QModelIndex &parent = QModelIndex()) const;

		QModelIndex parent(const QModelIndex &index) const;

		int rowCount(const QModelIndex &parent = QModelIndex()) const;

		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		QDomElement serialize(QDomDocument *doc);
		~KeyboardTab();


};

#endif
