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

#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include <QObject>
#include <QtXml/QDomElement>
#include <QHash>
#include "compoundcondition.h"
#include "simoncontextdetection_export.h"

class SIMONCONTEXTDETECTION_EXPORT ContextManager : public QObject
{
    Q_OBJECT
public:
    static ContextManager* instance();
    ~ContextManager();

    Condition* getCondition(const QDomElement &elem);
    
    void test();

private:
    explicit ContextManager(QObject *parent = 0);
    
    static ContextManager* m_instance;
  
    QHash<QString, Condition*> m_conditionLookup;
    QList<Condition*> m_conditions;

signals:

public slots:
};

#endif // CONTEXTMANAGER_H
