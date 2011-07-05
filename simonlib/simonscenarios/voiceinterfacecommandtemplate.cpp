/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "voiceinterfacecommandtemplate.h"

VoiceInterfaceCommandTemplate::VoiceInterfaceCommandTemplate(const QString& id, const QString& actionName,
const QString& icon, const QString& description, int state, int newState,
bool announce, bool showIcon,  QString defaultVisibleTrigger) :
m_id(id),
m_icon(icon),
m_actionName(actionName),
m_defaultVisibleTrigger(defaultVisibleTrigger),
m_description(description),
m_states(QList<int>() << state),
m_newState(newState),
m_announce(announce),
m_showIcon(showIcon)
{
}


VoiceInterfaceCommandTemplate::VoiceInterfaceCommandTemplate(const QString& id, const QString& actionName,
const QString& icon, const QString& description, QList<int> states, int newState,
bool announce, bool showIcon,  QString defaultVisibleTrigger) :
m_id(id),
m_icon(icon),
m_actionName(actionName),
m_defaultVisibleTrigger(defaultVisibleTrigger),
m_description(description),
m_states(states),
m_newState(newState),
m_announce(announce),
m_showIcon(showIcon)
{

}

void VoiceInterfaceCommandTemplate::assignAction(QObject *receiver, const QString& slot)
{
  m_receiver = receiver;
  m_slot = slot;
}


VoiceInterfaceCommandTemplate::~VoiceInterfaceCommandTemplate()
{
}
