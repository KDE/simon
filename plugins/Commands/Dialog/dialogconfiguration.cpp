/*   Copyright (C) 2009-2010 Grasch Peter <peter.grasch@bedahr.org>
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

#include "dialogconfiguration.h"
#include "turnconfiguration.h"
#include "dialogcommandmanager.h"

#include <simondialogengine/dialogtemplateoptions.h>
#include <simondialogengine/dialogboundvalues.h>

#include <simondialogengine/avatar.h>
#include <simondialogengine/avatarmodel.h>
#include <simondialogengine/boundvalue.h>
#include <simondialogengine/dialogstate.h>
#include <simondialogengine/dialogturn.h>
#include <simondialogengine/dialogcommand.h>
#include <simondialogengine/confui/templateoptionsconfiguration.h>
#include <simondialogengine/confui/boundvaluesconfiguration.h>
#include <simondialogengine/confui/avatarconfiguration.h>
#include <simonscenarios/scenario.h>

#include "createdialogcommandwidget.h"
#include "createtransitiondialog.h"

#include <QVariantList>
#include <QList>
#include <QString>
#include <QDialog>
#include <QTableView>
#include <QThread>
#include <QApplication>
#include <QPointer>

#include <kgenericfactory.h>
#include <KAboutData>
#include <KInputDialog>
#include <KMessageBox>
#include <KStandardDirs>
#include <simondialogengine/confui/outputconfiguration.h>

K_PLUGIN_FACTORY_DECLARATION(DialogCommandPluginFactory)

DialogConfiguration::DialogConfiguration(DialogCommandManager* _commandManager, Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "dialog", ki18n( "Dialog" ),
    "0.2", ki18n("Build voice controlled dialogs"),
    "im-user",
    DialogCommandPluginFactory::componentData()),
    commandManager(_commandManager),
    boundValuesConfig(new BoundValuesConfiguration(this)),
    templateOptionsConfig(new TemplateOptionsConfiguration(this)),
    avatarsConfig(new AvatarConfiguration(this)),
    outputConfiguration(new OutputConfiguration(this))

{
  Q_UNUSED(args);
  ui.setupUi(this);

  connect(ui.lwStates, SIGNAL(currentRowChanged(int)), this, SLOT(displayCurrentState()));
  connect(ui.lwTurns, SIGNAL(currentRowChanged(int)), this, SLOT(setCurrentTurn()));
  connect(ui.lwTurns, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editTurn()));

  connect(ui.pbAddState, SIGNAL(clicked()), this, SLOT(addState()));
  connect(ui.pbRenameState, SIGNAL(clicked()), this, SLOT(renameState()));
  connect(ui.pbRemoveState, SIGNAL(clicked()), this, SLOT(removeState()));

  connect(ui.pbMoveStateUp, SIGNAL(clicked()), this, SLOT(moveStateUp()));
  connect(ui.pbMoveStateDown, SIGNAL(clicked()), this, SLOT(moveStateDown()));

  ui.twMain->addTab(boundValuesConfig, i18n("Bound values"));
  ui.twMain->addTab(templateOptionsConfig, i18n("Template options"));
  ui.twMain->addTab(avatarsConfig, i18n("Avatars"));
  ui.twMain->addTab(outputConfiguration, i18n("Output"));

  ui.pbAddState->setIcon(KIcon("list-add"));
  ui.pbRenameState->setIcon(KIcon("document-edit"));
  ui.pbRemoveState->setIcon(KIcon("list-remove"));

  ui.pbMoveStateUp->setIcon(KIcon("arrow-up"));
  ui.pbMoveStateDown->setIcon(KIcon("arrow-down"));

  connect(ui.pbAddTurn, SIGNAL(clicked()), this, SLOT(addTurn()));
  connect(ui.pbEditTurn, SIGNAL(clicked()), this, SLOT(editTurn()));
  connect(ui.pbRemoveTurn, SIGNAL(clicked()), this, SLOT(removeTurn()));

  ui.pbAddTurn->setIcon(KIcon("list-add"));
  ui.pbEditTurn->setIcon(KIcon("document-edit"));
  ui.pbRemoveTurn->setIcon(KIcon("list-remove"));

  ui.pbAddRequiredField->setIcon(KIcon("list-add"));
  ui.pbRemoveRequiredField->setIcon(KIcon("list-remove"));

  displayCurrentState();
}

DialogTurn* DialogConfiguration::getCurrentTurn()
{
  DialogState* state = getCurrentState();
  int row = ui.lwTurns->currentRow();

  if (row == -1) return 0;

  QList<DialogTurn*> turns = state->getTurns();

  return turns[row];
}

void DialogConfiguration::addTurn()
{
  DialogState* state = getCurrentState();
  DialogTurn* turn = state->createTurn();

  QPointer<TurnConfiguration> turnConfig = new TurnConfiguration(turn, this);

  if (turnConfig->exec() == QDialog::Accepted)
  {
    state->addTurn(turn);
  }
  else
  {
    delete turn;
  }

  delete turnConfig;
  displayCurrentState();
}

void DialogConfiguration::editTurn()
{
  DialogTurn* turn = getCurrentTurn();
  if (!turn)
  {
    KMessageBox::sorry(this, i18n("No turn selected."));
  }
  else
  {
    DialogTurn* clone = turn->clone();
    QPointer<TurnConfiguration> turnConfig = new TurnConfiguration(clone, this);

    if (turnConfig->exec() == QDialog::Accepted)
    {
      getCurrentState()->setTurn(clone, ui.lwTurns->currentRow());
    }
    else
    {
      delete clone;
    }

    delete turnConfig;
    displayCurrentState();
  }
}

void DialogConfiguration::removeTurn()
{
  if (!getCurrentState()->removeTurn())
    KMessageBox::sorry(this, i18n("No turn selected."));
  displayCurrentState();
}

void DialogConfiguration::avatarSelected ( const QModelIndex& selected )
{
  // if (!selected.isValid())
  // {
  //   getCurrentState()->setAvatar(0);
  // } else {
  //   Avatar *a = static_cast<Avatar*>(selected.internalPointer());
  //   kDebug() << "Selected avatar: " << a->name();
  //   getCurrentState()->setAvatar(a->id());
  // }
}

void DialogConfiguration::addState()
{
  bool ok = true;
  QString name = KInputDialog::getText(i18n("Add State"), i18n("Name of the new state:"),
      QString(), &ok);
  if (!ok) return;

  if (!commandManager->addState(name))
    KMessageBox::sorry(this, i18n("Failed to add state"));
  displayStates();
}

void DialogConfiguration::renameState()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state) return;

  bool ok = true;
  QString name = KInputDialog::getText(i18n("Rename state"), i18n("New name of the state:"), 
      state->getName(), &ok);
  if (!ok) return;

  if (!state->rename(name))
    KMessageBox::sorry(this, i18n("Failed to rename state"));

  displayStates();
}

void DialogConfiguration::removeState()
{
  DialogState* state = getCurrentStateGraphical();
  if (!state) return;

  if (!state ||
      KMessageBox::questionYesNoCancel(this, 
        i18n("Do you really want to remove the selected state?"))
      != KMessageBox::Yes)
    return;

  if (!commandManager->removeState(state))
    KMessageBox::sorry(this, i18n("Failed to remove state"));

  displayStates();

  if (ui.lwStates->count() > 0)
    ui.lwStates->setCurrentRow(0);
}


void DialogConfiguration::moveStateUp()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state)
    return;

  if (!commandManager->moveStateUp(state))
  {
    KMessageBox::sorry(this, i18n("Failed to move state up.\n\nMaybe it is already at the top of the list?"));
    return;
  }

  int row = ui.lwStates->currentRow();
  displayStates();
  ui.lwStates->setCurrentRow(row-1);
}

void DialogConfiguration::moveStateDown()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state)
    return;

  if (!commandManager->moveStateDown(state))
  {
    KMessageBox::sorry(this, i18n("Failed to move state down.\n\nMaybe it is already at the end of the list?"));
    return;
  }

  int row = ui.lwStates->currentRow();
  displayStates();
  ui.lwStates->setCurrentRow(row+1);
}

void DialogConfiguration::textSilenceChanged()
{
  // DialogState *turn = getCurrentStateGraphical();
  // if (!turn)
  //   return;

  // turn->setSilence(ui.cbSilence->isChecked());
  // displayCurrentState();
}

void DialogConfiguration::textAnnounceRepeatChanged()
{
  // DialogState *turn = getCurrentStateGraphical();
  // if (!turn)
  //   return;

  // turn->setAnnounceRepeat(ui.cbAnnounceRepeat->isChecked());
  //displayCurrentState();
}


void DialogConfiguration::addTransition()
{
  // DialogState *turn = getCurrentStateGraphical();
  // if (!turn)
  //   return;

  // CreateDialogCommandWidget *create = new CreateDialogCommandWidget(commandManager, this);
  // CreateTransitionDialog *dialog = new CreateTransitionDialog(create, this);

  // DialogCommand *transition = dialog->createTransition();

  // delete create;
  // delete dialog;

  // if (!transition) return;

  // ((Command*) transition)->setParent(commandManager);
  // turn->addTransition(transition);
}

void DialogConfiguration::editTransition()
{
  // DialogState *turn = getCurrentStateGraphical();
  // DialogCommand *transition = getCurrentTransitionGraphical();
  // if (!turn || !transition)
  //   return;

  // CreateDialogCommandWidget *create = new CreateDialogCommandWidget(commandManager, this);
  // CreateTransitionDialog *dialog = new CreateTransitionDialog(create, this);

  // dialog->editTransition(transition);

  // delete create;
  // delete dialog;
}

void DialogConfiguration::removeTransition()
{
  // DialogState *turn = getCurrentStateGraphical();
  // DialogCommand *transition = getCurrentTransitionGraphical();
  // if (!turn || !transition || 
  //     KMessageBox::questionYesNoCancel(this, 
  //       i18n("Do you really want to remove the selected transition?"))
  //     != KMessageBox::Yes)
  //   return;

  // turn->removeTransition(transition);
}

void DialogConfiguration::moveTransitionUp()
{
  // DialogState *turn = getCurrentStateGraphical();
  // DialogCommand *transition = getCurrentTransitionGraphical();
  // if (!turn || !transition)
  //   return;

  // QItemSelectionModel *model = ui.lvTransitions->selectionModel();
  // int row = model->selectedRows().at(0).row();

  // if (!turn->moveTransitionUp(transition))
  //   KMessageBox::sorry(this, i18n("Failed to move transition up.\n\nMaybe it is already at the top of the list?"));

  // model->select(ui.lvTransitions->model()->index(row-1, 0),
  //                         QItemSelectionModel::ClearAndSelect);
}

void DialogConfiguration::moveTransitionDown()
{
  // DialogState *turn = getCurrentStateGraphical();
  // DialogCommand *transition = getCurrentTransitionGraphical();
  // if (!turn || !transition)
  //   return;

  // QItemSelectionModel *model = ui.lvTransitions->selectionModel();
  // int row = model->selectedRows().at(0).row();

  // if (!turn->moveTransitionDown(transition))
  //   KMessageBox::sorry(this, i18n("Failed to move transition down.\n\nMaybe it is already at the end of the list?"));

  // model->select(ui.lvTransitions->model()->index(row+1, 0),
  //                         QItemSelectionModel::ClearAndSelect);
}


QDomElement DialogConfiguration::serialize(QDomDocument* doc)
{
  QDomElement configElem = doc->createElement("config");

  configElem.appendChild(templateOptionsConfig->serialize(doc));

  configElem.appendChild(boundValuesConfig->serialize(doc));

  QDomElement avatarsElem = avatarsConfig->serialize(doc);
  configElem.appendChild(avatarsElem);

  configElem.appendChild(outputConfiguration->serialize(doc));
  return configElem;
}


bool DialogConfiguration::deSerialize(const QDomElement& elem)
{
  if (!outputConfiguration->deSerialize(elem)) {
    defaults();
    kDebug() << "Setting defaults...";
    return true;
  }

  if (!templateOptionsConfig->deSerialize(elem))
    return false;

  if (!boundValuesConfig->deSerialize(elem))
    return false;

  if (!avatarsConfig->deSerialize(elem))
    return false;
  // ui.lvStateAvatar->setModel(avatarsConfig->getModel());

  return true;
}

void DialogConfiguration::init()
{
  displayStates();
}

void DialogConfiguration::displayStates()
{
  int oldRow = ui.lwStates->currentRow();

  ui.lwStates->clear();

  QList<DialogState*> states = commandManager->getStates();
  int id = 1;
  foreach (DialogState* state, states)
  {
    ui.lwStates->addItem(i18nc("%1: id of state; %2: name of state", "%1: %2", id, state->getName()));
    id++;
  }

  if (ui.lwStates->count() > 0)
  {
    if ((ui.lwStates->count() > oldRow) && (oldRow >= 0))
      ui.lwStates->setCurrentRow(oldRow);
    else
      ui.lwStates->setCurrentRow(0);
  }

  if (!getCurrentState())
  {
    ui.lwTurns->clear();
  }

  bool stateEnabled = ui.lwStates->count() > 0;
  bool turnEnabled = ui.lwTurns->count() > 0;
  ui.pbAddRequiredField->setEnabled(stateEnabled);
  ui.pbRemoveRequiredField->setEnabled(stateEnabled);
  ui.pbAddTurn->setEnabled(stateEnabled);
  ui.pbEditTurn->setEnabled(stateEnabled && turnEnabled);
  ui.pbRemoveTurn->setEnabled(stateEnabled && turnEnabled);
  
}

DialogState* DialogConfiguration::getCurrentState()
{
  int row = ui.lwStates->currentRow();

  if (row == -1) return 0;

  QList<DialogState*> states = commandManager->getStates();

  return states[row];
}

DialogState* DialogConfiguration::getCurrentStateGraphical()
{
  DialogState *state = getCurrentState();
  if (!state)
    KMessageBox::information(this, i18n("Please select a turn from the left or add new ones as appropriate."));

  return state;
}

DialogCommand* DialogConfiguration::getCurrentTransition()
{
  // QModelIndex index = ui.lvTransitions->currentIndex();
  // if (!index.isValid())
  //   return 0;

  // return static_cast<DialogCommand*>(index.internalPointer());
  return 0;
}

DialogCommand* DialogConfiguration::getCurrentTransitionGraphical()
{
  DialogCommand *command = getCurrentTransition();
  if (!command)
    KMessageBox::information(this, i18n("Please select an option from the right or add new ones as appropriate."));

  return command;
}


void DialogConfiguration::displayCurrentState()
{
  DialogState* currentState = getCurrentState();

  if (!currentState)
  {
    return;
  }

  ui.lwTurns->clear();
  QList<DialogTurn*> turns = currentState->getTurns();
  int id = 1;
  foreach (DialogTurn* turn, turns)
  {
    ui.lwTurns->addItem(i18nc("%1: id of turn; %2: name of turn", "%1: %2", id, turn->getName()));
    ++id;
  }

  bool turnEnabled = ui.lwTurns->count() > 0;
  ui.pbEditTurn->setEnabled(turnEnabled);
  ui.pbRemoveTurn->setEnabled(turnEnabled);
}

void DialogConfiguration::setCurrentTurn()
{
  getCurrentState()->setCurrentTurn(ui.lwTurns->currentRow());
}

void DialogConfiguration::defaults()
{
  avatarsConfig->defaults();
  templateOptionsConfig->defaults();
  outputConfiguration->defaults();
  boundValuesConfig->defaults();
}

int DialogConfiguration::getAvatarSize() const
{
  return outputConfiguration->getAvatarSize();
}

bool DialogConfiguration::getDisplayAvatarNames() const
{
  return outputConfiguration->getDisplayAvatarNames();
}
QString DialogConfiguration::getOptionSeparatorText() const
{
  return outputConfiguration->getOptionSeparatorText();
}
QString DialogConfiguration::getRepeatAnnouncement() const
{
  return outputConfiguration->getRepeatAnnouncement();
}
bool DialogConfiguration::getRepeatOnInvalidInput() const
{
  return outputConfiguration->getRepeatOnInvalidInput();
}
QStringList DialogConfiguration::getRepeatTriggers() const
{
  return outputConfiguration->getRepeatTriggers();
}
bool DialogConfiguration::useGUIOutput() const
{
  return outputConfiguration->useGUIOutput();
}
bool DialogConfiguration::useTTSOutput() const
{
  return outputConfiguration->useTTSOutput();
}

void DialogConfiguration::avatarDisplayToggled (bool show)
{
  getCurrentState()->setDisplayAvatar(show);
}

DialogTemplateOptions* DialogConfiguration::getDialogTemplateOptions() const
{
  return templateOptionsConfig->getDialogTemplateOptions();
}

DialogBoundValues* DialogConfiguration::getDialogBoundValues() const
{
  return boundValuesConfig->getDialogBoundValues();
}

Avatar* DialogConfiguration::getAvatar ( int id ) const
{
  return avatarsConfig->getAvatar(id);
}

DialogConfiguration::~DialogConfiguration()
{
}
