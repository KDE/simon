/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "createlistcommandwidget.h"
#include <simonactions/listcommand.h>

#include <simonactions/commandtablemodel.h>
#include <simonactions/actionmanager.h>

#include <QListWidget>
#include <QInputDialog>
#include <KMessageBox>
#include <QStringList>

CreateListCommandWidget::CreateListCommandWidget(CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent),
allCommands(ActionManager::getInstance()->getCommandList()),
model(new CommandTableModel())
{
  ui.setupUi(this);

  setWindowIcon(ListCommand::staticCategoryIcon());
  setWindowTitle(ListCommand::staticCategoryText());

  foreach (const Command* com, allCommands) {
    QString name = com->getTrigger();
    QString category = com->getCategoryText();
    ui.cbCommands->addItem(com->getIcon(), name+" ("+category+')');
  }
  ui.tvCommands->setModel(model);

  connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(removeCommand()));
  connect(ui.pbAddCommand, SIGNAL(clicked()), this, SLOT(addCommandToList()));
  connect(ui.pbMoveUp, SIGNAL(clicked()), this, SLOT(moveUp()));
  connect(ui.pbMoveDown, SIGNAL(clicked()), this, SLOT(moveDown()));
  connect(ui.tvCommands, SIGNAL(clicked(QModelIndex)), this, SLOT(enableButtons(QModelIndex)));
  enableButtons(ui.tvCommands->currentIndex());

  ui.pbAddCommand->setIcon(KIcon("list-add"));
  ui.pbRemove->setIcon(KIcon("list-remove"));
  ui.pbMoveUp->setIcon(KIcon("arrow-up"));
  ui.pbMoveDown->setIcon(KIcon("arrow-down"));
}


void CreateListCommandWidget::enableButtons(const QModelIndex& index)
{
  if (!index.isValid()) {
    ui.pbRemove->setEnabled(false);
    ui.pbMoveUp->setEnabled(false);
    ui.pbMoveDown->setEnabled(false);
    return;
  } else
  ui.pbRemove->setEnabled(true);

  ui.pbMoveUp->setEnabled(index.row() > 0);
  ui.pbMoveDown->setEnabled(index.row() < model->rowCount()-1);

}


void CreateListCommandWidget::moveUp()
{
  model->moveUp(ui.tvCommands->currentIndex());
  int row = ui.tvCommands->currentIndex().row();
  ui.tvCommands->selectRow(row-1);
  enableButtons(ui.tvCommands->currentIndex());
}


void CreateListCommandWidget::moveDown()
{
  model->moveDown(ui.tvCommands->currentIndex());
  int row = ui.tvCommands->currentIndex().row();
  ui.tvCommands->selectRow(row+1);
  enableButtons(ui.tvCommands->currentIndex());
}


bool CreateListCommandWidget::isComplete()
{
  return (model->rowCount() > 0);
}


bool CreateListCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  ListCommand *listCommand = dynamic_cast<ListCommand*>(command);
  if (!listCommand) return false;

  QStringList selectedTriggers = listCommand->getCommands();
  QStringList selectedCategories = listCommand->getCommandTypes();
  QStringList selectedIconSrcs = listCommand->getIconSrcs();

  Q_ASSERT(selectedTriggers.count() == selectedCategories.count());
  Q_ASSERT(selectedTriggers.count() == selectedIconSrcs.count());

  QStringList notFound;
  int i=0;
  foreach (const QString& trigger, selectedTriggers) {
    QString cat = selectedCategories[i];
    bool found=false;
    foreach (Command* com, allCommands) {
      if ((com->getTrigger() == trigger) &&
      (com->getCategoryText() == cat)) {
        //found the command
        model->selectCommand(com);
        found=true;
        break;
      }
    }
    if (!found)
      notFound << trigger;
    i++;
  }

  if (!notFound.isEmpty()) {
    KMessageBox::sorry(this, i18n("Could not find all of the commands that make up this list command.\n\n"
      "The missing commands are: %1.", notFound.join(", ")));
  }

  return true;
}


void CreateListCommandWidget::addCommandToList()
{
  model->selectCommand(allCommands.at(ui.cbCommands->currentIndex()));
  enableButtons(ui.tvCommands->currentIndex());
  emit completeChanged();
}


void CreateListCommandWidget::removeCommand()
{
  model->removeCommand(ui.tvCommands->currentIndex().row());
  enableButtons(ui.tvCommands->currentIndex());
  emit completeChanged();
}


Command* CreateListCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  CommandList selectedCommands = model->selectedCommands();
  QStringList selectedIconSrcs , selectedTriggers, selectedCategories;

  foreach (Command* com, selectedCommands) {
    selectedIconSrcs << com->getIconSrc();
    selectedTriggers << com->getTrigger();
    selectedCategories << com->getCategoryText();
  }

  return new ListCommand(m_manager, name, iconSrc, description, selectedTriggers, selectedIconSrcs, selectedCategories);
}


CreateListCommandWidget::~CreateListCommandWidget()
{
  qDeleteAll(commandsToDelete);
}
