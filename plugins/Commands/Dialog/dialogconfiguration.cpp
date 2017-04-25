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
#include "dialogcommandmanager.h"

#include <simondialogengine/dialogtemplateoptions.h>
#include <simondialogengine/dialogboundvalues.h>

#include <simondialogengine/avatar.h>
#include <simondialogengine/avatarmodel.h>
#include <simondialogengine/boundvalue.h>
#include <simondialogengine/dialogstate.h>
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
#include <QThread>
#include <KMessageBox>

#include <KPluginFactory>
#include <KInputDialog>
#include <QDebug>

#include <simondialogengine/confui/outputconfiguration.h>

K_PLUGIN_FACTORY_DECLARATION(DialogCommandPluginFactory)

DialogConfiguration::DialogConfiguration(DialogCommandManager* _commandManager, Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "dialog", i18n( "Dialog" ),
  "0.1", i18n("Control a robot"),
  "im-user"),
  commandManager(_commandManager),
  boundValuesConfig(new BoundValuesConfiguration(this)),
  templateOptionsConfig(new TemplateOptionsConfiguration(this)),
  avatarsConfig(new AvatarConfiguration(this)),
  outputConfiguration(new OutputConfiguration(this))
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

  connect(ui.cbSilence, SIGNAL(toggled(bool)), this, SLOT(textSilenceChanged()));
  connect(ui.cbAnnounceRepeat, SIGNAL(toggled(bool)), this, SLOT(textAnnounceRepeatChanged()));

  connect(ui.pbAddTransition, SIGNAL(clicked()), this, SLOT(addTransition()));
  connect(ui.pbEditTransition, SIGNAL(clicked()), this, SLOT(editTransition()));
  connect(ui.pbRemoveTransition, SIGNAL(clicked()), this, SLOT(removeTransition()));

  connect(ui.pbMoveTransitionUp, SIGNAL(clicked()), this, SLOT(moveTransitionUp()));
  connect(ui.pbMoveTransitionDown, SIGNAL(clicked()), this, SLOT(moveTransitionDown()));
  
  connect(ui.cbDisplayAvatar, SIGNAL(toggled(bool)), this, SLOT(avatarDisplayToggled(bool)));
  
  connect(ui.sbText, SIGNAL(valueChanged(int)), this, SLOT(displaySelectedText()));
  connect(ui.pbAddText, SIGNAL(clicked()), this, SLOT(addText()));
  connect(ui.pbRemoveText, SIGNAL(clicked()), this, SLOT(removeText()));

  connect(ui.lvStateAvatar, SIGNAL(clicked(QModelIndex)), this, SLOT(avatarSelected(QModelIndex)));

  ui.twMain->addTab(boundValuesConfig, i18n("Bound values"));
  ui.twMain->addTab(templateOptionsConfig, i18n("Template options"));
  ui.twMain->addTab(avatarsConfig, i18n("Avatars"));
  ui.twMain->addTab(outputConfiguration, i18n("Output"));

  ui.pbAddState->setIcon(QIcon::fromTheme("list-add"));
  ui.pbAddTransition->setIcon(QIcon::fromTheme("list-add"));

  ui.pbRemoveState->setIcon(QIcon::fromTheme("list-remove"));
  ui.pbRemoveTransition->setIcon(QIcon::fromTheme("list-remove"));

  ui.pbRenameState->setIcon(QIcon::fromTheme("document-edit"));
  ui.pbEditTransition->setIcon(QIcon::fromTheme("document-edit"));
  ui.pbEditText->setIcon(QIcon::fromTheme("document-edit"));

  ui.pbMoveStateUp->setIcon(QIcon::fromTheme("arrow-up"));
  ui.pbMoveTransitionUp->setIcon(QIcon::fromTheme("arrow-up"));

  ui.pbMoveStateDown->setIcon(QIcon::fromTheme("arrow-down"));
  ui.pbMoveTransitionDown->setIcon(QIcon::fromTheme("arrow-down"));

  ui.pbAddText->setIcon(QIcon::fromTheme("list-add"));
  ui.pbRemoveText->setIcon(QIcon::fromTheme("list-remove"));
  displayCurrentState();
}

void DialogConfiguration::avatarSelected ( const QModelIndex& selected )
{
  if (!selected.isValid())
  {
    getCurrentState()->setAvatar(0);
  } else {
    Avatar *a = static_cast<Avatar*>(selected.internalPointer());
    qDebug() << "Selected avatar: " << a->name();
    getCurrentState()->setAvatar(a->id());
  }
}

void DialogConfiguration::updateTextSelector()
{
  DialogState *state = getCurrentState();
  if (!state) return;
  
  ui.sbText->setMaximum(state->getTextCount());
  
  displaySelectedText();
  ui.pbRemoveText->setEnabled(state->getTextCount() > 1);
}

void DialogConfiguration::displaySelectedText()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state) return;
  int textId = ui.sbText->value()-1;
  qDebug() << "Getting text " << textId;
  ui.teText->setText(state->getRawText(textId));
}

void DialogConfiguration::addText()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state) return;

  state->addText("");
  updateTextSelector();
  ui.sbText->setValue(ui.sbText->maximum());
  displaySelectedText();
}

void DialogConfiguration::removeText()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state) return;
  
  if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to remove the selected text variant?")) != KMessageBox::Yes)
    return;
  
  if (state->getTextCount() == 1)
  {
    KMessageBox::information(this, i18n("Each dialog state has to have at least one text."));
    return;
  }
  if (!state->removeText(ui.sbText->value()-1))
    KMessageBox::sorry(this, i18n("Could not remove text from state."));
  updateTextSelector();
}

void DialogConfiguration::addState()
{
  bool ok = true;
  QString name = KInputDialog::getText(i18n("Add state"), i18n("Name of the new state:"), 
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
  DialogState *state = getCurrentStateGraphical();
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


void DialogConfiguration::editText()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state)
    return;

  bool ok;
  QString text = KInputDialog::getMultiLineText(i18n("Text"), i18n("Enter the text to present to the user when this state is entered:"), 
                                        state->getRawText(ui.sbText->value()-1), &ok);
  if (!ok) return;
  
  if (!state->setRawText(ui.sbText->value()-1, text))
    KMessageBox::sorry(this, i18n("Failed to update state text."));

  displayCurrentState();
}

void DialogConfiguration::textSilenceChanged()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state)
    return;

  state->setSilence(ui.cbSilence->isChecked());
  displayCurrentState();
}

void DialogConfiguration::textAnnounceRepeatChanged()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state)
    return;

  state->setAnnounceRepeat(ui.cbAnnounceRepeat->isChecked());
  //displayCurrentState();
}
 

void DialogConfiguration::addTransition()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state)
    return;

  CreateDialogCommandWidget *create = new CreateDialogCommandWidget(commandManager, this);
  CreateTransitionDialog *dialog = new CreateTransitionDialog(create, this);

  DialogCommand *transition = dialog->createTransition();

  delete create;
  delete dialog;

  if (!transition) return;

  ((Command*) transition)->setParent(commandManager);
  state->addTransition(transition);
}

void DialogConfiguration::editTransition()
{
  DialogState *state = getCurrentStateGraphical();
  DialogCommand *transition = getCurrentTransitionGraphical();
  if (!state || !transition)
    return;

  CreateDialogCommandWidget *create = new CreateDialogCommandWidget(commandManager, this);
  CreateTransitionDialog *dialog = new CreateTransitionDialog(create, this);

  dialog->editTransition(transition);

  delete create;
  delete dialog;
}

void DialogConfiguration::removeTransition()
{
  DialogState *state = getCurrentStateGraphical();
  DialogCommand *transition = getCurrentTransitionGraphical();
  if (!state || !transition || 
      KMessageBox::questionYesNoCancel(this, 
        i18n("Do you really want to remove the selected transition?"))
      != KMessageBox::Yes)
    return;

  state->removeTransition(transition);
}

void DialogConfiguration::moveTransitionUp()
{
  DialogState *state = getCurrentStateGraphical();
  DialogCommand *transition = getCurrentTransitionGraphical();
  if (!state || !transition)
    return;

  QItemSelectionModel *model = ui.lvTransitions->selectionModel();
  int row = model->selectedRows().at(0).row();

  if (!state->moveTransitionUp(transition))
    KMessageBox::sorry(this, i18n("Failed to move transition up.\n\nMaybe it is already at the top of the list?"));

  model->select(ui.lvTransitions->model()->index(row-1, 0),
                          QItemSelectionModel::ClearAndSelect);
}

void DialogConfiguration::moveTransitionDown()
{
  DialogState *state = getCurrentStateGraphical();
  DialogCommand *transition = getCurrentTransitionGraphical();
  if (!state || !transition)
    return;

  QItemSelectionModel *model = ui.lvTransitions->selectionModel();
  int row = model->selectedRows().at(0).row();

  if (!state->moveTransitionDown(transition))
    KMessageBox::sorry(this, i18n("Failed to move transition down.\n\nMaybe it is already at the end of the list?"));

  model->select(ui.lvTransitions->model()->index(row+1, 0),
                          QItemSelectionModel::ClearAndSelect);
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
    qDebug() << "Setting defaults...";
    return true;
  }
  
  if (!templateOptionsConfig->deSerialize(elem))
    return false;

  if (!boundValuesConfig->deSerialize(elem))
    return false;

  if (!avatarsConfig->deSerialize(elem))
    return false;
  ui.lvStateAvatar->setModel(avatarsConfig->getModel());

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

  ui.wgText->setEnabled(currentState);
  ui.wgOptions->setEnabled(currentState);
  ui.wgAvatar->setEnabled(currentState);

  if (!currentState) 
  {
    ui.teText->clear();
    ui.lvTransitions->setModel(0);
    return;
  }

  updateTextSelector();
  ui.cbSilence->setChecked(currentState->silence());
  ui.cbAnnounceRepeat->setChecked(currentState->announceRepeat());
  
  ui.cbDisplayAvatar->setChecked(currentState->getDisplayAvatar());
  ui.lvStateAvatar->setEnabled(ui.cbDisplayAvatar->isChecked());
  int avatarId = currentState->getAvatarId();
  QModelIndex avatarIndex = avatarsConfig->getAvatarIndex(avatarId);
  
  qDebug() << "Avatar index: " << avatarIndex;
  //FIXME!
  //ui.lvStateAvatar->selectionModel()->select(avatarIndex,
    //QItemSelectionModel::ClearAndSelect);

  qDebug() << currentState;
  ui.lvTransitions->setModel(currentState);
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

void DialogConfiguration::avatarDisplayToggled ( bool show )
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
