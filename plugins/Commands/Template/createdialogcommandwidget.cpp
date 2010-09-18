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
#include <KLineEdit>

CreateDialogCommandWidget::CreateDialogCommandWidget(CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);

  setWindowIcon(DialogCommand::staticCategoryIcon());
  setWindowTitle(DialogCommand::staticCategoryText());
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

  return true;
}


Command* CreateDialogCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new DialogCommand(name, iconSrc, description);
}


CreateDialogCommandWidget::~CreateDialogCommandWidget()
{
}
