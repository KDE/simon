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

#include "createakonadicommandwidget.h"
#include "akonadicommand.h"
#include <simonactions/actionmanager.h>
#include <KLineEdit>
#include <KMessageBox>
#include <akonadi/attribute.h>

CreateAkonadiCommandWidget::CreateAkonadiCommandWidget(CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent),
allCommands(ActionManager::getInstance()->getCommandList())
{
  ui.setupUi(this);

  setWindowIcon(AkonadiCommand::staticCategoryIcon());
  setWindowTitle(AkonadiCommand::staticCategoryText());

  foreach (const Command* com, allCommands) {
    QString name = com->getTrigger();
    QString category = com->getCategoryText();
    ui.cbCommand->addItem(com->getIcon(), name+" ("+category+')');
  }
  
  connect(ui.cbCommand, SIGNAL(currentIndexChanged(int)), this, SIGNAL(completeChanged()));
}


bool CreateAkonadiCommandWidget::isComplete()
{
  return (ui.cbCommand->currentIndex() != -1);
}


bool CreateAkonadiCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  AkonadiCommand *akonadiCommand = dynamic_cast<AkonadiCommand*>(command);
  if (!akonadiCommand) return false;
  
  bool found = false;
  foreach (Command* com, allCommands) {
    if ((com->getTrigger() == akonadiCommand->getChildTrigger()) &&
    (com->getCategoryText() == akonadiCommand->getChildType())) {
      //found the command
      ui.cbCommand->setCurrentIndex(allCommands.indexOf(com));
      found = true;
      break;
    }
  }

  if (!found) {
    KMessageBox::sorry(this, i18nc("%1 is child command trigger; %2 is child command type", "Could not find child command: %1 (%2).", akonadiCommand->getChildTrigger(), 
				  akonadiCommand->getChildType()));
  }
  
  ui.rbAbsolute->setChecked(akonadiCommand->getType() == AkonadiCommand::Absolute);
  ui.rbRelative->setChecked(akonadiCommand->getType() != AkonadiCommand::Absolute);

  ui.dtAbsoluteTime->setDateTime(akonadiCommand->getAbsoluteExecutionTime().dateTime());
  AkonadiCommand::RelativeDurationDimension dimension;
  int relativeValue;
  akonadiCommand->getRelativeTime(dimension, relativeValue);
  ui.wgRelativeTime->setTime(dimension,relativeValue);

  return found;
}


Command* CreateAkonadiCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  Command *childCommand = allCommands.at(ui.cbCommand->currentIndex());
  AkonadiCommand::TimerType type;
  if (ui.rbAbsolute->isChecked())
    type = AkonadiCommand::Absolute;
  else
    type = AkonadiCommand::Relative;
  
  int relativeTime =  ui.wgRelativeTime->getTime();
  
  return new AkonadiCommand(name, iconSrc, description, childCommand->getTrigger(), 
			    childCommand->getCategoryText(), type, 
			    KDateTime(ui.dtAbsoluteTime->dateTime()), relativeTime);
}


CreateAkonadiCommandWidget::~CreateAkonadiCommandWidget()
{
}
