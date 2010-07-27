/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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
#include "dialogcommandmanager.h"
#include "dialogconfiguration.h"
#include "dialogcommand.h"

#include "dialogview.h"
#include "visualdialogview.h"

#include "createdialogcommandwidget.h"
#include <eventsimulation/eventhandler.h>
#include <KLocalizedString>
#include <KAction>

K_PLUGIN_FACTORY( DialogCommandPluginFactory,
registerPlugin< DialogCommandManager >();
)

K_EXPORT_PLUGIN( DialogCommandPluginFactory("simondialogcommand") )

DialogCommandManager::DialogCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
  GreedyReceiver(this),
  activateAction(new KAction(this))
{
  activateAction->setText(i18n("Activate Dialog"));
  activateAction->setIcon(KIcon("input-dialog"));
  connect(activateAction, SIGNAL(triggered(bool)),
    this, SLOT(activate()));
  guiActions<<activateAction;
}

bool DialogCommandManager::shouldAcceptCommand(Command *command)
{
  return (dynamic_cast<DialogCommand*>(command) != 0);
}

void DialogCommandManager::setFont(const QFont& font)
{
  foreach (DialogView* view, dialogViews)
    view->setFont(font);
}



void DialogCommandManager::activate()
{
  foreach (DialogView* view, dialogViews)
    view->start();

  startGreedy();
}


void DialogCommandManager::deregister()
{
  foreach (DialogView* view, dialogViews)
    view->stop();

  stopGreedy();
}


const QString DialogCommandManager::iconSrc() const
{
  return "im-user";
}


const QString DialogCommandManager::name() const
{
  return i18n("Dialog");
}


bool DialogCommandManager::greedyTrigger(const QString& inputText)
{
  return trigger(inputText);
}


DialogConfiguration* DialogCommandManager::getDialogConfiguration()
{
  return static_cast<DialogConfiguration*>(getConfigurationPage());
}



bool DialogCommandManager::deSerializeConfig(const QDomElement& elem)
{
  //Connect to Slots
  //connect(ui.pbOk, SIGNAL(clicked()), dialogWidget, SLOT(hide()));
  //connect(ui.pbOk, SIGNAL(clicked()), this, SLOT(deregister()));

  if (!config) config->deleteLater();
  config = new DialogConfiguration(this, parentScenario);
  config->deSerialize(elem);

  bool succ = true;
  succ &= installInterfaceCommand(this, "activate", i18n("Dialog"), iconSrc(),
    i18n("Starts dialog"), true /* announce */, true /* show icon */,
    SimonCommand::DefaultState /* consider this command when in this state */,
    SimonCommand::GreedyState,                    /* if executed switch to this state */
    QString() /* take default visible id from action name */,
    "startDialog" /* id */);

  //succ &= installInterfaceCommand(ui.pbOk, "click", i18nc("Close the dialog", "Ok"), "dialog-ok",
    //i18n("Hides the dialog"), false, true, SimonCommand::GreedyState,
    //SimonCommand::DefaultState);
    
  dialogViews << new VisualDialogView(this);
  
  return succ;
}

CreateCommandWidget* DialogCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreateDialogCommandWidget(this, parent);
}



DialogCommandManager::~DialogCommandManager()
{
  activateAction->deleteLater();
  qDeleteAll(dialogViews);
}
