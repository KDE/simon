/*
 *   Copyright (C) 2009 Dominik Neumeister <neudob06@edvhtl.at>
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

#ifndef BUTTONTABLEMODEL_H
#define BUTTONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QComboBox>
#include "keyboardset.h"

class ButtonTableModel : QAbstractTableModel
{
	private:
		QList<KeyboardSet *> *list;
		QComboBox *cbSet;
		QComboBox *cbTab;

	public:
		ButtonTableModel(QObject *parent);
		ButtonTableModel(QList<KeyboardSet *> *setList, QComboBox *cbSets, QComboBox *cbTabs, QObject *parent);
		int rowCount(const QModelIndex &parent) const;
		int columnCount(const QModelIndex &parent) const;
		QVariant data(const QModelIndex &index, int role) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		bool insertRows(int position, int rows, const QModelIndex &index);
		bool removeRows(int position, int rows, const QModelIndex &index);
		bool setData(const QModelIndex &index, const QVariant &value, int role);
		QList<KeyboardSet *> getList();
		Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif
