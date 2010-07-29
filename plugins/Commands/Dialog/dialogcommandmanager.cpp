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
#include "dialogstate.h"
#include "dialogtextparser.h"

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
  activateAction(new KAction(this)),
  dialogParser(NULL)
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

void DialogCommandManager::initState(DialogState *state)
{
  foreach (DialogView* view, dialogViews)
    view->present(*state);
}

void DialogCommandManager::initState(int state)
{
  kDebug() << "Switching to state: " << state;

  //0 state means quit
  if (state == 0)
  {
    deregister();
    return;
  }

  //else, keep in mind that indizes do still start with 0 so 
  //decrement state
  state--;

  if (state >= dialogStates.count() || state < 0)
  {
    kWarning() << "Invalid state provided";
    return;
  }

  initState(dialogStates.at(state));
}

void DialogCommandManager::activate()
{
  foreach (DialogView* view, dialogViews)
    view->start();

  startGreedy();
  
  //switch the command managers internal state to +1 as then the
  //numbers line up to the pages
  switchToState(SimonCommand::GreedyState + 1);
  initState(1); // always start with state 1;
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

QDomElement DialogCommandManager::serializeCommands(QDomDocument *doc)
{
  QDomElement commandsElem = CommandManager::serializeCommands(doc);

  foreach (DialogState *state, dialogStates) {
    QDomElement commandElem = state->serialize(doc);
    commandsElem.appendChild(commandElem);
  }

  return commandsElem;
}

bool DialogCommandManager::deSerializeCommandsPrivate(const QDomElement& elem)
{ 
  if (elem.isNull()) return false;

  if (!commands)
    commands = new CommandList();

  QDomElement stateElem = elem.firstChildElement("state");
  int stateId = SimonCommand::GreedyState + 1;
  while(!stateElem.isNull())
  {
    kDebug() << "Deserializing state element";
    DialogState *state = DialogState::createInstance(dialogParser, stateElem);

    if (state)
    {
      connect(state, SIGNAL(requestDialogState(int)), this, SLOT(initState(int)));
      dialogStates << state;

      QList<DialogCommand*> transitions = state->getTransitions();

      foreach (DialogCommand* transition, transitions)
      {
        transition->createStateLink(stateId);
        *commands << transition;
      }
    }

    stateElem = stateElem.nextSiblingElement("state");

    stateId++;
  }

  return true;
}


bool DialogCommandManager::deSerializeConfig(const QDomElement& elem)
{
  if (!config) config->deleteLater();
  config = new DialogConfiguration(this, parentScenario);
  config->deSerialize(elem);

  bool succ = true;
  succ &= installInterfaceCommand(this, "activate", i18n("Dialog"), iconSrc(),
    i18n("Start dialog"), true /* announce */, true /* show icon */,
    SimonCommand::DefaultState /* consider this command when in this state */,
    SimonCommand::GreedyState,                    /* if executed switch to this state */
    QString() /* take default visible id from action name */,
    "startDialog" /* id */);

  if (!dialogParser)
    dialogParser = new DialogTextParser();
    
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
