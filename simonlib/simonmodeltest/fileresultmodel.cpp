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

#include "fileresultmodel.h"

#include <QDir>
#include <QColor>

#include <KLocalizedString>
#include <KLocale>
#include <KColorScheme>

FileResultModel::FileResultModel(QHash<QString /*filename*/, TestResult*> testResults, QObject *parent) : QAbstractItemModel(parent)
{
	KLocale::setMainCatalog("simonlib");

	m_testResults = testResults;
//	m_results = results;
//	m_prompts = prompts;

	KColorScheme colorScheme(QPalette::Active);
	QColor negative = colorScheme.background(KColorScheme::NegativeBackground).color();
	recogWrong = KColorScheme::shade(negative, KColorScheme::MidShade);
}

QVariant FileResultModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();

	int row = index.row();

	QString path = m_testResults.keys().at(row);
	TestResult *t = m_testResults.value(path);

	if (role == Qt::DisplayRole) 
	{
		switch (index.column())
		{
			case 0:
				return path.mid(path.lastIndexOf(QDir::separator())+1);
			case 1:
				return t->getPrompt();
			case 2:
				if (t->getResults().isEmpty()) return "";
				return t->getResults().at(0).sentence();
			case 3:
				if (t->getResults().isEmpty()) return "";
				return QString("%1 \%").arg(t->getResults().at(0).averageConfidenceScore()*100.0f);
		}
	} else if (role == Qt::BackgroundRole) {
		if (t->getResults().isEmpty() || 
				(t->getResults().at(0).sentence().toUpper() != t->getPrompt()))
			return recogWrong;
	} else if (role == Qt::UserRole) {
		return path;
	}
	
	return QVariant();
}

QModelIndex FileResultModel::index(int row, int column,
		const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent) || parent.isValid())
		return QModelIndex();

	return createIndex(row, column);
}



Qt::ItemFlags FileResultModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

/*+-----------------------------------------------------------+*/
/*| Path | Expected result | Actual Result | Recognition Rate |*/
/*+-----------------------------------------------------------+*/

QVariant FileResultModel::headerData(int column, Qt::Orientation orientation,
			int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (column)
		{
			case 0:
				return i18n("Filename");
			case 1:
				return i18n("Expected result");
			case 2:
				return i18n("Actual result");
			case 3:
				return i18n("Recognition rate");
		}
	}
	
	//default
	return QVariant();
}


QModelIndex FileResultModel::parent(const QModelIndex &index) const
{
	Q_UNUSED(index);
	return QModelIndex();
}

int FileResultModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return m_testResults.keys().count();
	else return 0;
}

int FileResultModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 4;
}


FileResultModel::~FileResultModel()
{
}

