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

#ifndef SIMON_PLASMABOUNDVALUE_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_PLASMABOUNDVALUE_H_4B4956DCAE204C49977297D20CB81F09

#include "boundvalue.h"
#include "simondialogengine_export.h"
#include <QVariant>
#include <QString>

namespace Plasma
{
  class DataEngine;
}

class SIMONDIALOGENGINE_EXPORT PlasmaBoundValue : public BoundValue
{
  private:
    Plasma::DataEngine *m_currentEngine;
    QString m_dataEngine;
    QString m_dataEngineName;
    QString m_dataSource;
    QString m_key;

    void initEngine();

  protected:
    bool deSerialize(const QDomElement& elem);

  public:
    PlasmaBoundValue(const QString& name);
    PlasmaBoundValue(const QString& name, const QString& dataEngine, const QString& dataEngineName, 
        const QString& dataSource, const QString& key);
    ~PlasmaBoundValue();

    QString getTypeName();

    QString getDataEngine() { return m_dataEngine; }
    QString getDataEngineName() { return m_dataEngineName; }
    QString getDataSource() { return m_dataSource; }
    QString getKey() { return m_key; }

    QVariant getValue();
    QString getValueDescription();

    bool serializePrivate(QDomDocument *doc, QDomElement& elem, int& id);
};

#endif




