/*   Copyright (C) 2014 Grasch Peter <peter.grasch@bedahr.org>
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

#include "vrpnconfiguration.h"
#include "vrpncommandmanager.h"

#include <simonscenarios/scenario.h>

#include <vrpn_Configure.h>
#include <QString>
#include <QDebug>
K_PLUGIN_FACTORY_DECLARATION(VRPNCommandPluginFactory)

VRPNConfiguration::VRPNConfiguration(VRPNCommandManager* _commandManager, Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "vrpn", ki18n( "VRPN" ),
"0.1", ki18n("Provide a VRPN server"),
"network-connect"),
commandManager(_commandManager)
{
  Q_UNUSED(args);
  ui.setupUi(this);

}

QDomElement VRPNConfiguration::serialize(QDomDocument* doc)
{
  QDomElement configElem = doc->createElement("config");

  //general
  QDomElement portElem = doc->createElement("port");
  portElem.appendChild(doc->createTextNode(QString::number(ui.sbPort->value())));
  configElem.appendChild(portElem);
  QDomElement buttonsElem = doc->createElement("buttons");
  foreach (const QString& name, ui.elbButtons->items()) {
    QDomElement buttonElem = doc->createElement("button");
    buttonElem.appendChild(doc->createTextNode(name));
    buttonsElem.appendChild(buttonElem);
  }
  configElem.appendChild(buttonsElem);
  commandManager->restartServer();
  return configElem;
}

bool VRPNConfiguration::deSerialize(const QDomElement& elem)
{
  QDomElement portElem = elem.firstChildElement("port");
  QDomElement buttonsElem = elem.firstChildElement("buttons");
  if (portElem.isNull() || buttonsElem.isNull()) {
    defaults();
    return true;
  }

  ui.sbPort->setValue(portElem.text().toInt());
  QDomElement buttonElem = buttonsElem.firstChildElement("button");
  QStringList items;
  while (!buttonElem.isNull()) {
    items << buttonElem.text();
    buttonElem = buttonElem.nextSiblingElement("button");
  }
  ui.elbButtons->setItems(items);
  return true;
}

void VRPNConfiguration::defaults()
{
  ui.sbPort->setValue(vrpn_DEFAULT_LISTEN_PORT_NO);
}

QStringList VRPNConfiguration::getButtons() const
{
  return ui.elbButtons->items();
}
int VRPNConfiguration::getPort() const
{
  return ui.sbPort->value();
}

VRPNConfiguration::~VRPNConfiguration()
{
}
