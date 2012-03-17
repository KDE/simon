/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#ifndef DBUSCONDITION_H
#define DBUSCONDITION_H

#include <simoncontextdetection/condition.h>
#include <simoncontextdetection/compoundcondition.h>

class QDBusInterface;

class DBusCondition : public Condition
{
    Q_OBJECT
public:
    explicit DBusCondition(QObject *parent, const QVariantList& args);
    virtual CreateConditionWidget* getCreateConditionWidget(CompoundCondition *compoundCondition, QWidget *parent);

    virtual QString name();
    
    QString serviceName() { return m_serviceName; }
    QString path() { return m_path; }
    QString interface() { return m_interface; }
    QString checkMethod() { return m_checkMethod; }
    QString notificationSignal() { return m_notificationSignal; }
    QString value() { return m_value; }

private:
    QString m_serviceName;
    QString m_path;
    QString m_interface;
    QString m_checkMethod;
    QString m_notificationSignal;
    QString m_value;
    
    QDBusInterface *m_connection;
    
    bool privateDeSerialize(QDomElement elem);
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);

private slots:
    void check();
};

#endif
