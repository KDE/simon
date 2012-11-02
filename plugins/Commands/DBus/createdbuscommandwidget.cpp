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

#include "createdbuscommandwidget.h"
#include "dbuscommand.h"
#include <KLineEdit>

CreateDBusCommandWidget::CreateDBusCommandWidget(CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);

  setWindowIcon(DBusCommand::staticCategoryIcon());
  setWindowTitle(DBusCommand::staticCategoryText());

  connect(ui.leServiceName, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.lePath, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leMethod, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
}


bool CreateDBusCommandWidget::isComplete()
{
  return ((!ui.leServiceName->text().isEmpty()) && (!ui.lePath->text().isEmpty()) 
      && (!ui.leMethod->text().isEmpty()));
}


bool CreateDBusCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  DBusCommand *dbusCommand = dynamic_cast<DBusCommand*>(command);
  if (!dbusCommand) return false;

  ui.leServiceName->setText(dbusCommand->serviceName());
  ui.lePath->setText(dbusCommand->path());
  ui.leInterface->setText(dbusCommand->interface());
  ui.leMethod->setText(dbusCommand->method());
  ui.elbArguments->setItems(dbusCommand->arguments());

  return true;
}


Command* CreateDBusCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new DBusCommand(name, iconSrc, description, ui.leServiceName->text(), ui.lePath->text(),
      ui.leInterface->text(), ui.leMethod->text(), ui.elbArguments->items());
}


CreateDBusCommandWidget::~CreateDBusCommandWidget()
{
}
