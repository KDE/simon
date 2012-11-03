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

#ifndef SIMON_VERSIONNUMBER_H_DB0686F0049B4BE6ACCB2950DFEF971E
#define SIMON_VERSIONNUMBER_H_DB0686F0049B4BE6ACCB2950DFEF971E
#include <QString>
#include "simonscenariobase_export.h"
#include "scenarioobject.h"

class Scenario;

class SCENARIOBASE_EXPORT VersionNumber: public ScenarioObject
{

  private:
    int m_majorNumber;
    int m_minorNumber;
    int m_patchLevel;

  protected:
    VersionNumber(Scenario *parent);
    bool parseString(const QString& version);

  public:
    VersionNumber(Scenario *parent, const QString& version);
    virtual ~VersionNumber() {}

    bool isValid();
    QString toString();

    int majorNumber() const { return m_majorNumber; }
    int minorNumber() const { return m_minorNumber; }
    int patchLevel() const { return m_patchLevel; }

    bool operator<=(const VersionNumber& v2) const;
    bool operator<(const VersionNumber& v2) const;
    bool operator==(const VersionNumber& v2) const;
    bool operator>=(const VersionNumber& v2) const;

    static VersionNumber* createVersionNumber(Scenario* parent, const QDomElement&);
    bool deSerialize(const QDomElement&);
    QDomElement serialize(QDomDocument *doc);
};
#endif
