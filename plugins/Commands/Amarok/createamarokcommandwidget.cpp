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

#include "createamarokcommandwidget.h"
#include "amarokcommand.h"
#include <KLineEdit>

CreateAmarokCommandWidget::CreateAmarokCommandWidget(CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);

  setWindowIcon(AmarokCommand::staticCategoryIcon());
  setWindowTitle(AmarokCommand::staticCategoryText());
}


bool CreateAmarokCommandWidget::isComplete()
{
  return !ui.leTrackId->text().isEmpty();
}


bool CreateAmarokCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  AmarokCommand *amarokCommand = dynamic_cast<AmarokCommand*>(command);
  if (!amarokCommand) return false;

  ui.leTrackId->setText(amarokCommand->getPath());
  return true;
}


Command* CreateAmarokCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new AmarokCommand(name, iconSrc, description, ui.leTrackId->text());
}


CreateAmarokCommandWidget::~CreateAmarokCommandWidget()
{
}
