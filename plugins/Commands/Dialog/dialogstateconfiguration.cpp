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

#include "dialogstateconfiguration.h"
#include "dialogcommandmanager.h"
#include "turnconfiguration.h"
#include <simondialogengine/dialogstate.h>
#include <simondialogengine/dialogturn.h>
#include "ui_dialogstateview.h"

#include <QLineEdit>
#include <KMessageBox>

DialogStateConfiguration::DialogStateConfiguration(QWidget *parent,
  DialogCommandManager* _commandManager, DialogState* _state)
: QWidget(parent),
  commandManager(_commandManager),
  state(_state),
  ui(new Ui::DialogStateView())
{
  ui->setupUi(this);
  connect(ui->lwTurns, SIGNAL(currentRowChanged(int)), this, SLOT(setCurrentTurn()));
  connect(ui->lwTurns, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editTurn()));

  stateChanged();

  // To intialize toggle edit state.
  ui->pbEditState->click();
  ui->pbEditState->click();

  connect(state, SIGNAL(changed()), this, SLOT(stateChanged()));
  connect(state, SIGNAL(destroyed()), this, SLOT(stateDestroyed()));
  connect(ui->pbEditState, SIGNAL(clicked()), this, SLOT(editState()));
  connect(ui->pbRemoveState, SIGNAL(clicked()), this, SLOT(removeState()));
  connect(ui->pbAddTurn, SIGNAL(clicked()), this, SLOT(addTurn()));
  connect(ui->pbEditTurn, SIGNAL(clicked()), this, SLOT(editTurn()));
  connect(ui->pbRemoveTurn, SIGNAL(clicked()), this, SLOT(removeTurn()));
  ui->pbRemoveState->setIcon(KIcon("list-remove"));

  // ui.pbAddTransition->setIcon(KIcon("list-add"));
  // ui.pbRemoveTransition->setIcon(KIcon("list-remove"));
  // ui.pbMoveStateUp->setIcon(KIcon("arrow-up"));
  // ui.pbMoveStateDown->setIcon(KIcon("arrow-down"));
  // ui.pbEditTransition->setIcon(KIcon("document-edit"));
  // ui.pbEditText->setIcon(KIcon("document-edit")); 
  // ui.pbMoveTransitionUp->setIcon(KIcon("arrow-up"));
  ui->pbAddTurn->setIcon(KIcon("list-add"));
  ui->pbEditTurn->setIcon(KIcon("document-edit"));
  ui->pbRemoveTurn->setIcon(KIcon("list-remove"));
  // ui.pbMoveTransitionDown->setIcon(KIcon("arrow-down"));
  ui->pbAddRequiredField->setIcon(KIcon("list-add"));
  ui->pbRemoveRequiredField->setIcon(KIcon("list-remove"));
  // ui.pbAddText->setIcon(KIcon("list-add"));
  // ui.pbRemoveText->setIcon(KIcon("list-remove"));
}

void DialogStateConfiguration::stateChanged()
{
  kDebug() << "stateChanged";
  //TODO: What do you think about this? I don't like the idea of having state
  // communicate to stateconfiguration that it changed because
  // stateconfiguration told state to change.
  ui->gbStateName->setTitle(state->getName());
  ui->leStateName->setText(state->getName());

  ui->lwTurns->clear();
  QList<DialogTurn*> turns = state->getTurns();
  int id = 1;
  foreach (DialogTurn* turn, turns)
  {
    ui->lwTurns->addItem(i18nc("%1: id of turn; %2: name of turn", "%1: %2", id, turn->getName()));
    ++id;
  }
}

void DialogStateConfiguration::editState()
{
  if (ui->pbEditState->isChecked()) {
    //TODO: does anything need to be done when going into edit mode?
  } else {
    //TODO: save name of state, turns, fields
    //What if this state gets deleted?
    //Adding/saving/deleting of turns gets done by this point
    state->rename(ui->leStateName->text());
  }
}

void DialogStateConfiguration::removeState()
{
  //TODO: is this check necessary? No, I don't think so.
  if (!state) return;

  if (!state ||
      KMessageBox::questionYesNoCancel(this, 
        i18n("Do you really want to remove this state?"))
      != KMessageBox::Yes)
    return;

  if (!commandManager->removeState(state))
    KMessageBox::sorry(this, i18n("Failed to remove state"));

  emit deleted();
}

void DialogStateConfiguration::stateDestroyed()
{
  //TODO: remove self from UI ??
  //When a state is removed code flow ends up here.
  kDebug() << "stateDestroyed";
}

void DialogStateConfiguration::addTurn()
{
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
  stateChanged();
  //TODO: redraw this new added turn?
  // displayCurrentState();
}

void DialogStateConfiguration::editTurn()
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
      state->setTurn(clone, ui->lwTurns->currentRow());
    }
    else
    {
      delete clone;
    }
    delete turnConfig;
    stateChanged();
  }
}

void DialogStateConfiguration::removeTurn()
{
  DialogTurn* turn = getCurrentTurn();
  if (!turn)
  {
    KMessageBox::sorry(this, i18n("No turn selected."));
  }
  else
  {
    if (KMessageBox::questionYesNoCancel(this,
      i18n("Do you really want to remove the selected turn?"))
        != KMessageBox::Yes)
      return;
    //TODO: Finish this
  }
}

void DialogStateConfiguration::setCurrentTurn()
{
  state->setCurrentTurn(ui->lwTurns->currentRow());
}

DialogTurn* DialogStateConfiguration::getCurrentTurn()
{
  int row = ui->lwTurns->currentRow();

  if (row == -1) return 0;

  QList<DialogTurn*> turns = state->getTurns();

  return turns[row];
}

void DialogStateConfiguration::init()
{
  // displayStates();
}


DialogStateConfiguration::~DialogStateConfiguration()
{
}
