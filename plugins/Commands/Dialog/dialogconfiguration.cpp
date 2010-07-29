/*   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
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
#include "dialogcommandmanager.h"

#include "dialogstate.h"
#include "dialogcommand.h"
#include "createdialogcommandwidget.h"
#include "createtransitiondialog.h"

#include <QVariantList>
#include <kgenericfactory.h>
#include <QList>
#include <KAboutData>
#include <KMessageBox>
#include <KStandardDirs>
#include <QString>
#include <QDialog>
#include <KInputDialog>
#include <QTableView>
#include <QThread>
#include <QApplication>
#include <simonscenarios/scenario.h>

K_PLUGIN_FACTORY_DECLARATION(DialogCommandPluginFactory)

DialogConfiguration::DialogConfiguration(DialogCommandManager* _commandManager, Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "dialog", ki18n( "Dialog" ),
  "0.1", ki18n("Control a robot"),
  "im-user",
  DialogCommandPluginFactory::componentData()),
  commandManager(_commandManager)
{
  Q_UNUSED(args);
  ui.setupUi(this);

  connect(ui.lwStates, SIGNAL(currentRowChanged(int)), this, SLOT(displayCurrentState()));

  connect(ui.pbAddState, SIGNAL(clicked()), this, SLOT(addState()));
  connect(ui.pbRenameState, SIGNAL(clicked()), this, SLOT(renameState()));
  connect(ui.pbRemoveState, SIGNAL(clicked()), this, SLOT(removeState()));

  connect(ui.pbMoveStateUp, SIGNAL(clicked()), this, SLOT(moveStateUp()));
  connect(ui.pbMoveStateDown, SIGNAL(clicked()), this, SLOT(moveStateDown()));

  connect(ui.pbEditText, SIGNAL(clicked()), this, SLOT(editText()));

  connect(ui.pbAddTransition, SIGNAL(clicked()), this, SLOT(addTransition()));
  connect(ui.pbEditTransition, SIGNAL(clicked()), this, SLOT(editTransition()));
  connect(ui.pbRemoveTransition, SIGNAL(clicked()), this, SLOT(removeTransition()));

  connect(ui.pbMoveTransitionUp, SIGNAL(clicked()), this, SLOT(moveTransitionUp()));
  connect(ui.pbMoveTransitionDown, SIGNAL(clicked()), this, SLOT(moveTransitionDown()));

  ui.pbAddState->setIcon(KIcon("list-add"));
  ui.pbAddTransition->setIcon(KIcon("list-add"));

  ui.pbRemoveState->setIcon(KIcon("list-remove"));
  ui.pbRemoveTransition->setIcon(KIcon("list-remove"));

  ui.pbRenameState->setIcon(KIcon("document-edit"));
  ui.pbEditTransition->setIcon(KIcon("document-edit"));
  ui.pbEditText->setIcon(KIcon("document-edit"));

  ui.pbMoveStateUp->setIcon(KIcon("arrow-up"));
  ui.pbMoveTransitionUp->setIcon(KIcon("arrow-up"));

  ui.pbMoveStateDown->setIcon(KIcon("arrow-down"));
  ui.pbMoveTransitionDown->setIcon(KIcon("arrow-down"));
}

void DialogConfiguration::addState()
{
  //TODO
}

void DialogConfiguration::renameState()
{
  //TODO
}

void DialogConfiguration::removeState()
{
  //TODO
}


void DialogConfiguration::moveStateUp()
{
  //TODO
}

void DialogConfiguration::moveStateDown()
{
  //TODO
}


void DialogConfiguration::editText()
{
  //TODO
}
 

void DialogConfiguration::addTransition()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state)
    return;

  CreateDialogCommandWidget *create = new CreateDialogCommandWidget(commandManager, this);
  CreateTransitionDialog *dialog = new CreateTransitionDialog(create, this);

  DialogCommand *transition = dialog->createTransition();

  if (!transition) return;

  state->addTransition(transition);
}

void DialogConfiguration::editTransition()
{
  //TODO

}

void DialogConfiguration::removeTransition()
{
  DialogState *state = getCurrentStateGraphical();
  DialogCommand *transition = getCurrentTransitionGraphical();
  if (!state || !transition)
    return;

  state->removeTransition(transition);
}

void DialogConfiguration::moveTransitionUp()
{
  //TODO
}

void DialogConfiguration::moveTransitionDown()
{
  //TODO
}


QDomElement DialogConfiguration::serialize(QDomDocument* doc)
{
  QDomElement configElem = doc->createElement("config");

  //general
//   QDomElement caseSensitivityElem = doc->createElement("caseSensitivity");
//   caseSensitivityElem.appendChild(doc->createTextNode(ui.cbCaseSensitivity->isChecked() ? "1" : "0"));
//   configElem.appendChild(caseSensitivityElem);

  return configElem;
}


bool DialogConfiguration::deSerialize(const QDomElement& elem)
{
  Q_UNUSED(elem);
//   QDomElement caseSensitivityElem = elem.firstChildElement("caseSensitivity");
//   if (caseSensitivityElem.isNull()) {
//     defaults();
//     return true;
//   }
//   ui.cbCaseSensitivity->setChecked(caseSensitivityElem.text() == "1");

  return true;
}

void DialogConfiguration::init()
{
  displayStates();
}

void DialogConfiguration::displayStates()
{
  ui.lwStates->clear();
  
  QList<DialogState*> states = commandManager->getStates();
  int id = 1;
  foreach (DialogState* state, states)
  {
    ui.lwStates->addItem(i18nc("%1: id of state; %2: name of state", "%1: %2", id, state->getName()));
    id++;
  }
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
    KMessageBox::information(this, i18n("Please select a state from the left or add new ones as appropriate."));

  return state;
}

DialogCommand* DialogConfiguration::getCurrentTransition()
{
  QModelIndex index = ui.lvTransitions->currentIndex();
  if (!index.isValid())
    return 0;

  return static_cast<DialogCommand*>(index.internalPointer());
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
  DialogState *currentState = getCurrentState();
  if (!currentState) return;
  ui.teText->setText(currentState->getText());

  ui.lvTransitions->setModel(currentState);
}

void DialogConfiguration::defaults()
{
  
}


DialogConfiguration::~DialogConfiguration()
{
}
