/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "voiceinterfacecommand.h"
#include "voiceinterfacecommandtemplate.h"
#include "commandmanager.h"
#include <QMetaObject>
#include <QDebug>

VoiceInterfaceCommand::VoiceInterfaceCommand(CommandManager *parentManager, const QString& trigger, const QString& iconSrc,
const QString& description, const QString& id, QList<int> states, int newState,const QString& visibleTrigger,
bool showIcon, bool announce) :
Command(trigger, iconSrc, description, states, newState, announce),
m_id(id),
m_visibleTrigger(visibleTrigger),
m_receiver(0),
m_showIcon(showIcon)
{
  setParent(parentManager);
}


//copy constructor
VoiceInterfaceCommand::VoiceInterfaceCommand(const VoiceInterfaceCommand& b) :
Command(b.getTrigger(), b.getIconSrc(), b.getDescription(), b.getBoundStates(), b.getTargetState(), b.getAnnounce()),
m_id(b.id()),
m_visibleTrigger(b.visibleTrigger()),
m_receiver(b.receiver()),
m_showIcon(b.showIcon())
{
  setParent(b.parent());
}


VoiceInterfaceCommand::VoiceInterfaceCommand(CommandManager *parentManager, VoiceInterfaceCommandTemplate *tem) :
Command(tem->actionName(), tem->icon(), tem->description(), tem->states(), tem->newState(), tem->announce()),
m_id(tem->id()),
m_visibleTrigger(tem->defaultVisibleTrigger()),
m_receiver(0),
m_showIcon(tem->showIcon())
{
  setParent(parentManager);
}


void VoiceInterfaceCommand::assignAction(CommandManager *parentManager, QObject *receiver, const QString& slot)
{
  setParent(parentManager);
  m_receiver = receiver;
  m_slot = slot;
}


bool VoiceInterfaceCommand::triggerPrivate(int *status)
{
  Q_UNUSED(status);

  if (m_receiver == 0) return false;

  //Queued connection:
  // -: Cannot be used with types that qt doesn't know about. You need to call qRegisterMetaType
  //    If you want to invoke slots that use custom types. Refer to the qt documentation
  //    for more info
  // +: Works in multithreaded environments
  QByteArray slotName = m_slot.toAscii();
  qDebug() << "Executing slot: " << slotName << " on object " << m_receiver;
  
  QStringList args = currentArguments();

  QMetaObject::invokeMethod(m_receiver, slotName.data(), Qt::AutoConnection,
	    (currentArguments().count() >= 1) ? Q_ARG(QString, currentArguments()[0]) : QGenericArgument(),
	    (currentArguments().count() >= 2) ? Q_ARG(QString, currentArguments()[1]) : QGenericArgument(),
	    (currentArguments().count() >= 3) ? Q_ARG(QString, currentArguments()[2]) : QGenericArgument(),
	    (currentArguments().count() >= 4) ? Q_ARG(QString, currentArguments()[3]) : QGenericArgument(),
	    (currentArguments().count() >= 5) ? Q_ARG(QString, currentArguments()[4]) : QGenericArgument(),
	    (currentArguments().count() >= 6) ? Q_ARG(QString, currentArguments()[5]) : QGenericArgument(),
	    (currentArguments().count() >= 7) ? Q_ARG(QString, currentArguments()[6]) : QGenericArgument(),
	    (currentArguments().count() >= 8) ? Q_ARG(QString, currentArguments()[7]) : QGenericArgument(),
	    (currentArguments().count() >= 9) ? Q_ARG(QString, currentArguments()[8]) : QGenericArgument(),
	    (currentArguments().count() >= 10) ? Q_ARG(QString, currentArguments()[9]) : QGenericArgument()
  );

  return true;
}


const QMap<QString,QVariant> VoiceInterfaceCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;

  out.insert(i18n("Visible trigger"), m_visibleTrigger);
  out.insert(i18n("Show icon"), 
             m_showIcon ? i18nc("Possible answer to the question: Should we show an icon?", "Yes") : 
	                  i18nc("Possible answer to the question: Should we show an icon?", "No"));
  return out;
}


bool VoiceInterfaceCommand::deSerializePrivate(const QDomElement& element)
{
  if (element.isNull()) return false;

  QDomElement idElement = element.firstChildElement("id");
  QDomElement visibleTriggerElement = element.firstChildElement("visibleTrigger");
  QDomElement showIconElement = element.firstChildElement("showIcon");
  m_id = idElement.text();
  m_visibleTrigger = visibleTriggerElement.text();
  m_showIcon = (showIconElement.text().toInt() == 1);

  return true;
}


VoiceInterfaceCommand* VoiceInterfaceCommand::createInstance(const QDomElement& element)
{
                                                  //private constructor
  VoiceInterfaceCommand *guiInterfaceCommand = new VoiceInterfaceCommand();
  if (!guiInterfaceCommand->deSerialize(element)) {
    delete guiInterfaceCommand;
    return 0;
  }

  return guiInterfaceCommand;
}


QDomElement VoiceInterfaceCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  QDomElement idElement = doc->createElement("id");
  idElement.appendChild(doc->createTextNode(m_id));

  QDomElement visibleTriggerElement = doc->createElement("visibleTrigger");
  visibleTriggerElement.appendChild(doc->createTextNode(m_visibleTrigger));

  QDomElement showIconElement = doc->createElement("showIcon");
  showIconElement.appendChild(doc->createTextNode(m_showIcon ? "1" : "0"));

  commandElem.appendChild(idElement);
  commandElem.appendChild(visibleTriggerElement);
  commandElem.appendChild(showIconElement);

  return commandElem;
}


const QString VoiceInterfaceCommand::staticCategoryText()
{
  return i18n("Voice Interface");
}


const QIcon VoiceInterfaceCommand::staticCategoryIcon()
{
  return QIcon::fromTheme("text-speak");
}


const QIcon VoiceInterfaceCommand::getCategoryIcon() const
{
  return parent()->icon();
}


const QString VoiceInterfaceCommand::getCategoryText() const
{
  return parent()->name();
}


VoiceInterfaceCommand::~VoiceInterfaceCommand()
{
}
