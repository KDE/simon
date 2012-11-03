/*
 *   Copyright (C) 2012 Adam Nash <adam.t.nash@gmail.com>
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

#ifndef SAMPLEGROUPCONDITION_H
#define SAMPLEGROUPCONDITION_H

#include "simoncontextdetection_export.h"
#include "condition.h"
#include <QPair>
#include <QAbstractItemModel>

class SIMONCONTEXTDETECTION_EXPORT SampleGroupCondition : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SampleGroupCondition(QObject *parent = 0);
    virtual ~SampleGroupCondition();

    void addSampleGroupCondition(Condition* condition, QString sampleGroup, bool holdChecks = false);
    bool removeSampleGroupCondition( int index, bool holdChecks = false );
    bool changeSampleGroup(int index, QString);
    QStringList getSampleGroups() const;
    
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    QList<QPair<Condition*, QString> > m_sampleGroupConditions;
    QStringList m_lastDeactivatedList;

signals:
    /// Emitted when changes in the acoustic model conditions change the sample group
    void sampleGroupChanged(QStringList);
    void changed();

public slots:
    /// Checks for a change in sample group when the acoustic model conditions change
    void checkAcousticContext();

    void saveSampleGroupContext();
    void loadSampleGroupContext();
    void updateCondition ( int row, Condition* edit );
    QStringList getDeactivatedSampleGroups();
};

#endif // SAMPLEGROUPCONDITION_H
