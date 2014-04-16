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
#include "dialogstateconfiguration.h"

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
#include <QLayoutItem>

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

  connect(ui.pbAddState, SIGNAL(clicked()), this, SLOT(addState()));

  ui.twMain->addTab(boundValuesConfig, i18n("Bound values"));
  ui.twMain->addTab(templateOptionsConfig, i18n("Template options"));
  ui.twMain->addTab(avatarsConfig, i18n("Avatars"));
  ui.twMain->addTab(outputConfiguration, i18n("Output"));

  ui.pbAddState->setIcon(KIcon("list-add"));
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
  addStateGraphical(commandManager->getStates().back());
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

void DialogConfiguration::removeState()
{
  int index = ui.hlStates->indexOf(static_cast<DialogStateConfiguration*>(sender()));
  QLayoutItem* item = ui.hlStates->takeAt(index);
  delete item->widget();
  delete item;
}

void DialogConfiguration::addStateGraphical(DialogState* state)
{
  DialogStateConfiguration* stateConfig = new DialogStateConfiguration(this, commandManager, state);
  stateConfig->setMinimumWidth(stateConfig->sizeHint().width());
  connect(stateConfig, SIGNAL(deleted()), this, SLOT(removeState()));
  ui.hlStates->addWidget(stateConfig);
}

void DialogConfiguration::displayStates()
{
  // Should it do as before and delete all the states that are present and just
  // reconstruct them from the list of states of the commandManager?

  QLayoutItem* item;
  while ((item = ui.hlStates->takeAt(0)) != NULL)
  {
    delete item->widget();
    delete item;
  }

  QList<DialogState*> states = commandManager->getStates();
  foreach (DialogState* state, states)
  {
    addStateGraphical(state);
  }
}

DialogState* DialogConfiguration::getCurrentState()
{
  // int row = ui.lwStates->currentRow();

  // if (row == -1) return 0;

  //TODO: There's no more current state with this new ui right?
  // Every method should get called on a specific state now I believe.

  QList<DialogState*> states = commandManager->getStates();

  return states[0];
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
