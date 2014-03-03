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
#include "dialogcommandmanager.h"
#include "dialogconfiguration.h"
#include <simondialogengine/dialogcommand.h>
#include <simondialogengine/dialogstate.h>
#include <simondialogengine/dialogtextparser.h>

#include <simondialogengine/dialogview.h>
#include <simondialogengine/graphicalview/visualdialogview.h>
#include <simondialogengine/ttsview/ttsdialogview.h>

#include <simonscenarios/scenario.h>
#include <simonscenarios/voiceinterfacecommand.h>

#include "createdialogcommandwidget.h"
#include <eventsimulation/eventhandler.h>
#include <KLocalizedString>
#include <KAction>
#include <KMessageBox>

K_PLUGIN_FACTORY( DialogCommandPluginFactory,
registerPlugin< DialogCommandManager >();
)

K_EXPORT_PLUGIN( DialogCommandPluginFactory("simondialogcommand") )

DialogCommandManager::DialogCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
  GreedyReceiver(this),
  activateAction(new KAction(this)),
  currentDialogState(NULL),
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

void DialogCommandManager::initState(DialogState* state)
{
  if (currentDialogState)
    currentDialogState->left();

  state->updateRandomTextSelection();
  
  foreach (DialogView* view, dialogViews)
    view->present(*(state->getTurns().at(0)));

  state->presented();

  currentDialogState = state;
}

void DialogCommandManager::initState(int State)
{
  kDebug() << "Switching to State: " << State;

  //0 State means quit
  if ((State == 0) ||  (State >= DialogStates.count()+1 || State < 1))
  {
    if (currentDialogState)
      currentDialogState->left();
    currentDialogState = NULL;

    deregister();
    switchToState(SimonCommand::DefaultState);
    return;
  }

<<<<<<< HEAD
  //else, keep in mind that indizes do still start with 0 so 
  //decrement State
  State--;
=======
  //else, keep in mind that indices do still start with 0 so 
  //decrement state
  state--;
>>>>>>> local_testing

  initState(DialogStates.at(State));
}

bool DialogCommandManager::addState(const QString& name)
{
  DialogState* state = new DialogState(dialogParser, name, 
                                    QList<DialogTurn*>(), this);
  connect(state, SIGNAL(changed()), this, SLOT(StateChanged()));
  DialogStates << state;

  /*
  DialogState *turn = new DialogState(dialogParser, name, QString(), false, true,
                                    QList<DialogCommand*>(), this);
  connect(turn, SIGNAL(requestDialogState(int)), this, SLOT(initState(int)));
    DialogStates << turn;
  currentDialogState->addTurn(name, dialogParser);
  */
  kDebug() << "Adding state...";

  return true;
  //return parentScenario->save();
}

bool DialogCommandManager::removeState(DialogState *state)
{
  if (state == currentDialogState)
  {
    currentDialogState = NULL;
    initState(0);
  }
  int removed = DialogStates.removeAll(state);

  if (!removed)
    return false;

  delete state;
  return true;
  //return parentScenario->save();
}

bool DialogCommandManager::moveStateUp(DialogState *state)
{
  int index = DialogStates.indexOf(state);
  if (index <= 0) return false;

  DialogStates.insert(index-1, DialogStates.takeAt(index));
  return true;
  //return parentScenario->save();
}

bool DialogCommandManager::moveStateDown(DialogState *state)
{
  int index = DialogStates.indexOf(state);
  if ((index == -1) || (index == (DialogStates.count()-1)))
    return false;

  DialogStates.insert(index+1, DialogStates.takeAt(index));
  return true;
  //return parentScenario->save();
}


void DialogCommandManager::activate(const QString& arg0, const QString& arg1, const QString& arg2, const QString& arg3,
      const QString& arg4, const QString& arg5, const QString& arg6,
      const QString& arg7, const QString& arg8, const QString& arg9)
{
  QStringList arguments;
  arguments << arg0 << arg1 << arg2 << arg3 << arg4 
	    << arg5 << arg6 << arg7 << arg8 << arg9;
  dialogParser->setArguments(arguments);
  
  qDeleteAll(dialogViews);
  dialogViews.clear();

  if (DialogStates.isEmpty())
    return;

  if (getDialogConfiguration()->useGUIOutput())
    dialogViews << new VisualDialogView(this);
  if (getDialogConfiguration()->useTTSOutput())
    dialogViews << new TTSDialogView(this);
  
  if (dialogViews.isEmpty()) return;

  foreach (DialogView* view, dialogViews)
    view->start();

  startGreedy();
  
  //switch the command managers internal State to +1 as then the
  //numbers line up to the pages
  switchToState(SimonCommand::GreedyState + 1);
  initState(1); // always start with State 1;
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

bool DialogCommandManager::trigger(const QString& triggerName, bool silent)
{
  bool found = CommandManager::trigger(triggerName, silent);
  if (!currentDialogState)
    return found;

  if (!found)
  {
    //check if this is a repeat trigger
    if (getDialogConfiguration()->getRepeatTriggers().contains(triggerName, Qt::CaseInsensitive))
    {
      foreach (DialogView* view, dialogViews)
<<<<<<< HEAD
        view->repeat(*(currentDialogState->getTurns().at(0)));
=======
        view->repeat(*currentDialogState);
>>>>>>> local_testing
      found = true;
    }
  }
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
  return trigger(inputText, false /* silent */);
}


DialogConfiguration* DialogCommandManager::getDialogConfiguration() const
{
  return static_cast<DialogConfiguration*>(getConfigurationPage());
}

QDomElement DialogCommandManager::serializeCommands(QDomDocument *doc)
{
  QDomElement commandsElem = doc->createElement("commands");
  foreach (Command *c, commands) {
    //only store voice interface commands
    if (dynamic_cast<VoiceInterfaceCommand*>(c))
    {
      QDomElement commandElem = c->serialize(doc);
      commandElem.setTagName("voiceInterfaceCommand");
      commandsElem.appendChild(commandElem);
    }
  }

  foreach (DialogState *turn, DialogStates) {
    QDomElement commandElem = turn->serialize(doc);
    commandsElem.appendChild(commandElem);
  }

  return commandsElem;
}

void DialogCommandManager::StateChanged()
{
  bindStateCommands();
}

bool DialogCommandManager::deSerializeCommandsPrivate(const QDomElement& elem)
{ 
  if (elem.isNull()) return false;

  QDomElement stateElem = elem.firstChildElement("State");
  while(!stateElem.isNull())
  {
    kDebug() << "Deserializing State element";
    DialogState *turn = DialogState::createInstance(dialogParser, stateElem);

    if (turn)
    {
      connect(turn, SIGNAL(requestDialogState(int)), this, SLOT(initState(int)));
      connect(turn, SIGNAL(changed()), this, SLOT(StateChanged()));
      connect(turn, SIGNAL(destroyed()), this, SLOT(StateDestroyed()));
      DialogStates << turn;
    }

    stateElem = stateElem.nextSiblingElement("State");
  }

  bindStateCommands();

  getDialogConfiguration()->init();
  return true;
}

void DialogCommandManager::StateDestroyed()
{
  DialogStates.removeAll(static_cast<DialogState*>(sender()));
}

void DialogCommandManager::bindStateCommands()
{
  kDebug() << "rebinding";
  QList<Command*> oldCommands;

  foreach (Command* c, commands)
  {
    if (dynamic_cast<DialogCommand*>(c))
    {
      commands.removeAll(c);
      oldCommands << c;
    }
  }

  int StateId = SimonCommand::GreedyState + 1;
  foreach (DialogState *state, DialogStates)
  {
    /*
    QList<DialogCommand*> transitions = state->getTransitions();
    
    foreach (DialogCommand* transition, transitions)
    {
      transition->createStateLink(StateId);
      commands << transition;
    }

    StateId++;
    */

    state->bindStateCommands(commands);
    StateId++;
  }

  foreach (Command* c, oldCommands)
  {
    if (!commands.contains(c))
      delete c;
  }
}

bool DialogCommandManager::deSerializeConfig(const QDomElement& elem)
{
  if (!config) config->deleteLater();
  config = new DialogConfiguration(this, parentScenario);
  if (!config->deSerialize(elem))
    KMessageBox::sorry(0, i18n("Failed to initialize dialog configuration."));

  bool succ = true;
  succ &= installInterfaceCommand(this, "activate", i18n("Dialog"), iconSrc(),
    i18n("Start dialog"), true /* announce */, true /* show icon */,
    SimonCommand::DefaultState /* consider this command when in this State */,
    SimonCommand::GreedyState,                    /* if executed switch to this State */
    QString() /* take default visible id from action name */,
    "startDialog" /* id */);

  if (!dialogParser)
    dialogParser = new DialogTextParser(getDialogConfiguration()->getDialogTemplateOptions(),
                        getDialogConfiguration()->getDialogBoundValues());
    
  return succ;
}

QString DialogCommandManager::getOptionSeparatorText() const
{
  return getDialogConfiguration()->getOptionSeparatorText();
}

QString DialogCommandManager::getRepeatAnnouncement() const
{
  return getDialogConfiguration()->getRepeatAnnouncement();
}
 
bool DialogCommandManager::getRepeatOnInvalidInput() const
{
  return getDialogConfiguration()->getRepeatOnInvalidInput();
}

Avatar* DialogCommandManager::getAvatar ( int id ) const
{
  return getDialogConfiguration()->getAvatar(id);
}
bool DialogCommandManager::getDisplayAvatarNames() const
{
  return getDialogConfiguration()->getDisplayAvatarNames();
}
int DialogCommandManager::getAvatarSize() const
{
  return getDialogConfiguration()->getAvatarSize();
}

DialogCommandManager::~DialogCommandManager()
{
  activateAction->deleteLater();
  qDeleteAll(dialogViews);
  foreach (DialogState *s, DialogStates)
    disconnect(s, SIGNAL(destroyed()), this, SLOT(StateDestroyed()));
  qDeleteAll(DialogStates);
}
