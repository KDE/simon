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

#ifndef CONDITIONASSOCIATION_H
#define CONDITIONASSOCIATION_H

#include "condition.h"
#include "simoncontextdetection_export.h"

class SIMONCONTEXTDETECTION_EXPORT ConditionAssociation : public Condition
{
    Q_OBJECT
public:
    explicit ConditionAssociation(QObject *parent, const QVariantList &args);

    virtual QString name();

protected:
    QList<Condition*> m_conditions;
    bool privateDeSerialize(QDomElement elem);
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);
    QString m_pluginName;

signals:

public slots:
    virtual void evaluateConditions()=0;
};

#endif // CONDITIONASSOCIATION_H
