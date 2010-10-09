/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_TESTRESULTMODEL_H_8163A007180343A3A027C402EDCE6C93
#define SIMON_TESTRESULTMODEL_H_8163A007180343A3A027C402EDCE6C93

#include "simonmodeltest_export.h"
#include <QList>
#include <QAbstractItemModel>

class TestResult;

class MODELTEST_EXPORT TestResultModel : public QAbstractItemModel
{
  private:
    QList<TestResult*> m_results;

  protected:
    QVariant headerData(int, Qt::Orientation orientation,
      int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  public:
    TestResultModel(QObject *parent=0);
    void setResults(QList<TestResult*>& results);
    void clear();

};

#endif

