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

#include "voiceinterfacecommand.h"
#include "voiceinterfacecommandtemplate.h"
#include "commandmanager.h"
#include <QMetaObject>
#include <KDebug>

VoiceInterfaceCommand::VoiceInterfaceCommand(CommandManager *parentManager, const QString& trigger, const QString& iconSrc, 
			const QString& description, const QString& id, int state, int newState,const QString& visibleTrigger, 
			bool showIcon, bool announce) :
	Command(trigger, iconSrc, description, state, newState, announce),
	m_parentManager(parentManager),
	m_id(id), 
	m_visibleTrigger(visibleTrigger),
	m_receiver(NULL),
	m_showIcon(showIcon)
{
}

//copy constructor
VoiceInterfaceCommand::VoiceInterfaceCommand(const VoiceInterfaceCommand& b) :
	Command(b.getTrigger(), b.getIconSrc(), b.getDescription(), b.getBoundState(), b.getTargetState(), b.getAnnounce()),
	m_parentManager(b.getParentManager()),
	m_id(b.id()),
	m_visibleTrigger(b.visibleTrigger()),
	m_receiver(b.receiver()),
	m_showIcon(b.showIcon())
{
}

VoiceInterfaceCommand::VoiceInterfaceCommand(CommandManager *parentManager, VoiceInterfaceCommandTemplate *tem) :
	Command(tem->actionName(), tem->icon(), tem->description(), tem->state(), tem->newState(), tem->announce()),
	m_parentManager(parentManager),
	m_id(tem->id()),
	m_visibleTrigger(tem->defaultVisibleTrigger()),
	m_receiver(NULL),
	m_showIcon(tem->showIcon())
{
}

void VoiceInterfaceCommand::assignAction(CommandManager *parentManager, QObject *receiver, const QString& slot)
{
	m_parentManager = parentManager;
	m_receiver = receiver;
	m_slot = slot;
}

bool VoiceInterfaceCommand::triggerPrivate(int *status)
{
	Q_UNUSED(status);

	if (m_receiver == NULL) return false;

	//Queued connection:
	// -: Cannot be used with types that qt doesn't know about. You need to call qRegisterMetaType
	//    If you want to invoke slots that use custom types. Refer to the qt documentation
	//    for more info
	// +: Works in multithreaded environments
	QByteArray slotName = m_slot.toAscii();
	kDebug() << "Executing slot: " << slotName << " on object " << m_receiver;
	QMetaObject::invokeMethod(m_receiver, slotName.data(), Qt::QueuedConnection);

	return true;
}

const QMap<QString,QVariant> VoiceInterfaceCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;

	out.insert(i18n("Visible trigger"), m_visibleTrigger);
	out.insert(i18n("Show icon"), m_showIcon ? i18n("Yes") : i18n("No"));
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
	VoiceInterfaceCommand *guiInterfaceCommand = new VoiceInterfaceCommand(); //private constructor
	if (!guiInterfaceCommand->deSerialize(element))
	{
		delete guiInterfaceCommand;
		return NULL;
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

const KIcon VoiceInterfaceCommand::staticCategoryIcon()
{
	return KIcon("text-speak");
}


const KIcon VoiceInterfaceCommand::getCategoryIcon() const
{
	return m_parentManager->icon();
}

const QString VoiceInterfaceCommand::getCategoryText() const
{
	return m_parentManager->name();
}

VoiceInterfaceCommand::~VoiceInterfaceCommand()
{
}

