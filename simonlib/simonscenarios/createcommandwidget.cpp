/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include "createcommandwidget.h"
#include "commandmanager.h"

bool CreateCommandWidget::addCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return m_manager->addCommand(createCommand(name, iconSrc, description));
}


void CreateCommandWidget::propagateCreatedCommand(Command *c)
{
  c->setParent(m_manager);
  emit commandSuggested(c);
}


bool CreateCommandWidget::isInstanceOfSameManager(Command *c)
{
  Q_ASSERT(c);
  return isInstanceOfSameManager(c->parent());
}

bool CreateCommandWidget::isInstanceOfSameManager(CommandManager *manager)
{
  return (m_manager == manager);
}

