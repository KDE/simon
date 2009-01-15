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
#include "compositecommandmanager.h"
#include "xmlcompositecommand.h"
#include "compositecommand.h"
#include "createcompositecommandwidget.h"
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( CompositeCommandPluginFactory, 
			registerPlugin< CompositeCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( CompositeCommandPluginFactory("simoncompositecommand") )


CompositeCommandManager::CompositeCommandManager(QObject *parent, const QVariantList& args) :CommandManager(parent, args)  
{
	this->xmlCompositeCommand = new XMLCompositeCommand();
}

const KIcon CompositeCommandManager::icon() const
{
	return CompositeCommand::staticCategoryIcon();
}

bool CompositeCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<CompositeCommand*>(command))
	{
		this->commands->append(command);
		return save();
	}
	return false;
}

const QString CompositeCommandManager::name() const
{
	return CompositeCommand::staticCategoryText();
}


CreateCommandWidget* CompositeCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreateCompositeCommandWidget(parent);
}

bool CompositeCommandManager::load()
{
	QString commandPath = KStandardDirs::locate("appdata", "conf/composites.xml");
	Logger::log(i18n("[INF] Loading composite commands from %1", commandPath));

	bool ok = false;
	this->commands = xmlCompositeCommand->load(ok, commandPath);
	return ok;
}

bool CompositeCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/composites.xml");
	Logger::log(i18n("[INF] Saving composite commands to %1", commandPath));
	return xmlCompositeCommand->save(commands, commandPath);
}

CompositeCommandManager::~CompositeCommandManager()
{
	if (xmlCompositeCommand) xmlCompositeCommand->deleteLater();
}
