/*
 *   Copyright (C) 2014 Ashish Madeti <ashishmadeti@gmail.com>
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

#include "mprisplayercommandmanager.h"
#include "mprisplayerconfiguration.h"
#include "mprisplayercommand.h"
#include "createmprisplayercommandwidget.h"

#include <KLocalizedString>

K_PLUGIN_FACTORY( MprisPlayerPluginFactory,
registerPlugin< MprisPlayerCommandManager >();
)

K_EXPORT_PLUGIN( MprisPlayerPluginFactory("simoncommandmprisplayer") )

MprisPlayerCommandManager::MprisPlayerCommandManager(QObject *parent, const QVariantList &args)
    : CommandManager((Scenario*) parent, args)
{
}

bool MprisPlayerCommandManager::shouldAcceptCommand(Command *command)
{
    return (dynamic_cast<MprisPlayerCommand*>(command) != 0);
}

const QString MprisPlayerCommandManager::preferredTrigger() const
{
    return QString();
}

const QString MprisPlayerCommandManager::iconSrc() const
{
    return "applications-multimedia";
}

const QString MprisPlayerCommandManager::name() const
{
    return i18n("Media Player Control");
}

CreateCommandWidget *MprisPlayerCommandManager::getCreateCommandWidget(QWidget *parent)
{
    return new CreateMprisPlayerCommandWidget(dynamic_cast<MprisPlayerConfiguration*>(config)->mediaPlayerServiceName(), this, parent);
}

bool MprisPlayerCommandManager::deSerializeConfig(const QDomElement &elem)
{
    config = new MprisPlayerConfiguration(parentScenario);
    config->deSerialize(elem);

    return true;
}

DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(MprisPlayerCommandManager, MprisPlayerCommand)

MprisPlayerCommandManager::~MprisPlayerCommandManager()
{
}
