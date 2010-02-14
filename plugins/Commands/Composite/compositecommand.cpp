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

#include "compositecommand.h"
#include <simonactions/actionmanager.h>
#include <KIcon>
#include <KProcess>
#include <KLocalizedString>
#include <QObject>
#include <QVariant>
#include <QDomElement>
#include <QDomDocument>
#ifdef Q_OS_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

const QString CompositeCommand::staticCategoryText()
{
	return i18n("Composite");
}

const QString CompositeCommand::getCategoryText() const
{
	return CompositeCommand::staticCategoryText();
}

const KIcon CompositeCommand::staticCategoryIcon()
{
	return KIcon("view-choose");
}

const KIcon CompositeCommand::getCategoryIcon() const
{
	return CompositeCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> CompositeCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(i18n("Commands"), commands.join("\n"));
	return out;
	
}


QDomElement CompositeCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
	Q_ASSERT(commands.count() == commandTypes.count());

	QDomElement childCommandsElement = doc->createElement("childCommands");

	for (int i=0; i < commands.count(); i++) {
		QDomElement childComElement = doc->createElement("childCommand");
		QDomElement childTriggerElem = doc->createElement("trigger");
		QDomElement childCategoryElem = doc->createElement("category");

		childTriggerElem.appendChild(doc->createTextNode(commands[i]));
		childCategoryElem.appendChild(doc->createTextNode(commandTypes[i]));

		childComElement.appendChild(childTriggerElem);
		childComElement.appendChild(childCategoryElem);
		childCommandsElement.appendChild(childComElement);
	}
	commandElem.appendChild(childCommandsElement);
		
	return commandElem;
}


bool CompositeCommand::triggerPrivate(int *state)
{
	Q_UNUSED(state);
	Q_ASSERT(commands.count() == commandTypes.count());

	for (int i=0; i<commands.count();i++)
	{
		QString type = commandTypes[i];
		kDebug() << type << i18n("Delay");
		if (type==i18n("Delay"))
		{
			bool ok=true;
			kDebug() << commands[i];
			int amount = commands[i].toInt(&ok);
			if (!ok) {kDebug() << "Not ok"; continue;}
			kDebug() << "Sleeping: " << amount;
			#ifdef Q_OS_WIN32
			Sleep(amount);
			#else
			usleep(amount*1000);
			#endif
		} 
		else
			ActionManager::getInstance()->triggerCommand(commandTypes[i], commands[i]);
	}

	return true;
}


bool CompositeCommand::deSerializePrivate(const QDomElement& commandElem)
{
	QDomElement childCommandsElem = commandElem.firstChildElement("childCommands");
	if (childCommandsElem.isNull()) return false;

	commands.clear();
	commandTypes.clear();

	QDomElement childCommandElem = childCommandsElem.firstChildElement();

	while (!childCommandElem.isNull()) {
		QDomElement childCommandTriggerElem = childCommandElem.firstChildElement();
		QDomElement childCommandCategoryElem = childCommandTriggerElem.nextSiblingElement();
		commands << childCommandTriggerElem.text();
		commandTypes << childCommandCategoryElem.text();
		childCommandElem = childCommandElem.nextSiblingElement();
	}
	kDebug() << "Triggers: " << commands;
	kDebug() << "Categories: " << commandTypes;

	return true;
}

STATIC_CREATE_INSTANCE_C(CompositeCommand);


