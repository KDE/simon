/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "institutionmodel.h"
#include <sscobjects/institution.h>
#include <QtGlobal>
#include <KLocalizedString>

InstitutionModel::InstitutionModel(QList<Institution*> institutions, QObject *parent) : 
	QAbstractItemModel(parent),
	m_institutions(institutions)
{
}


/**
 * Replaces the data with the given one; The old list will be deleted;
 * This will reset the model to update all views accordingly
 */
void InstitutionModel::replaceData(QList<Institution*> newInstitutions)
{
	qDeleteAll(m_institutions);

	this->m_institutions = newInstitutions;
	reset();
}


QVariant InstitutionModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();

	if (role == Qt::DisplayRole) 
	{
		switch (index.column())
		{
			case 0:
				return m_institutions.at(index.row())->id();
			case 1:
				return m_institutions.at(index.row())->name();
		}
	}

	return QVariant();
}

Qt::ItemFlags InstitutionModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant InstitutionModel::headerData(int column, Qt::Orientation orientation,
			int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (column) {
			case 0:
				return i18n("Id");
			case 1:
				return i18n("Name");
		}
	}
	
	//default
	return QVariant();
}

QModelIndex InstitutionModel::parent(const QModelIndex &index) const
{
	Q_UNUSED(index);
	return QModelIndex();
}

int InstitutionModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return m_institutions.count();
	else return 0;
}

int InstitutionModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 2;
}

QModelIndex InstitutionModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent) || parent.isValid())
		return QModelIndex();

	return createIndex(row, column, m_institutions.at(row));
}


