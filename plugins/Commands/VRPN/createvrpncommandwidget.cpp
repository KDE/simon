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
#include <QStringList>
#include <KLineEdit>

CreateVRPNCommandWidget::CreateVRPNCommandWidget(const QStringList& buttons, CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);

  setWindowIcon(VRPNCommand::staticCategoryIcon());
  setWindowTitle(VRPNCommand::staticCategoryText());

  ui.cbButton->clear();
  foreach (const QString& button, buttons)
    ui.cbButton->addItem(button);

  connect(ui.cbButton, SIGNAL(currentIndexChanged(int)), this, SIGNAL(completeChanged()));
}

bool CreateVRPNCommandWidget::isComplete()
{
  return !ui.cbButton->currentText().isEmpty();
}

bool CreateVRPNCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  VRPNCommand *vrpnCommand = dynamic_cast<VRPNCommand*>(command);
  if (!vrpnCommand) return false;

  int index = ui.cbButton->findText(vrpnCommand->getButton());
  // if not found, this behaves correctly as it well be set to index -1 (i.e., nothing)
  ui.cbButton->setCurrentIndex(index);
  return true;
}

Command* CreateVRPNCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new VRPNCommand(name, iconSrc, description, ui.cbButton->currentText());
}

CreateVRPNCommandWidget::~CreateVRPNCommandWidget()
{
}
