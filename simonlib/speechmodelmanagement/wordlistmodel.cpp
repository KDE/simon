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

#include "wordlistmodel.h"

#include <QBrush>

#include <KLocalizedString>
#include <KColorScheme>
#include <KLocale>

WordListModel::WordListModel(WordList* WL, QObject* parent) : QAbstractItemModel(parent),
	words(WL)
{
	KLocale::setMainCatalog("simonlib");
	buildBrushes();
}

void WordListModel::buildBrushes()
{
	KColorScheme colorScheme(QPalette::Active);
	QColor negative = colorScheme.background(KColorScheme::NegativeBackground).color();

	recogNone = KColorScheme::shade(negative, KColorScheme::DarkShade);
	recogWeak = KColorScheme::shade(negative, KColorScheme::MidShade);

}

void WordListModel::updateWordList(WordList *words)
{
	buildBrushes();
	if (this->words)
		delete this->words;
	this->words = words;
	reset();
}

QVariant WordListModel::data(const QModelIndex &index, int role) const
{
	Q_ASSERT(words);
	if (!index.isValid()) return QVariant();

	Word *word = static_cast<Word*>(index.internalPointer());

	if (!word) {
		return QVariant();
	}
	if (role == Qt::DisplayRole) 
	{
		switch (index.column())
		{
			case 0:
				return word->getWord();
			case 1:
				return word->getPronunciation();
			case 2:
				return word->getTerminal();
			case 3:
				return word->getPropability();
		}
	} else if (role == Qt::BackgroundRole)
	{
		int propab = word->getPropability();
		switch (propab)
		{
			case 0:
				return recogNone;
			case 1:
				return recogWeak;
		}
	}
	
	return QVariant();
}

QModelIndex WordListModel::index(int row, int column,
		const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent) || parent.isValid())
		return QModelIndex();

	Q_ASSERT(words);

	return createIndex(row, column, (void*) &words->at(row));
}



Qt::ItemFlags WordListModel::flags(const QModelIndex &index) const
{
	Q_ASSERT(words);

	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant WordListModel::headerData(int column, Qt::Orientation orientation,
			int role) const
{
	Q_ASSERT(words);
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (column)
		{
			case 0:
				return i18n("Word");
			case 1:
				return i18n("Pronunciation");
			case 2:
				return i18n("Category");
			case 3:
				return i18n("Recognition Rate");
		}
	}
	
	//default
	return QVariant();
}


QModelIndex WordListModel::parent(const QModelIndex &index) const
{
	Q_UNUSED(index);
	return QModelIndex();
}

int WordListModel::rowCount(const QModelIndex &parent) const
{
	Q_ASSERT(words);

	if (!parent.isValid())
		return words->count();
	else return 0;
}

int WordListModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 4;
}


WordListModel::~WordListModel()
{
	
}
