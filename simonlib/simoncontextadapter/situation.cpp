/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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


#include "situation.h"
#include <QHash>

Situation::Situation ( const QStringList& deactivatedScenarios, const QStringList& deactivatedSampleGroups ) : 
      m_deactivatedScenarios(deactivatedScenarios), m_deactivatedSampleGroups(deactivatedSampleGroups)
{
}

QString Situation::id() const
{
  return id(m_deactivatedScenarios, m_deactivatedSampleGroups);
}

QString Situation::id ( const QStringList& deactivatedScenarios, const QStringList& deactivatedSampleGroups )
{
  if (deactivatedScenarios.isEmpty() && deactivatedSampleGroups.isEmpty())
    return "active"; //hardcode stock id
    
  return QString::number(qHash(deactivatedScenarios.join("|^|")) ^ qHash(deactivatedSampleGroups.join("|^|")));
}

bool Situation::operator== ( const Situation& other ) const
{
  return (other.id() == id());
}


uint qHash(const Situation& s)
{
  return qHash(s.id());
}