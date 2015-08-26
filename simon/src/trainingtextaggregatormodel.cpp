/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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


#include "trainingtextaggregatormodel.h"
#include <simonscenarios/trainingtextcollection.h>

TrainingTextAggregatorModel::TrainingTextAggregatorModel ( QObject* parent ) : QAbstractProxyModel ( parent )
{
}

void TrainingTextAggregatorModel::setCollections ( QList< TrainingTextCollection* > collections )
{
  m_collections = collections;
  
  foreach (TrainingTextCollection *collection, m_collections) {
    connect(collection, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(changed()));
    connect(collection, SIGNAL(rowsInserted (QModelIndex, int, int)), this, SLOT(changed()));
    connect(collection, SIGNAL(rowsRemoved (QModelIndex, int, int)), this, SLOT(changed()));
  }
  reset();
}

void TrainingTextAggregatorModel::changed()
{
  reset();
}

QModelIndex TrainingTextAggregatorModel::mapFromSource ( const QModelIndex& sourceIndex ) const
{
  TrainingText *text = static_cast<TrainingText*>(sourceIndex.internalPointer());
  
  int row = findRowInProxy(text);
  
  if (row == -1)
    return QModelIndex();
  
  return createIndex(row, sourceIndex.column(), text);
}

QItemSelection TrainingTextAggregatorModel::mapSelectionFromSource ( const QItemSelection& selection ) const
{
  if (selection.empty()) return QItemSelection();
  
  QItemSelectionRange range = selection.first();
  return QItemSelection(mapFromSource(range.topLeft()), mapFromSource(range.bottomRight()));
}

QItemSelection TrainingTextAggregatorModel::mapSelectionToSource ( const QItemSelection& selection ) const
{
  if (selection.empty()) return QItemSelection();
  
  QItemSelectionRange range = selection.first();
  return QItemSelection(mapToSource(range.topLeft()), mapToSource(range.bottomRight()));
}

QModelIndex TrainingTextAggregatorModel::mapToSource ( const QModelIndex& proxyIndex ) const
{
  TrainingText *text = static_cast<TrainingText*>(proxyIndex.internalPointer());
  
  foreach (TrainingTextCollection *collection, m_collections) {
    int currentRow = 0;
    foreach (TrainingText *thisText, collection->texts()) {
      if (text == thisText)
        return collection->index(currentRow, proxyIndex.column());
      ++currentRow;
    }
  }
  return QModelIndex();
}

int TrainingTextAggregatorModel::findRowInProxy ( TrainingText* text ) const
{
  int currentRow = 0;
  bool found = false;
  //find text in list of collections and determine the place there
  foreach (TrainingTextCollection *collection, m_collections) {
    foreach (TrainingText *thisText, collection->texts()) {
      if (thisText == text) {
        //found the text!
        found = true;
        break;
      } else
        ++currentRow;
    }
    if (found) break;
  }
  return found ? currentRow : -1;
}

QVariant TrainingTextAggregatorModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
  if (m_collections.isEmpty()) return QVariant();
  return m_collections.first()->headerData(section, orientation, role);
}

QModelIndex TrainingTextAggregatorModel::index ( int row, int column, const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  int requestedRow = row;
  TrainingText *text = 0;
  foreach (TrainingTextCollection *collection, m_collections) {
    int count = collection->rowCount() ;
    if (row >= count) {
      row -= count;
    } else {
      if (count > 0) {
        text = collection->texts()[row];
        break;
      }
    }
  }
  return createIndex(requestedRow, column, text);
}

int TrainingTextAggregatorModel::columnCount ( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  if (m_collections.isEmpty())
    return 0;
  return m_collections.first()->columnCount();
}
 
int TrainingTextAggregatorModel::rowCount ( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  int count = 0;
  foreach (TrainingTextCollection *collection, m_collections)
    count += collection->rowCount();
  return count;
}

QModelIndex TrainingTextAggregatorModel::parent ( const QModelIndex& child ) const
{
  Q_UNUSED(child);
  return QModelIndex();
}

QVariant TrainingTextAggregatorModel::data ( const QModelIndex& proxyIndex, int role ) const
{
  return mapToSource(proxyIndex).data(role);
}

Qt::ItemFlags TrainingTextAggregatorModel::flags ( const QModelIndex& index ) const
{
  return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
  
//   return QAbstractProxyModel::flags(index);
//   return mapToSource(index).flags();
}
