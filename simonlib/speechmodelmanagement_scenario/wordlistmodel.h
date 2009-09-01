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

#ifndef WORDLISTMODEL_H
#define WORDLISTMODEL_H


#include <QAbstractItemModel>
#include <QStringList>
#include <QBrush>
#include <speechmodelbase_scenario/word.h>
#include "simonmodelmanagement_export.h"


class MODELMANAGEMENT_EXPORT WordListModel : public QAbstractItemModel {

Q_OBJECT

public slots:
	void updateWordList(WordList *wordlist);

private:
	QBrush recogWeak, recogNone;
	WordList *words;

	void buildBrushes();

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
	WordListModel(WordList *words, QObject *parent=0);
	~WordListModel();

};



#endif
