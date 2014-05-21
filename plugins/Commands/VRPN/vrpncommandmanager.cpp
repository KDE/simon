/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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
#include "vrpncommandmanager.h"
#include "vrpnconfiguration.h"
#include "vrpncommand.h"
#include "createvrpncommandwidget.h"
#include <eventsimulation/eventhandler.h>
#include <simonactions/actionmanager.h>
#include <KLocalizedString>
#include <KAction>

K_PLUGIN_FACTORY( VRPNCommandPluginFactory,
registerPlugin< VRPNCommandManager >();
)

K_EXPORT_PLUGIN( VRPNCommandPluginFactory("simonvrpncommand") )

VRPNCommandManager::VRPNCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args)
{
}

bool VRPNCommandManager::shouldAcceptCommand(Command *command)
{
  return (dynamic_cast<VRPNCommand*>(command) != 0);
}

const QString VRPNCommandManager::iconSrc() const
{
  return "network-connect";
}

const QString VRPNCommandManager::name() const
{
  return i18n("VRPN");
}

VRPNConfiguration* VRPNCommandManager::getVRPNConfiguration()
{
  return static_cast<VRPNConfiguration*>(getConfigurationPage());
}

bool VRPNCommandManager::deSerializeConfig(const QDomElement& elem)
{
  if (!config) config->deleteLater();
  config = new VRPNConfiguration(this, parentScenario);
  return config->deSerialize(elem);
}

CreateCommandWidget* VRPNCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreateVRPNCommandWidget(this, parent);
}



VRPNCommandManager::~VRPNCommandManager()
{
}
