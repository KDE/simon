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

#include <simonscenarios/scenario.h>
#include <simonscenarios/voiceinterfacecommand.h>

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
  currentDialogSate(NULL),
  dialogParser(NULL)
{
  activateAction->setText(i18n("Activate Dialog"));
  activateAction->setIcon(KIcon("im-user"));
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
  if (currentDialogSate)
    currentDialogSate->left();

  foreach (DialogView* view, dialogViews)
    view->present(*state);

  state->presented();

  currentDialogSate = state;
}

void DialogCommandManager::initState(int state)
{
  kDebug() << "Switching to state: " << state;

  //0 state means quit
  if (state == 0)
  {
    if (currentDialogSate)
      currentDialogSate->left();
    currentDialogSate = NULL;

    deregister();
    switchToState(SimonCommand::DefaultState);
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

bool DialogCommandManager::addState(const QString& name)
{
  DialogState *state = new DialogState(dialogParser, name, QString(),
                                    QList<DialogCommand*>(), this);
  connect(state, SIGNAL(requestDialogState(int)), this, SLOT(initState(int)));
  connect(state, SIGNAL(changed()), this, SLOT(stateChanged()));
  dialogStates << state;

  return parentScenario->save();
}

bool DialogCommandManager::removeState(DialogState *state)
{
  int removed = dialogStates.removeAll(state);

  if (!removed)
    return false;

  delete state;
  return parentScenario->save();
}

bool DialogCommandManager::moveStateUp(DialogState *state)
{
  int index = dialogStates.indexOf(state);
  if (index <= 0) return false;

  dialogStates.insert(index-1, dialogStates.takeAt(index));
  return parentScenario->save();
}

bool DialogCommandManager::moveStateDown(DialogState *state)
{
  int index = dialogStates.indexOf(state);
  if ((index == -1) || (index == (dialogStates.count()-1))) 
    return false;

  dialogStates.insert(index+1, dialogStates.takeAt(index));
  return parentScenario->save();
}


void DialogCommandManager::activate()
{
  if (dialogViews.isEmpty() || dialogStates.isEmpty())
    return;

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

bool DialogCommandManager::trigger(const QString& triggerName)
{
  bool found = CommandManager::trigger(triggerName);
  if (found) {
    foreach (DialogView* view, dialogViews)
      view->correctInputReceived();
  } else {
    foreach (DialogView* view, dialogViews)
      view->warnOfInvalidInput(triggerName);
  }
  return found;
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
  QDomElement commandsElem = doc->createElement("commands");
  if (commands) {
    foreach (Command *c, *commands) {
      //only store voice interface commands
      if (dynamic_cast<VoiceInterfaceCommand*>(c))
      {
        QDomElement commandElem = c->serialize(doc);
        commandElem.setTagName("voiceInterfaceCommand");
        commandsElem.appendChild(commandElem);
      }
    }
  }

  foreach (DialogState *state, dialogStates) {
    QDomElement commandElem = state->serialize(doc);
    commandsElem.appendChild(commandElem);
  }

  return commandsElem;
}

void DialogCommandManager::stateChanged()
{
  bindStateCommands();
  parentScenario->save();
}

bool DialogCommandManager::deSerializeCommandsPrivate(const QDomElement& elem)
{ 
  if (elem.isNull()) return false;

  if (!commands)
    commands = new CommandList();

  QDomElement stateElem = elem.firstChildElement("state");
  while(!stateElem.isNull())
  {
    kDebug() << "Deserializing state element";
    DialogState *state = DialogState::createInstance(dialogParser, stateElem);

    if (state)
    {
      connect(state, SIGNAL(requestDialogState(int)), this, SLOT(initState(int)));
      connect(state, SIGNAL(changed()), this, SLOT(stateChanged()));
      dialogStates << state;
    }

    stateElem = stateElem.nextSiblingElement("state");
  }

  bindStateCommands();

  getDialogConfiguration()->init();
  return true;
}

void DialogCommandManager::bindStateCommands()
{
  kDebug() << "rebinding";
  QList<Command*> oldCommands;

  foreach (Command* c, *commands)
  {
    if (dynamic_cast<DialogCommand*>(c))
    {
      commands->removeAll(c);
      oldCommands << c;
    }
  }

  int stateId = SimonCommand::GreedyState + 1;
  foreach (DialogState *state, dialogStates)
  {
    QList<DialogCommand*> transitions = state->getTransitions();

    foreach (DialogCommand* transition, transitions)
    {
      transition->createStateLink(stateId);
      *commands << transition;
    }

    stateId++;
  }

  foreach (Command* c, oldCommands)
  {
    if (!commands->contains(c))
      delete c;
  }
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
    dialogParser = new DialogTextParser(getDialogConfiguration()->getDialogTemplateOptions(),
                        getDialogConfiguration()->getDialogBoundValues());
    
  dialogViews << new VisualDialogView(this);
  
  return succ;
}



DialogCommandManager::~DialogCommandManager()
{
  activateAction->deleteLater();
  qDeleteAll(dialogViews);
}
