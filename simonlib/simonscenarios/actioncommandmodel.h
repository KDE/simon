/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_ACTIONCOMMANDMODEL_H_3EBF59D79A3C46A1B84DE19FA1269213
#define SIMON_ACTIONCOMMANDMODEL_H_3EBF59D79A3C46A1B84DE19FA1269213
#include <QString>
#include <QAbstractItemModel>
#include "actioncollection.h"
#include "simonmodelmanagement_export.h"

class MODELMANAGEMENT_EXPORT ActionCommandModel : public QAbstractItemModel
{
  public:
    ActionCommandModel(ActionCollection *base);
    ~ActionCommandModel();

    void update();

  private:
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int, Qt::Orientation orientation,
      int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QList<Action*> m_actions;
    ActionCollection *ref;
};
#endif
