/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SCENARIOOBJECT_H_10BF7730AC634828820C72F90E65AA5E
#define SIMON_SCENARIOOBJECT_H_10BF7730AC634828820C72F90E65AA5E
#include <QString>
#include <QDomElement>
#include "simonscenariobase_export.h"

class Scenario;

class SCENARIOBASE_EXPORT ScenarioObject
{

  protected:
    Scenario *parentScenario;
  public:

    ScenarioObject(Scenario *parentScenario)
    : parentScenario(parentScenario) {
    }

    virtual void assignParent(Scenario *parent) { parentScenario = parent; }

    virtual bool deSerialize(const QDomElement&)=0;
    virtual QDomElement serialize(QDomDocument *doc)=0;
    
    QString parentScenarioId() const;

    virtual ~ScenarioObject() {}

};
#endif
