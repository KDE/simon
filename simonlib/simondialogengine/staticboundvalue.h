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

#ifndef SIMON_STATICBOUNDVALUE_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_STATICBOUNDVALUE_H_4B4956DCAE204C49977297D20CB81F09

#include "boundvalue.h"
#include "simondialogengine_export.h"
#include <QVariant>
#include <QString>

class SIMONDIALOGENGINE_EXPORT StaticBoundValue : public BoundValue
{
  private:
    QVariant m_value;

  protected:
    bool deSerialize(const QDomElement& elem);

  public:
    StaticBoundValue(const QString& name);
    StaticBoundValue(const QString& name, const QVariant& value);

    QString getTypeName();

    QVariant getValue();
    QString getValueDescription();

    bool serializePrivate(QDomDocument *doc, QDomElement& elem, int& id);
};

#endif


