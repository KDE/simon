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

#include "desktopgridcommandmanager.h"
#include <simoncommandpluginbase/simoncommandpluginbase_export.h>
#include "logging/logger.h"
#include "screengrid.h"
#include <KLocalizedString>
#include <KGenericFactory>
#include <KMessageBox>
#include <QDebug>

SIMONCOMMAND_EXPORT_PLUGIN(DesktopGridCommandManager)

DesktopGridCommandManager::DesktopGridCommandManager(QObject *parent, const QVariantList& args) : CommandManager(parent)
{
	Q_UNUSED(args);
}

const QString DesktopGridCommandManager::name() const
{
	i18n("Desktopgitter");
}

bool DesktopGridCommandManager::trigger(const QString& triggerName)
{
// 	if (triggerName != Settings::getS("Commands/DesktopGrid/Trigger")) return false;
	return false;

	Logger::log(i18n("[INF] Aktiviere Desktopgitter"));
	ScreenGrid *screenGrid = new ScreenGrid();
	screenGrid->show();
	return true;
}

bool DesktopGridCommandManager::load()
{
// 	KMessageBox::information(0, "du bist da king");
	return true;
}

bool DesktopGridCommandManager::save()
{
	return true;
}
