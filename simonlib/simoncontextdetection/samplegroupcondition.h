/*
 *   Copyright (C) 2012 Adam Nash <adam.t.nash@gmail.com>
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

#include <QObject>
#include "simoncontextdetection_export.h"
#include "condition.h"

class SIMONCONTEXTDETECTION_EXPORT SampleGroupCondition : public QObject
{
    Q_OBJECT
public:
    explicit SampleGroupCondition(QObject *parent = 0);

    QString getSampleGroup(int index);

    QStringList getSampleGroups();

    Condition* getSampleGroupCondition(int index);

    int getSampleGroupConditionCount();

    void addSampleGroupCondition(Condition* condition, QString sampleGroup, int index = -1);

    bool removeSampleGroupCondition(int index);

    bool changeSampleGroup(int index, QString);

    bool promoteCondition(int index);

    bool demoteCondition(int index);

private:
    /// A list of the currently active Acoustic Context condition objects in order of priority
    QList<Condition*> m_sampleGroupConditions;

    /// A hash table that contains the mapping of acoustic conditions to sample groups
    QList<QString> m_sampleGroups;

    /// A string containing the most recently determined sample group
    QString m_currentSampleGroup;

signals:
    /// Emitted when changes in the acoustic model conditions change the sample group
    void sampleGroupChanged(QString);

public slots:
    /// Checks for a change in sample group when the acoustic model conditions change
    void checkAcousticContext();

    void saveSampleGroupContext();
    void loadSampleGroupContext();
};

#endif // SAMPLEGROUPCONDITION_H
