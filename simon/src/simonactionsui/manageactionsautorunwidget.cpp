/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "manageactionsautorunwidget.h"
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/command.h>

ManageActionsAutorunWidget::ManageActionsAutorunWidget(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);
  initCommands();

  Scenario *currentScenario = ScenarioManager::getInstance()->getCurrentScenario();
  ui.cbAutorun->setChecked(currentScenario->autorunActive());

  int index = ui.cbAutoStartCommand->findText(currentScenario->autorunTrigger() + " ("+
      currentScenario->autorunType()+')');

  ui.cbAutoStartCommand->setCurrentIndex((index == -1) ? 0 : index);
}

void ManageActionsAutorunWidget::initCommands()
{
  ui.cbAutoStartCommand->clear();
  ui.cbAutoStartCommand->addItem(KIcon("list-remove"), i18nc("No autostart command", "None"));

  allCommands = ScenarioManager::getInstance()->getCurrentScenario()->getCommandList();
  int i = 1;
  foreach (const Command* com, allCommands) {
    QString name = com->getTrigger();
    QString category = com->getCategoryText();
    ui.cbAutoStartCommand->addItem(com->getIcon(), name+" ("+category+')');
    ui.cbAutoStartCommand->setItemData(i, name, Qt::UserRole);
    ui.cbAutoStartCommand->setItemData(i, category, Qt::UserRole+1);
    i++;
  }
}


void ManageActionsAutorunWidget::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);

  int oldIndex = ui.cbAutoStartCommand->currentIndex();

  initCommands();
  
  if (ui.cbAutoStartCommand->count() > oldIndex)
    ui.cbAutoStartCommand->setCurrentIndex(oldIndex);
}


bool ManageActionsAutorunWidget::save()
{
  int currentRow = ui.cbAutoStartCommand->currentIndex();
  return ScenarioManager::getInstance()->getCurrentScenario()->setAutorun(ui.cbAutorun->isChecked(),
        ui.cbAutoStartCommand->itemData(currentRow, Qt::UserRole+1).toString(),
        ui.cbAutoStartCommand->itemData(currentRow, Qt::UserRole).toString());
}
