/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SCENARIOOFFERUI_H_232B26BFFBAC4D88A29901748E1EB441
#define SIMON_SCENARIOOFFERUI_H_232B26BFFBAC4D88A29901748E1EB441

#include "simonscenarios_export.h"
#include <QList>

class Author;

class SIMONSCENARIOS_EXPORT ScenarioOfferUi
{
public:
  virtual bool askToAcceptScenario(const QString& requester, const QString& name, const QList<Author*> authors) const = 0;
  virtual ~ScenarioOfferUi() {}
};
#endif
