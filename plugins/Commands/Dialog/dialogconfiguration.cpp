/*   Copyright (C) 2009-2010 Grasch Peter <grasch@simon-listens.org>
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

#include "dialogtemplateoptions.h"
#include "dialogboundvalues.h"

#include "boundvalue.h"
#include "dialogstate.h"
#include "dialogcommand.h"
#include "createdialogcommandwidget.h"
#include "createtransitiondialog.h"
#include "createtemplateoptiondialog.h"
#include "createboundvaluedialog.h"
#include "avatarmodel.h"
#include "avatar.h"

#include <simonscenarios/scenario.h>

#include <QVariantList>
#include <QList>
#include <QString>
#include <QDialog>
#include <QTableView>
#include <QThread>
#include <QApplication>

#include <kgenericfactory.h>
#include <KAboutData>
#include <KInputDialog>
#include <KMessageBox>
#include <KStandardDirs>
#include "createavatardialog.h"

K_PLUGIN_FACTORY_DECLARATION(DialogCommandPluginFactory)

DialogConfiguration::DialogConfiguration(DialogCommandManager* _commandManager, Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "dialog", ki18n( "Dialog" ),
  "0.1", ki18n("Control a robot"),
  "im-user",
  DialogCommandPluginFactory::componentData()),
  commandManager(_commandManager),
  templateOptions(0),
  boundValues(0),
  avatarModel(0)
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
  
  connect(ui.pbAddAvatar, SIGNAL(clicked()), this, SLOT(addAvatar()));
  connect(ui.pbEditAvatar, SIGNAL(clicked()), this, SLOT(editAvatar()));
  connect(ui.pbRemoveAvatar, SIGNAL(clicked()), this, SLOT(removeAvatar()));

  connect(ui.pbMoveTransitionUp, SIGNAL(clicked()), this, SLOT(moveTransitionUp()));
  connect(ui.pbMoveTransitionDown, SIGNAL(clicked()), this, SLOT(moveTransitionDown()));

  connect(ui.pbAddTemplateOption, SIGNAL(clicked()), this, SLOT(addTemplateOption()));
  connect(ui.pbEditTemplateOption, SIGNAL(clicked()), this, SLOT(editTemplateOption()));
  connect(ui.pbRemoveTemplateOption, SIGNAL(clicked()), this, SLOT(removeTemplateOption()));

  connect(ui.pbAddBoundValue, SIGNAL(clicked()), this, SLOT(addBoundValue()));
  connect(ui.pbEditBoundValue, SIGNAL(clicked()), this, SLOT(editBoundValue()));
  connect(ui.pbRemoveBoundValue, SIGNAL(clicked()), this, SLOT(removeBoundValue()));

  ui.pbAddState->setIcon(KIcon("list-add"));
  ui.pbAddTransition->setIcon(KIcon("list-add"));
  ui.pbAddBoundValue->setIcon(KIcon("list-add"));
  ui.pbAddTemplateOption->setIcon(KIcon("list-add"));
  ui.pbAddAvatar->setIcon(KIcon("list-add"));

  ui.pbRemoveState->setIcon(KIcon("list-remove"));
  ui.pbRemoveTransition->setIcon(KIcon("list-remove"));
  ui.pbRemoveBoundValue->setIcon(KIcon("list-remove"));
  ui.pbRemoveTemplateOption->setIcon(KIcon("list-remove"));

  ui.pbRenameState->setIcon(KIcon("document-edit"));
  ui.pbEditTransition->setIcon(KIcon("document-edit"));
  ui.pbEditText->setIcon(KIcon("document-edit"));
  ui.pbEditBoundValue->setIcon(KIcon("document-edit"));
  ui.pbEditTemplateOption->setIcon(KIcon("document-edit"));
  ui.pbEditAvatar->setIcon(KIcon("document-edit"));
  ui.pbRemoveAvatar->setIcon(KIcon("list-remove"));

  ui.pbMoveStateUp->setIcon(KIcon("arrow-up"));
  ui.pbMoveTransitionUp->setIcon(KIcon("arrow-up"));

  ui.pbMoveStateDown->setIcon(KIcon("arrow-down"));
  ui.pbMoveTransitionDown->setIcon(KIcon("arrow-down"));

  displayCurrentState();
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
    KMessageBox::sorry(this, i18n("Failed to move state up.\n\nMaybe it is already at the end of the list?"));
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
                                        state->getRawText(), &ok);
  if (!ok) return;
  
  if (!state->setRawText(text))
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
  displayCurrentState();
}
 

void DialogConfiguration::addTransition()
{
  DialogState *state = getCurrentStateGraphical();
  if (!state)
    return;

  CreateDialogCommandWidget *create = new CreateDialogCommandWidget(commandManager, this);
  CreateTransitionDialog *dialog = new CreateTransitionDialog(create, this);

  DialogCommand *transition = dialog->createTransition();
  ((Command*) transition)->setParent(commandManager);

  delete create;
  delete dialog;

  if (!transition) return;

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


QString DialogConfiguration::getCurrentTemplateIndex()
{
  QModelIndex index = ui.tvTemplateOptions->currentIndex();
  if (!index.isValid())
    return QString();
  QString id = index.data(Qt::UserRole).toString();
  return id;
}

QString DialogConfiguration::getCurrentTemplateIndexGraphical()
{
  QString id = getCurrentTemplateIndex();
  if (id.isNull())
    KMessageBox::sorry(this, i18n("Please select a template option."));
  return id;
}

void DialogConfiguration::addTemplateOption()
{
  QPointer<CreateTemplateOptionDialog> dialog = new CreateTemplateOptionDialog(this);
  if (dialog->exec())
    templateOptions->addOption(dialog->getName(), dialog->getEnabled());
  if (dialog) dialog->deleteLater();
}

void DialogConfiguration::editTemplateOption()
{
  QString id = getCurrentTemplateIndexGraphical();
  if (id.isNull()) return;

  bool currentData = templateOptions->isEnabled(id);

  QPointer<CreateTemplateOptionDialog> dialog = new CreateTemplateOptionDialog(this);

  dialog->setName(id);
  dialog->setNameReadOnly(true);
  dialog->setEnabled(currentData);

  if (dialog->exec())
    //will automatically replace the old value because they share the same id
    templateOptions->addOption(dialog->getName(), dialog->getEnabled());

  if (dialog) dialog->deleteLater();
}

void DialogConfiguration::removeTemplateOption()
{
  QString id = getCurrentTemplateIndexGraphical();

  if (id.isNull() || KMessageBox::questionYesNoCancel(this, 
        i18n("Do you really want to remove the selected template option?"))
      != KMessageBox::Yes)
    return;

  templateOptions->removeOption(id);
}

BoundValue* DialogConfiguration::getCurrentBoundValue()
{
  return static_cast<BoundValue*>(ui.tvBoundValues->currentIndex().internalPointer());
}

BoundValue* DialogConfiguration::getCurrentBoundValueGraphical()
{
  BoundValue *value = getCurrentBoundValue();
  if (!value)
    KMessageBox::information(this, i18n("Please select a bound value from the table above."));
  return value;
}

void DialogConfiguration::addBoundValue()
{
  CreateBoundValueDialog *dialog = new CreateBoundValueDialog(this);

  BoundValue *boundValue = dialog->createBoundValue();
 
  if (!boundValue)
    return;

  boundValues->addBoundValue(boundValue);
  delete dialog;
}

void DialogConfiguration::editBoundValue()
{
  BoundValue *value = getCurrentBoundValueGraphical();
  if (!value) return;

  CreateBoundValueDialog *dialog = new CreateBoundValueDialog(this);
  BoundValue *newValue = dialog->createBoundValue(value);
  if (newValue)
  {
    if (!boundValues->removeBoundValue(value))
      delete value;
    if (!boundValues->addBoundValue(newValue))
      KMessageBox::sorry(this, i18n("Could not edit bound value."));
  }
  delete dialog;
}

void DialogConfiguration::removeBoundValue()
{
  BoundValue *value = getCurrentBoundValueGraphical();
  if (!value || KMessageBox::questionYesNoCancel(this, 
        i18n("Do you really want to remove the selected bound value option?"))
      != KMessageBox::Yes) return;

  boundValues->removeBoundValue(value);
}


    
QDomElement DialogConfiguration::serialize(QDomDocument* doc)
{
  QDomElement configElem = doc->createElement("config");

  QDomElement options = templateOptions->serialize(doc);
  configElem.appendChild(options);

  QDomElement boundValuesElem = boundValues->serialize(doc);
  configElem.appendChild(boundValuesElem);

  QDomElement avatarsElem = avatarModel->serialize(doc);
  configElem.appendChild(avatarsElem);

  QDomElement outputElem = doc->createElement("output");
  QDomElement graphicalOutput = doc->createElement("gui");
  QDomElement ttsOutput = doc->createElement("tts");
  graphicalOutput.appendChild(doc->createTextNode(ui.cbGraphical->isChecked() ? "1" : "0"));
  ttsOutput.appendChild(doc->createTextNode(ui.cbTextToSpeech->isChecked() ? "1" : "0"));

  outputElem.appendChild(graphicalOutput);
  outputElem.appendChild(ttsOutput);

  configElem.appendChild(outputElem);

  QDomElement ttsOptions = doc->createElement("ttsOptions");
  QDomElement separatorElem = doc->createElement("optionsSeparator");
  separatorElem.appendChild(doc->createTextNode(ui.leOptionSeparator->text()));
  ttsOptions.appendChild(separatorElem);
      
  QDomElement repeatTriggersElem = doc->createElement("repeatTriggers");
  QStringList repeatTriggers = ui.elwRepeatTriggers->items();
  foreach (const QString& trigger, repeatTriggers)
  {
    QDomElement repeatTriggerElem = doc->createElement("repeatTrigger");
    repeatTriggerElem.appendChild(doc->createTextNode(trigger));
    repeatTriggersElem.appendChild(repeatTriggerElem);
  }
  ttsOptions.appendChild(repeatTriggersElem);

  QDomElement announceRepeatElem = doc->createElement("announceRepeat");
  announceRepeatElem.appendChild(doc->createTextNode(ui.leAnnounceRepeat->text()));
  ttsOptions.appendChild(announceRepeatElem);

  QDomElement repeatOnInvalidInputElem = doc->createElement("repeatOnInvalidInput");
  repeatOnInvalidInputElem.appendChild(doc->createTextNode(ui.cbRepeatOnInvalidInput->isChecked() ? "1" : "0"));
  ttsOptions.appendChild(repeatOnInvalidInputElem);

  outputElem.appendChild(ttsOptions);
  return configElem;
}


bool DialogConfiguration::deSerialize(const QDomElement& elem)
{
  Q_UNUSED(elem);
  QDomElement options = elem.firstChildElement("options");
  if (!templateOptions)
  {
    templateOptions = DialogTemplateOptions::createInstance(options);
    if (!templateOptions)
      return false;
    
    ui.tvTemplateOptions->setModel(templateOptions);
  } else {
    templateOptions->deSerialize(options);
  }

  QDomElement boundValuesElem = elem.firstChildElement("boundValues");
  if (!boundValues)
  {
    boundValues = DialogBoundValues::createInstance(boundValuesElem);
    if (!boundValues)
      return false;
    
    ui.tvBoundValues->setModel(boundValues);
  } else {
    boundValues->deSerialize(options);
  }
  
  QDomElement avatarsElement = elem.firstChildElement("avatars");
  if (!avatarModel)
  {
    avatarModel = AvatarModel::createInstance(avatarsElement);
    if (!avatarModel)
      return false;
    ui.lvAvatars->setModel(avatarModel);
    ui.lvStateAvatar->setModel(avatarModel);
  } else
    avatarModel->deSerialize(avatarsElement);

  QDomElement outputElem = elem.firstChildElement("output");
  if (outputElem.isNull())
  {
    defaults();
    return true;
  }

  QDomElement graphicalOutput = outputElem.firstChildElement("gui");
  QDomElement ttsOutput = outputElem.firstChildElement("tts");
  ui.cbGraphical->setChecked(graphicalOutput.text() == "1");
  ui.cbTextToSpeech->setChecked(ttsOutput.text() == "1");

  QDomElement ttsOptions = outputElem.firstChildElement("ttsOptions");
  QDomElement separatorElem = ttsOptions.firstChildElement("optionsSeparator");
  ui.leOptionSeparator->setText(separatorElem.text());
  QDomElement repeatTriggers = ttsOptions.firstChildElement("repeatTriggers");
  QDomElement repeatTrigger = repeatTriggers.firstChildElement("repeatTrigger");
  QStringList repeatTriggersStrs;
  while (!repeatTrigger.isNull())
  {
    repeatTriggersStrs << repeatTrigger.text();
    repeatTrigger = repeatTrigger.nextSiblingElement("repeatTrigger");
  }
  ui.elwRepeatTriggers->setItems(repeatTriggersStrs);

  QDomElement announceRepeatElem = ttsOptions.firstChildElement("announceRepeat");
  ui.leAnnounceRepeat->setText(announceRepeatElem.text());

  QDomElement repeatOnInvalidInputElem = ttsOptions.firstChildElement("repeatOnInvalidInput");
  ui.cbRepeatOnInvalidInput->setChecked(repeatOnInvalidInputElem.text() == "1");

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

  ui.teText->setText(currentState->getRawText());
  ui.cbSilence->setChecked(currentState->silence());
  ui.cbAnnounceRepeat->setChecked(currentState->announceRepeat());

  ui.lvTransitions->setModel(currentState);
}

void DialogConfiguration::defaults()
{
  ui.cbGraphical->setChecked(true);
  ui.cbTextToSpeech->setChecked(false);
  ui.leOptionSeparator->setText(i18n("Please answer with any of the following options."));

  ui.elwRepeatTriggers->setItems(QStringList() << i18n("Repeat"));
  ui.leAnnounceRepeat->setText(i18n("Say \"Repeat\" to hear this text again."));

  ui.cbRepeatOnInvalidInput->setChecked(true);
}

bool DialogConfiguration::useGUIOutput() const
{
  return ui.cbGraphical->isChecked();
}

bool DialogConfiguration::useTTSOutput() const
{
  return ui.cbTextToSpeech->isChecked();
}


QString DialogConfiguration::getOptionSeparatorText() const
{
  return ui.leOptionSeparator->text();
}

QString DialogConfiguration::getRepeatAnnouncement() const
{
  return ui.leAnnounceRepeat->text();
}

QStringList DialogConfiguration::getRepeatTriggers() const
{
  return ui.elwRepeatTriggers->items();
}

bool DialogConfiguration::getRepeatOnInvalidInput() const
{
  return ui.cbRepeatOnInvalidInput->isChecked();
}

void DialogConfiguration::addAvatar()
{
  CreateAvatarDialog *dlg = new CreateAvatarDialog(this);
  dlg->addAvatar(avatarModel);
  delete dlg;
}
  
Avatar* DialogConfiguration::getCurrentAvatar()
{
  QModelIndex currentIndex = ui.lvAvatars->currentIndex();
  if (!currentIndex.isValid()) return 0;
  
  return static_cast<Avatar*>(currentIndex.internalPointer());
}

Avatar* DialogConfiguration::getCurrentAvatarGraphical()
{
  Avatar *a = getCurrentAvatar();
  if (!a)
    KMessageBox::information(this, i18n("Please select an existing avatar from the list or add a new one as appropriate."));
  return a;
}

void DialogConfiguration::editAvatar()
{
  Avatar *a = getCurrentAvatarGraphical();
  CreateAvatarDialog *dlg = new CreateAvatarDialog(this);
  dlg->editAvatar(a);
  delete dlg;
}
void DialogConfiguration::removeAvatar()
{
  Avatar *a = getCurrentAvatarGraphical();
  if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to remove the avatar \"%1\" from your dialog?", a->name())) == KMessageBox::Yes)
  {
    if (!avatarModel->removeAvatar(a))
      KMessageBox::sorry(this, i18n("Failed to remove avatar."));
    else delete a;
  }
}


DialogConfiguration::~DialogConfiguration()
{
}
