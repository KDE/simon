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

#include "launchercommand.h"
#include <simonscenarios/commandlauncher.h>
#include <unistd.h>
#include <QObject>
#include <QVariant>
#include <QDomDocument>
#include <QDomElement>
#include <KIcon>
#include <KDebug>
#include <KLocalizedString>

LauncherCommand::LauncherCommand(CommandLauncher *launcher_)
: Command(QString(launcher_->pluginTrigger()+" "+launcher_->trigger()).trimmed(), 
		launcher_->iconSrc()), launcher(launcher_)
{
}

LauncherCommand::LauncherCommand(const QString& iconSrc, const QString& trigger) : Command(trigger, iconSrc),
	launcher(new CommandLauncher(iconSrc, trigger, ""))
{
}

const QString LauncherCommand::staticCategoryText()
{
	return i18n("Launcher");
}

const QString LauncherCommand::getCategoryText() const
{
	return LauncherCommand::staticCategoryText();
}

const KIcon LauncherCommand::staticCategoryIcon()
{
	return KIcon("fork");
}

const KIcon LauncherCommand::getCategoryIcon() const
{
	return LauncherCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> LauncherCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(i18n("Plugin"), launcher->plugin());
	out.insert(i18n("Plugin trigger"), launcher->pluginTrigger());
	out.insert(i18n("Launcher trigger"), launcher->trigger());
	out.insert(i18n("Description"), launcher->description());
	return out;
}

bool LauncherCommand::triggerPrivate()
{
	kDebug() << "Triggering launcher";
	return true;
}

QDomElement LauncherCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
	Q_UNUSED(doc);
//	QDomElement launcherElem = doc->createElement("launcher");
//	launcherElem.appendChild(doc->createTextNode(QString::number(launcher)));
//	
//	commandElem.appendChild(launcherElem);
		
	return commandElem;
}

