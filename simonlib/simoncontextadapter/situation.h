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


#ifndef SITUATION_H
#define SITUATION_H

#include "simoncontextadapter_export.h"
#include <QHash>

class SIMONCONTEXTADAPTER_EXPORT Situation
{
public:
  Situation(const QStringList& deactivatedScenarios, const QStringList& deactivatedSampleGroups);
  Situation();
  
  QString id() const;
  QStringList deactivatedScenarios() const { return m_deactivatedScenarios; }
  QStringList deactivatedSampleGroups() const { return m_deactivatedSampleGroups; }
  void setDeactivatedScenarios(const QStringList& deactivatedScenarios) { m_deactivatedScenarios = deactivatedScenarios; }
  void setDeactivatedSampleGroups(const QStringList& deactivatedSampleGroups) { m_deactivatedSampleGroups = deactivatedSampleGroups; }
  
  static QString id(const QStringList& deactivatedScenarios, const QStringList& deactivatedSampleGroups);
  
  bool operator==(const Situation& other) const;
  
private:
  QStringList m_deactivatedScenarios;
  QStringList m_deactivatedSampleGroups;
};

uint qHash(const Situation& s);

#endif // SITUATION_H
