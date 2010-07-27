/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include "createdialogcommandwidget.h"
#include "dialogcommand.h"

#include <simonactions/actionmanager.h>
#include <simonactions/commandtablemodel.h>

#include <KLineEdit>
#include <KMessageBox>

CreateDialogCommandWidget::CreateDialogCommandWidget(CommandManager *manager, QWidget* parent) : 
  CreateCommandWidget(manager, parent),
  m_allCommands(ActionManager::getInstance()->getCommandList()),
  m_model(new CommandTableModel())
{
  ui.setupUi(this);

  setWindowIcon(DialogCommand::staticCategoryIcon());
  setWindowTitle(DialogCommand::staticCategoryText());

  foreach (const Command* com, *m_allCommands) {
    QString name = com->getTrigger();
    QString category = com->getCategoryText();
    ui.cbCommands->addItem(com->getIcon(), name+" ("+category+')');
  }
  ui.tvCommands->setModel(m_model);

  connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(removeCommand()));
  connect(ui.pbAddCommand, SIGNAL(clicked()), this, SLOT(addCommandToDialog()));
  connect(ui.pbMoveUp, SIGNAL(clicked()), this, SLOT(moveUp()));
  connect(ui.pbMoveDown, SIGNAL(clicked()), this, SLOT(moveDown()));
  connect(ui.tvCommands, SIGNAL(clicked(const QModelIndex&)), this, SLOT(enableButtons(const QModelIndex&)));
  enableButtons(ui.tvCommands->currentIndex());

  ui.pbAddCommand->setIcon(KIcon("list-add"));
  ui.pbRemove->setIcon(KIcon("list-remove"));
  ui.pbMoveUp->setIcon(KIcon("arrow-up"));
  ui.pbMoveDown->setIcon(KIcon("arrow-down"));
}

void CreateDialogCommandWidget::enableButtons(const QModelIndex& index)
{
  if (!index.isValid()) {
    ui.pbRemove->setEnabled(false);
    ui.pbMoveUp->setEnabled(false);
    ui.pbMoveDown->setEnabled(false);
    return;
  } else
  ui.pbRemove->setEnabled(true);

  Q_ASSERT(m_allCommands);
  ui.pbMoveUp->setEnabled(index.row() > 0);
  ui.pbMoveDown->setEnabled(index.row() < m_model->rowCount()-1);

}


void CreateDialogCommandWidget::moveUp()
{
  m_model->moveUp(ui.tvCommands->currentIndex());
  int row = ui.tvCommands->currentIndex().row();
  ui.tvCommands->selectRow(row-1);
  enableButtons(ui.tvCommands->currentIndex());
}


void CreateDialogCommandWidget::moveDown()
{
  m_model->moveDown(ui.tvCommands->currentIndex());
  int row = ui.tvCommands->currentIndex().row();
  ui.tvCommands->selectRow(row+1);
  enableButtons(ui.tvCommands->currentIndex());
}



bool CreateDialogCommandWidget::isComplete()
{
  return true;
}


bool CreateDialogCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  DialogCommand *dialogCommand = dynamic_cast<DialogCommand*>(command);
  if (!dialogCommand) return false;

  ui.leText->setText(dialogCommand->text());
  ui.cbShowIcon->setChecked(dialogCommand->showIcon());
  ui.gbSwitchState->setChecked(dialogCommand->changeDialogState());
  ui.sbNextState->setValue(dialogCommand->nextDialogState());
  ui.gbCommands->setChecked(dialogCommand->executeCommands());

  QStringList selectedTriggers = dialogCommand->getCommands();
  QStringList selectedCategories = dialogCommand->getCommandTypes();

  Q_ASSERT(selectedTriggers.count() == selectedCategories.count());

  QStringList notFound;
  int i=0;
  foreach (const QString& trigger, selectedTriggers) {
    QString cat = selectedCategories[i];
    bool found=false;
    foreach (Command* com, *m_allCommands) {
      if ((com->getTrigger() == trigger) &&
        (com->getCategoryText() == cat)) {
        //found the command
        m_model->selectCommand(com);
        found=true;
        break;
      }
    }
    if (!found)
      notFound << trigger;

    i++;
  }

  if (!notFound.isEmpty()) {
    KMessageBox::sorry(this, i18n("Could not find all of the commands that are referenced form this dialog command.\n\n"
      "The missing commands are: %1.", notFound.join(", ")));
  }

  return true;
}


void CreateDialogCommandWidget::addCommandToDialog()
{
  m_model->selectCommand(m_allCommands->at(ui.cbCommands->currentIndex()));
  enableButtons(ui.tvCommands->currentIndex());
  emit completeChanged();
}


void CreateDialogCommandWidget::removeCommand()
{
  m_model->removeCommand(ui.tvCommands->currentIndex().row());
  enableButtons(ui.tvCommands->currentIndex());
  emit completeChanged();
}

Command* CreateDialogCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  CommandList *selectedCommands = m_model->selectedCommands();
  QStringList selectedTriggers, selectedCategories;

  foreach (Command* com, *selectedCommands) {
    selectedTriggers << com->getTrigger();
    selectedCategories << com->getCategoryText();
  }

  return new DialogCommand(name, iconSrc, description, 
      ui.leText->text(), ui.cbShowIcon->isChecked(),
      ui.gbSwitchState->isChecked(), ui.sbNextState->value(), 
      ui.gbCommands->isChecked(), selectedTriggers,
      selectedCategories);
}


CreateDialogCommandWidget::~CreateDialogCommandWidget()
{
  delete m_allCommands;
}
