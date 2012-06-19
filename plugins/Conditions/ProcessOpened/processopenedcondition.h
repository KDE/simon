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

#ifndef PROCESSOPENEDCONDITION_H
#define PROCESSOPENEDCONDITION_H

/** \file processopenedcondition.h
 * \brief The file containing the ProcessOpenedCondition baseclass header.
 */

#include <simoncontextdetection/condition.h>
#include <simoncontextdetection/processinfo.h>
#include <simoncontextdetection/simoncontextdetection_export.h>
#include <simoncontextdetection/compoundcondition.h>

/**
 *	@class ProcessOpenedCondition
 *	@brief The ProcessOpenedCondition class is a condition plugin that monitors whether or not a specified process is running
 *
 *      The ProcessOpenedCondition will be satisfied whenever the process name that it is monitoring (\ref m_processName) is
 *      found among the list of running processes as determined by the ProcessInfo singleton.  Upon initialization, all
 *      running processes are checked, and afterwards, the added and removed processes are checked by connecting the
 *      ProcessInfo signals \ref ProcessInfo::processAdded(QString) and \ref ProcessInfo::processRemoved(QString) to the
 *      ProcessOpenedCondition slots \ref checkAddedProcess(QString) and \ref checkRemovedProcess(QString) respectively.
 *
 *      Just like any other condition, the ProcessOpenedCondition will accordingly update its \ref Condition::m_satisfied value and then emit its \ref Condition::conditionChanged()
 *      signal whenever it becomes newly satisfied or unsatisfied.
 *
 *      \sa Condition, ProcessInfo, CreateProcessOpenedConditionWidget
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class ProcessOpenedCondition : public Condition
{
    Q_OBJECT
public:
    explicit ProcessOpenedCondition(QObject *parent, const QVariantList& args);

    QString getName() {return m_processName;}

    virtual QString name();

    virtual CreateConditionWidget* getCreateConditionWidget(CompoundCondition *compoundCondition, QWidget *parent);

private:
    bool privateDeSerialize(QDomElement elem);
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);
    int m_openedInstances;

    QString m_processName;

signals:

public slots:
    void checkRemovedProcess(QString processName);
    void checkAddedProcess(QString processName);
};

#endif // PROCESSOPENEDCONDITION_H
