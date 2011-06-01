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

#include "simoncontextdetection/condition.h"
#include "simoncontextdetection/processinfo.h"
#include "simoncontextdetection/simoncontextdetection_export.h"

class SIMONCONTEXTDETECTION_EXPORT ProcessOpenedCondition : public Condition
{
    Q_OBJECT
public:
    explicit ProcessOpenedCondition(QObject *parent, const QVariantList& args);

    QString getName() {return m_processName;}

private:
    bool privateDeSerialize(QDomElement elem);
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);
    int m_openedInstances;

    QString m_processName;
    QString m_pluginName;

signals:

public slots:
    void checkRemovedProcess(QString name);
    void checkAddedProcess(QString name);
};

#endif // PROCESSOPENEDCONDITION_H
