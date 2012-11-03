/*   Copyright (C) 2010 Grasch Peter <peter.grasch@bedahr.org>
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

#include "commandscheduleitem.h"
#include "akonadiconfiguration.h"
#include <simonactions/actionmanager.h>
#include <simonlogging/logger.h>
#include <kcalcore/event.h>
#include <KDebug>

CommandScheduleItem::CommandScheduleItem(QSharedPointer< KCalCore::Event > event,
  AkonadiConfiguration *config
) : ScheduleItem(event), m_config(config)
{
}

bool CommandScheduleItem::trigger()
{
      QString command = m_summary.remove(m_config->akonadiRequestPrefix()).trimmed();
      kDebug() << "Executing: " << command;
      QStringList parts = command.split("//");
      if (parts.count() != 2)
      {
	kWarning() << "Bad command format: " << command;
	Logger::log(i18n("Invalid akonadi command format: %1", command), Logger::Warning);
	return false;
      }
      return ActionManager::getInstance()->triggerCommand(parts[0], parts[1]);
}
