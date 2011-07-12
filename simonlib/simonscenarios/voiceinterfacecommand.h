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

#ifndef SIMON_VOICEINTERFACECOMMAND_H_7942FB19340C44C1BE53CEFCAEE2E74A
#define SIMON_VOICEINTERFACECOMMAND_H_7942FB19340C44C1BE53CEFCAEE2E74A

#include "simonmodelmanagement_export.h"
#include "command.h"
#include <QString>
#include <QDomElement>

class QObject;
class QDomDocument;
class CommandManager;
class VoiceInterfaceCommandTemplate;

class MODELMANAGEMENT_EXPORT  VoiceInterfaceCommand : public Command
{
  private:
    QString m_id;
    QString m_visibleTrigger;
    QObject *m_receiver;
    QString m_slot;
    bool m_showIcon;

    VoiceInterfaceCommand() {}
  public:
    VoiceInterfaceCommand(const VoiceInterfaceCommand& b);
    VoiceInterfaceCommand(CommandManager *parentManager, VoiceInterfaceCommandTemplate *tem);
    VoiceInterfaceCommand(CommandManager *parentManager, const QString& trigger,  const QString& iconSrc,
      const QString& description, const QString& id, QList<int> state, int newState,
      const QString& visibleTrigger, bool showIcon, bool announce);

    void assignAction(CommandManager *m_parentManager, QObject *receiver, const QString& slot);

    bool deSerializePrivate(const QDomElement&);
    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
    static VoiceInterfaceCommand* createInstance(const QDomElement& element);
    const QMap<QString,QVariant> getValueMapPrivate() const;

    bool triggerPrivate(int *status);

    QString id() const { return m_id; }
    QString visibleTrigger() const { return m_visibleTrigger; }
    QObject* receiver() const { return m_receiver; }
    bool showIcon() const { return m_showIcon; }

    virtual ~VoiceInterfaceCommand();

    static const QString staticCategoryText();
    static const KIcon staticCategoryIcon();

    const KIcon getCategoryIcon() const;
    const QString getCategoryText() const;

};
#endif
