/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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


#ifndef TRAININGTEXTAGGREGATORMODEL_H
#define TRAININGTEXTAGGREGATORMODEL_H

#include <QAbstractProxyModel>
#include <QList>

class TrainingTextCollection;
class TrainingText;

class TrainingTextAggregatorModel : public QAbstractProxyModel
{
  Q_OBJECT
public:
  TrainingTextAggregatorModel ( QObject* parent = 0 );
  virtual QModelIndex mapFromSource ( const QModelIndex& sourceIndex ) const;
  virtual QModelIndex mapToSource ( const QModelIndex& proxyIndex ) const;
  virtual int columnCount ( const QModelIndex& parent = QModelIndex() ) const;
  virtual int rowCount ( const QModelIndex& parent = QModelIndex() ) const;
  virtual QModelIndex parent ( const QModelIndex& child ) const;
  virtual QModelIndex index ( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
  
  void setCollections(QList<TrainingTextCollection*> collections);
  
  virtual QVariant data ( const QModelIndex& proxyIndex, int role = Qt::DisplayRole ) const;
  virtual Qt::ItemFlags flags ( const QModelIndex& index ) const;
  
  virtual QItemSelection mapSelectionFromSource ( const QItemSelection& selection ) const;
  virtual QItemSelection mapSelectionToSource ( const QItemSelection& selection ) const;
  virtual QVariant headerData ( int section, Qt::Orientation orientation, int role ) const;
  
private slots:
  void changed();
  
private:
  QList<TrainingTextCollection*> m_collections;
  
  int findRowInProxy(TrainingText* text) const;
};

#endif // TRAININGTEXTAGGREGATORMODEL_H
