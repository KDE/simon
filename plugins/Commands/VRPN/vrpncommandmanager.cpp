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

#include <vrpn_Connection.h>
#include <vrpn_Button.h>

#include <QMutexLocker>
#include <QThread>
#include <KI18n/klocalizedstring.h>
#include <QDebug>

K_PLUGIN_FACTORY( VRPNCommandPluginFactory,
registerPlugin< VRPNCommandManager >();
)

// K_EXPORT_PLUGIN( VRPNCommandPluginFactory("simonvrpncommand") )


class SimonButton : public vrpn_Button {
protected:
  void prepareChange() {
    timeval timestampTemp;
    vrpn_gettimeofday(&timestampTemp, NULL);
    vrpn_Button::timestamp = timestampTemp;
    vrpn_Button::lastbuttons[0] = vrpn_Button::buttons[0];
  }
public:
  SimonButton(const QString& name, vrpn_Connection* connection) : vrpn_Button(name.toUtf8(), connection) {
    vrpn_Button::num_buttons = 1;
    vrpn_Button::buttons[0] = vrpn_Button::lastbuttons[0] = 0;
  }
  virtual void mainloop() {
    server_mainloop();
  }
  void toggle() {
    prepareChange();
    vrpn_Button::buttons[0] = !vrpn_Button::buttons[0];
    vrpn_Button::report_changes();
  }
  void press() {
    prepareChange();
    vrpn_Button::buttons[0] = 1;
    vrpn_Button::report_changes();
  }
  void release() {
    prepareChange();
    vrpn_Button::buttons[0] = 0;
    vrpn_Button::report_changes();
  }
};

VRPNCommandManager::VRPNCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
  connection(0)
{
  connect(&serverMainLoopTimer, SIGNAL(timeout()), this, SLOT(serverMainLoop()));
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
  bool success = config->deSerialize(elem);
  restartServer();
  return success;
}

CreateCommandWidget* VRPNCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreateVRPNCommandWidget(getVRPNConfiguration()->getButtons(), this, parent);
}

void VRPNCommandManager::restartServer()
{
  QMutexLocker l(&serverMutex);
  qDeleteAll(buttons);
  buttons.clear();
  delete connection;

  connection = vrpn_create_server_connection(getVRPNConfiguration()->getPort());
  foreach (const QString& name, getVRPNConfiguration()->getButtons()) {
    SimonButton* button = 0;
    if ( (button = new SimonButton(name, connection)) == 0) {
      qWarning() << "Failed to create button: " << name;
    }
    buttons.insert(name, button);
  }
  serverMainLoopTimer.start(600);
}

void VRPNCommandManager::serverMainLoop()
{
  QMutexLocker l(&serverMutex);
  if (!connection)
    return;
  foreach (SimonButton* button, buttons)
    button->mainloop();
  connection->mainloop();
}

bool VRPNCommandManager::activateButton(const QString &name, VRPNCommand::ClickMode clickMode)
{
  QMutexLocker l(&serverMutex);
  if (!buttons.contains(name))
    return false;
  SimonButton *button = buttons.value(name);
  switch (clickMode) {
    case VRPNCommand::Press:
      button->press();
      break;
    case VRPNCommand::Release:
      button->release();
      break;
    case VRPNCommand::PressAndRelease:
      button->press();
      button->release();
      break;
    case VRPNCommand::Toggle:
      button->toggle();
      break;
  }
  return true;
}

DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(VRPNCommandManager, VRPNCommand);

VRPNCommandManager::~VRPNCommandManager()
{
  QMutexLocker l(&serverMutex);
  qDeleteAll(buttons);
  delete connection;
}

#include "vrpncommandmanager.moc"