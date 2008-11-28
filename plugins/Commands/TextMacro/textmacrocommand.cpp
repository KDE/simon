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

#include "textmacrocommand.h"
#include <QObject>
#include <QCoreApplication>
#include <QVariant>
#include <KLocalizedString>
#include <eventsimulation/eventhandler.h>


const QString TextMacroCommand::staticCategoryText()
{
	return i18n("Text-Macro");
}

const QString TextMacroCommand::getCategoryText() const
{
	return TextMacroCommand::staticCategoryText();
}

const KIcon TextMacroCommand::staticCategoryIcon()
{
	return KIcon("format-text-bold");
}

const KIcon TextMacroCommand::getCategoryIcon() const
{
	return TextMacroCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> TextMacroCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(i18n("Text"), getText());
	return out;
}


bool TextMacroCommand::triggerPrivate()
{
	QCoreApplication::processEvents();
	EventHandler::getInstance()->sendWord(getText());
	return true;
}
