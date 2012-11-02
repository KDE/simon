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

#include "alarmscheduleitem.h"

#include "akonadiconfiguration.h"
#include "akonadicommandmanager.h"
#include <simondialogengine/dialogcommand.h>
#include <simondialogengine/dialogstate.h>
#include <simondialogengine/dialogtextparser.h>

#include <QList>
#include <kcalcore/event.h>
#include <KDebug>
#include "dialogrunner.h"
#include <simondialogengine/dialogtemplateoptions.h>
#include <simondialogengine/dialogboundvalues.h>

AlarmScheduleItem::AlarmScheduleItem(QSharedPointer< KCalCore::Event > event, KCalCore::Alarm::Ptr alarm,
      AkonadiConfiguration *config, AkonadiCommandManager *manager) : 
  ScheduleItem(event), m_alarmText(alarm->text()), m_eventTime(event->dtStart().dateTime()), m_eventLocation(event->location()),
  m_config(config), m_manager(manager)
{
  
}

bool AlarmScheduleItem::trigger()
{
  DialogTemplateOptions *templateOptions = m_config->getTemplateOptions();
  DialogBoundValues *boundValues = m_config->getBoundValues();
  DialogTextParser *parser = new DialogTextParser(templateOptions, boundValues);
  QList<DialogCommand*> commands;
  if (m_config->getShowDismiss()) {
    DialogCommand *c = new DialogCommand(m_config->dismissText(), "dialog-ok", i18n("Dismisses the reminder"), m_config->dismissText(),
				  true, false, false, 0, true, 2, false, QStringList(), QStringList());
    ((Command *) c)->setParent(m_manager);
    commands << c;
  }
  if (m_config->getShowDelay()) {
    DialogCommand *c = new DialogCommand(m_config->delayText(), "chronometer", i18n("Hides the reminder temporarily"), m_config->delayText(),
				  true, false, false, 0, true, 3, false, QStringList(), QStringList());
    ((Command *) c)->setParent(m_manager);
    commands << c;
  }
  
  QString text = m_config->dialogText();
  text.replace("%1", m_summary);
  text.replace("%2", KGlobal::locale()->formatDate(m_eventTime.date()));
  text.replace("%3", KGlobal::locale()->formatTime(m_eventTime.time()));
  text.replace("%4", m_eventLocation);

  DialogState *state = new DialogState(parser, "Name", text, false, true, commands, 0);
  state->setDisplayAvatar(m_config->getDisplayAvatar());
  state->setAvatar(m_config->getSelectedAvatar());
  
  DialogRunner *runner = new DialogRunner(m_config, parser, state);
  runner->run();
  kDebug() << "Displaying alarm for event: " << m_summary;
  return true;
}
