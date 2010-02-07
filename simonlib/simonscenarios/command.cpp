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

#include "command.h"
#include <simoninfo/simoninfo.h>
#include <QVariant>
#include <QDomElement>
#include <QDomDocument>
#include <KDebug>

bool Command::trigger(int* state)
{
	if (announce) {
		KIcon commandIcon = getIcon();
		SimonInfo::showMessage(getTrigger(), 2500, &commandIcon);
	}
	*state = switchToState;
	kDebug() << "Switchting to state: " << switchToState << getTrigger();
	return triggerPrivate(state);
}

const QMap<QString,QVariant> Command::getValueMap() const
{
	QMap<QString,QVariant> out = getValueMapPrivate();
 	out.insert(i18n("Description"), getDescription());
// 	out.insert(i18n("Name"), getTrigger());
// 	out.insert(i18n("Icon"), getIcon());
	return out;
}

QDomElement Command::serialize(QDomDocument *doc)
{
	QDomElement commandElem = doc->createElement("command");

	QDomElement name = doc->createElement("name");
	name.appendChild(doc->createTextNode(triggerName));
	QDomElement icon = doc->createElement("icon");
	icon.appendChild(doc->createTextNode(iconSrc));
	QDomElement descriptionElem = doc->createElement("description");
	descriptionElem.appendChild(doc->createTextNode(description));
	QDomElement stateElem = doc->createElement("state");
	stateElem.appendChild(doc->createTextNode(QString::number(boundState)));
	QDomElement newStateElem = doc->createElement("newState");
	newStateElem.appendChild(doc->createTextNode(QString::number(switchToState)));
	QDomElement announceElem = doc->createElement("announce");
	announceElem.appendChild(doc->createTextNode(announce ? "1" : "0"));

	commandElem.appendChild(name);
	commandElem.appendChild(icon);
	commandElem.appendChild(descriptionElem);
	commandElem.appendChild(stateElem);
	commandElem.appendChild(newStateElem);
	commandElem.appendChild(announceElem);

	return serializePrivate(doc, commandElem);
}

bool Command::matches(int commandManagerState, const QString& trigger)
{
	kDebug() << "Commandmanager state: " << commandManagerState << "Command bound to: " << boundState << trigger << getTrigger();
	if (commandManagerState != boundState)
		return false;

	return (trigger.compare(this->triggerName, Qt::CaseInsensitive) == 0);
}

bool Command::deSerialize(const QDomElement& elem)
{
	QDomElement name = elem.firstChildElement();
	QDomElement icon = name.nextSiblingElement();
	QDomElement descriptionElem = icon.nextSiblingElement();
	QDomElement stateElem = descriptionElem.nextSiblingElement();
	QDomElement newStateElem = stateElem.nextSiblingElement();
	QDomElement announceElem = newStateElem.nextSiblingElement();
	triggerName = name.text();
	iconSrc = icon.text();
	description = descriptionElem.text();
	boundState = stateElem.text().toInt();
	switchToState = newStateElem.text().toInt();
	announce = (announceElem.text().toInt() == 1);

	return deSerializePrivate(elem);
}

