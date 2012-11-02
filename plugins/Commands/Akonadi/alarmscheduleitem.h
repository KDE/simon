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

#ifndef ALARMSCHEDULEITEM_H
#define ALARMSCHEDULEITEM_H

#include <scheduleitem.h>
#include <kcalcore/alarm.h>

class AkonadiConfiguration;
class AkonadiCommandManager;

class AlarmScheduleItem : public ScheduleItem
{
private:
  QString m_alarmText;
  QDateTime m_eventTime;
  QString m_eventLocation;
  AkonadiConfiguration *m_config;
  AkonadiCommandManager *m_manager;

public:
    AlarmScheduleItem(QSharedPointer< KCalCore::Event > event, KCalCore::Alarm::Ptr alarm,
      AkonadiConfiguration *config, AkonadiCommandManager *manager
    );
    
    bool trigger();
};

#endif // ALARMSCHEDULEITEM_H
