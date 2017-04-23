/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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
#include <QStringList>

class QDBusInterface;

class DBusCondition : public Condition
{
    Q_OBJECT
public:
    explicit DBusCondition(QObject *parent, const QVariantList& args);
    virtual CreateConditionWidget* getCreateConditionWidget(QWidget *parent);

    virtual QString name();
    
    QString serviceName() { return m_serviceName; }
    QString statePath() { return m_statePath; }
    QString stateInterface() { return m_stateInterface; }
    QString stateCheckMethod() { return m_stateCheckMethod; }
    QStringList stateArguments() { return m_stateArguments; }
    QString value() { return m_value; }
    
    QString notificationPath() { return m_notificationPath; }
    QString notificationInterface() { return m_notificationInterface; }
    QString notificationSignal() { return m_notificationSignal; }

private:
    QString m_serviceName;
    QString m_statePath;
    QString m_stateInterface;
    QString m_stateCheckMethod;
    QStringList m_stateArguments;
    QString m_value;
    
    QString m_notificationPath;
    QString m_notificationInterface;
    QString m_notificationSignal;
    
    QDBusInterface *m_stateConnection;
    
    bool privateDeSerialize(QDomElement elem);
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);

private slots:
    void check();
};

#endif
