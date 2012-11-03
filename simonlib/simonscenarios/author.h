/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_AUTHOR_H_22406693285645508C8628A3575126FD
#define SIMON_AUTHOR_H_22406693285645508C8628A3575126FD

#include "simonmodelmanagement_export.h"
#include <simonscenariobase/scenarioobject.h>
#include <QString>

class Scenario;

class MODELMANAGEMENT_EXPORT Author: public ScenarioObject
{

  private:
    QString m_name;
    QString m_contact;

  protected:
    Author(Scenario *parent);

  public:
    Author(Scenario *parent, const QString& name, const QString& contact);

    QString name() const { return m_name; }
    QString contact() const { return m_contact; }

    static Author* createAuthor(Scenario *parent, const QDomElement&);

    bool deSerialize(const QDomElement&);
    QDomElement serialize(QDomDocument *doc);
};
#endif
