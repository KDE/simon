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

#include "shortcutcommand.h"
#include <QObject>
#include <QVariant>
#include <QDomDocument>
#include <QDomElement>
#include <KLocalizedString>
#include <eventsimulation/eventhandler.h>

const QString ShortcutCommand::staticCategoryText()
{
	return i18n("Shortcut");
}

const QString ShortcutCommand::getCategoryText() const
{
	return ShortcutCommand::staticCategoryText();
}

const KIcon ShortcutCommand::staticCategoryIcon()
{
	return KIcon("go-jump-locationbar");
}

const KIcon ShortcutCommand::getCategoryIcon() const
{
	return ShortcutCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> ShortcutCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;

	out.insert(i18n("Shortcut"), shortcut.toString());
	return out;
}

bool ShortcutCommand::triggerPrivate()
{
	EventHandler::getInstance()->sendShortcut(getShortcut());
	return true;
}

QDomElement ShortcutCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
	QDomElement shortcutElem = doc->createElement("shortcut");
	shortcutElem.appendChild(doc->createTextNode(shortcut.toString()));

	commandElem.appendChild(shortcutElem);
		
	return commandElem;
}

