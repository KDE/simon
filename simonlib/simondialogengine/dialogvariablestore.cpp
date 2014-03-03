/*
 *   Copyright (C) 2014 Benjamin Bowley-Bryant <benbb@utexas.edu>
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

#include <QList>
#include <QString>
#include <QMap>
#include "dialogvariablestore.h"
#include "dialogvariable.h"


QString DialogVariableStore::getValue(const QString& name) const
{
  return this->dialogVariables[name]->getValue();
}

void DialogVariableStore::removeVariable(const QString& name)
{
  Q_ASSERT(this->dialogVariables.contains(name));
  delete(this->dialogVariables.take(name));
}

