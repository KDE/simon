/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#include "createvrpncommandwidget.h"
#include "vrpncommand.h"
#include <KLineEdit>

CreateVRPNCommandWidget::CreateVRPNCommandWidget(CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);

  setWindowIcon(VRPNCommand::staticCategoryIcon());
  setWindowTitle(VRPNCommand::staticCategoryText());
}


bool CreateVRPNCommandWidget::isComplete()
{
  return true;
}


bool CreateVRPNCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  VRPNCommand *vrpnCommand = dynamic_cast<VRPNCommand*>(command);
  if (!vrpnCommand) return false;

  return true;
}


Command* CreateVRPNCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new VRPNCommand(name, iconSrc, description);
}


CreateVRPNCommandWidget::~CreateVRPNCommandWidget()
{
}
