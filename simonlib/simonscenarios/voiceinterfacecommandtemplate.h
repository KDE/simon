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

#ifndef SIMON_VOICEINTERFACECOMMANDTEMPLATE_H_71E1009585854F21AA6058993C51FCEE
#define SIMON_VOICEINTERFACECOMMANDTEMPLATE_H_71E1009585854F21AA6058993C51FCEE

#include "simonmodelmanagement_export.h"
#include <QString>

class MODELMANAGEMENT_EXPORT  VoiceInterfaceCommandTemplate
{
  private:
    QString m_id;
    QString m_icon;
    QString m_actionName;
    QString m_defaultVisibleTrigger;
    QString m_description;
    QList<int> m_states;
    int m_newState;
    bool m_announce;
    bool m_showIcon;

    QObject *m_receiver;
    QString m_slot;

  public:
    VoiceInterfaceCommandTemplate(const QString& id, const QString& actionName,
      const QString& icon, const QString& description, int state,
      int newState, bool announce=false, bool showIcon=true,
      QString defaultVisibleTrigger=QString());
    VoiceInterfaceCommandTemplate(const QString& id, const QString& actionName,
      const QString& icon, const QString& description, QList<int> states,
      int newState, bool announce=false, bool showIcon=true,
      QString defaultVisibleTrigger=QString());

    void assignAction(QObject *receiver, const QString& slot);

    QString id() { return m_id; }
    QString actionName() { return m_actionName; }
    QString description() { return m_description; }
    QString icon() { return m_icon; }
    QObject* receiver() { return m_receiver; }
    QString slot() { return m_slot; }
    QList<int> states() { return m_states; }
    int newState() { return m_newState; }

    QString defaultVisibleTrigger() {
      return m_defaultVisibleTrigger.isNull() ? m_actionName : m_defaultVisibleTrigger;
    }

    bool showIcon() { return m_showIcon; }
    bool announce() { return m_announce; }

    ~VoiceInterfaceCommandTemplate();

};
#endif
