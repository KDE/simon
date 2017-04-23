/*   Copyright (C) 2009 Grasch Peter <peter.grasch@bedahr.org>
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

#include "jsonconfiguration.h"
#include "jsoncommandmanager.h"

#include <QVariantList>
#include <KPluginFactory>

K_PLUGIN_FACTORY_DECLARATION(JsonCommandPluginFactory)

JsonConfiguration::JsonConfiguration(Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "json", ki18n( "JSON" ),
"0.1", ki18n("Send JSON messages"),
"network-connect",
JsonCommandPluginFactory::componentData())
{
  Q_UNUSED(args);
  ui.setupUi(this);

}



QDomElement JsonConfiguration::serialize(QDomDocument* doc)
{
  QDomElement configElem = doc->createElement("config");

  QDomElement hostElem = doc->createElement("host");
  hostElem.appendChild(doc->createTextNode(ui.leHost->text()));
  QDomElement portElem = doc->createElement("port");
  portElem.appendChild(doc->createTextNode(ui.sbPort->text()));

  configElem.appendChild(hostElem);
  configElem.appendChild(portElem);

  return configElem;
}


bool JsonConfiguration::deSerialize(const QDomElement& elem)
{
  QDomElement hostElem = elem.firstChildElement("host");
  QDomElement portElem = elem.firstChildElement("port");

  if (hostElem.isNull() || portElem.isNull()) 
  {
    defaults();
    return true;
  }

  ui.leHost->setText(hostElem.text());
  ui.sbPort->setValue(portElem.text().toInt());

  return true;
}


void JsonConfiguration::defaults()
{
  ui.leHost->setText("127.0.0.1");
  ui.sbPort->setValue(8080);
}

QString JsonConfiguration::host()
{
  return ui.leHost->text();
}

int JsonConfiguration::port()
{
  return ui.sbPort->value();
}

JsonConfiguration::~JsonConfiguration()
{
}
