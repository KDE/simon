/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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


#ifndef SIMON_BOUNDVALUE_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_BOUNDVALUE_H_4B4956DCAE204C49977297D20CB81F09

#include "simondialogengine_export.h"
#include <QVariant>
#include <QString>

class QDomElement;
class QDomDocument;

class SIMONDIALOGENGINE_EXPORT BoundValue
{
  private:
    QString m_name;

  protected:
    BoundValue(const QString& name) : m_name(name) {}

    virtual bool deSerialize(const QDomElement& elem)=0;
    virtual bool serializePrivate(QDomDocument *doc, QDomElement& elem, int& id)=0;

  public:
    virtual ~BoundValue() {}

    virtual QVariant getValue()=0;
    virtual QString getValueDescription()=0;

    static BoundValue* createInstance(const QDomElement& elem);

    virtual QString getTypeName()=0;

    QString getName() { return m_name; }

    QDomElement serialize(QDomDocument *doc);

};

#endif

