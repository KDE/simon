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

#ifndef COMPOUNDCONDITION_H
#define COMPOUNDCONDITION_H

#include <QObject>
#include "condition.h"
#include "compoundconditionmodel.h"

class SIMONCONTEXTDETECTION_EXPORT CompoundCondition : public QObject
{
    Q_OBJECT
public:
    bool isSatisfied() {return m_satisfied;}
    QDomElement serialize(QDomDocument *doc);
    static CompoundCondition* createInstance(const QDomElement &elem);
    static QDomElement createEmpty(QDomDocument *doc);
    bool addCondition(Condition* condition);
    bool removeCondition(Condition* condition);
    QList<Condition*> getConditions() {return m_conditions;}
    CompoundConditionModel* getProxy() {return m_proxy;}

private:
    explicit CompoundCondition(QObject *parent = 0);
    bool deSerialize(const QDomElement &elem);

    bool m_satisfied;
    QList<Condition*> m_conditions;
    CompoundConditionModel *m_proxy;

signals:
    void conditionChanged(bool);
    void modified();

public slots:
    void evaluateConditions();
};

#endif // COMPOUNDCONDITION_H
