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

#include "createtransitiondialog.h"
#include "dialogcommand.h"
#include "createdialogcommandwidget.h"
#include <QWidget>
#include <QVBoxLayout>

CreateTransitionDialog::CreateTransitionDialog(CreateDialogCommandWidget *creator, QWidget *parent) : 
  KDialog(parent),
  m_creator(creator)
{
  QWidget *mainWidget = new QWidget(this);
  ui.setupUi(mainWidget);
  setMainWidget(mainWidget);

  static_cast<QVBoxLayout*>(mainWidget->layout())->insertWidget(1, creator);
  connect(ui.leTrigger, SIGNAL(textChanged(const QString&)), creator, SLOT(updatePresentation(const QString&)));
}


DialogCommand* CreateTransitionDialog::createTransition()
{
  if (!exec()) return 0;

  DialogCommand *command = static_cast<DialogCommand*>(m_creator->createCommand(ui.leTrigger->text(), 
                                              ui.ibIcon->icon(), ui.teDescription->toPlainText()));
  return command;
}

