/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#ifndef SCENARIOTREEMODEL_H
#define SCENARIOTREEMODEL_H

#include "simonmodelmanagement_export.h"
#include <QAbstractItemModel>

class Scenario;

class MODELMANAGEMENT_EXPORT ScenarioTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ScenarioTreeModel(QObject *parent = 0);
    ~ScenarioTreeModel();

    Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;
    QVariant headerData(int column, Qt::Orientation orientation, int role) const;
    QModelIndex index (int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent (const QModelIndex & index) const;
    int rowCount (const QModelIndex & parent = QModelIndex()) const;
    int columnCount (const QModelIndex & parent = QModelIndex()) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

    void setRootScenario(Scenario* root);

private:
    Scenario* m_rootScenario;

signals:

public slots:

};

#endif // SCENARIOTREEMODEL_H
