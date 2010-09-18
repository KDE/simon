/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "createjsoncommandwidget.h"
#include "jsoncommand.h"
#include <KLineEdit>

CreateJsonCommandWidget::CreateJsonCommandWidget(CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);

  setWindowIcon(JsonCommand::staticCategoryIcon());
  setWindowTitle(JsonCommand::staticCategoryText());

  connect(ui.leURL, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));
  connect(ui.leRequest, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));
}


bool CreateJsonCommandWidget::isComplete()
{
  return ((!ui.leURL->text().isEmpty()) && (!ui.leRequest->text().isEmpty()));
}


bool CreateJsonCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  JsonCommand *jsonCommand = dynamic_cast<JsonCommand*>(command);
  if (!jsonCommand) return false;

  ui.leURL->setText(jsonCommand->url());
  ui.leRequest->setText(jsonCommand->request());

  return true;
}


Command* CreateJsonCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new JsonCommand(name, iconSrc, description, ui.leURL->text(), ui.leRequest->text());
}


CreateJsonCommandWidget::~CreateJsonCommandWidget()
{
}
