/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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
#include "jsoncommandmanager.h"
#include "jsoncommand.h"
#include "createjsoncommandwidget.h"
#include "jsonconfiguration.h"

#include <simonjsonconnector/simonjsonconnector.h>

#include <QNetworkReply>

#include <KLocalizedString>

K_PLUGIN_FACTORY( JsonCommandPluginFactory,
registerPlugin< JsonCommandManager >();
)

K_EXPORT_PLUGIN( JsonCommandPluginFactory("simonjsoncommand") )

JsonCommandManager::JsonCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args)
{
}

bool JsonCommandManager::deSerializeConfig(const QDomElement& elem)
{
  delete config; // delete existing config

  config = new JsonConfiguration(parentScenario);
  return config->deSerialize(elem);
}

bool JsonCommandManager::sendRequest(const QString& url, const QString& request)
{
  JsonConfiguration *jsonConfig = static_cast<JsonConfiguration*>(config);
  kDebug() << "Configuration: " << jsonConfig->host() << jsonConfig->port();
  kDebug() << "Triggering..." << url << request;

  SimonJsonConnector::getInstance()->sendRequest(jsonConfig->host(), jsonConfig->port(), 
                                                  url, request, this, "requestFinished");
  //QNetworkRequest networkRequest;
  //networkRequest.setUrl(QUrl("http://"+jsonConfig->host()+':'+QString::number(jsonConfig->port())+'/'+url));
  //kDebug() << "Url: " << "http://"+jsonConfig->host()+':'+QString::number(jsonConfig->port())+'/'+url;
  //m_accessManager->post(networkRequest, request.toAscii());

  return true;
}

void JsonCommandManager::requestFinished(QNetworkReply *reply)
{
  kDebug() << "Request has finished...";
  kDebug() << "Reply: " << reply;

  kDebug() << "Reply: " << reply->readAll();
  kDebug() << "Error: " << reply->error();
}


bool JsonCommandManager::shouldAcceptCommand(Command *command)
{
  return (dynamic_cast<JsonCommand*>(command) != 0);
}



const QString JsonCommandManager::iconSrc() const
{
  return "network-connect";
}


const QString JsonCommandManager::name() const
{
  return i18n("JSON");
}


CreateCommandWidget* JsonCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreateJsonCommandWidget(this, parent);
}

DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(JsonCommandManager, JsonCommand);


JsonCommandManager::~JsonCommandManager()
{
}
