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

#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>
#include <QtXml/QDomElement>
#include "simoncontextdetection_export.h"

class SIMONCONTEXTDETECTION_EXPORT Condition : public QObject
{
    Q_OBJECT
public:
    explicit Condition(QObject *parent, const QVariantList &args);

    bool isSatisfied();

    QDomElement serialize(QDomDocument *doc);
    bool deSerialize(QDomElement elem);

protected:
    virtual bool privateDeSerialize(QDomElement elem)=0;
    virtual QDomElement privateSerialize(QDomDocument *doc, QDomElement elem)=0;

    bool m_satisfied;
    QString m_pluginName;

private:
    bool m_inverted;

signals:
    void conditionChanged();

public slots:

};

#endif // CONDITION_H
