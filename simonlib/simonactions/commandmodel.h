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

#ifndef COMMANDMODEL_H
#define COMMANDMODEL_H


#include <QAbstractItemModel>
#include <QStringList>
#include <QMutex>
#include <commandpluginbase/command.h>
#include "actionlib_export.h"


class SIMONACTIONS_EXPORT CommandModel : public QAbstractItemModel {

Q_OBJECT

public slots:
	void updateCommands(CommandList *commands);

private:
	CommandList *commands;

	const QStringList getCategories(const CommandList *commands) const;

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int, Qt::Orientation orientation,
				int role = Qt::DisplayRole) const;

	QModelIndex index(int row, int column,
			const QModelIndex &parent = QModelIndex()) const;

	QModelIndex parent(const QModelIndex &index) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

public:
	CommandModel(CommandList *commands);
	~CommandModel();

};



#endif
