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

#include "commandpreviewwidget.h"
#include <simonscenarios/command.h>
#include <simonscenarios/simoncommand.h>
#include <QModelIndex>
#include <QMap>
#include <QLayout>

#include <QIcon>
#include <QUrl>

CommandPreviewWidget::CommandPreviewWidget(QWidget* parent) : QWidget(parent),
command(0)
{
  ui.setupUi(this);

  connect(ui.pbTrigger, SIGNAL(clicked()), this, SLOT(trigger()));
  hide();

  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
}


void CommandPreviewWidget::trigger()
{
  int state = SimonCommand::DefaultState;
  if (command) command->trigger(&state);
}


void CommandPreviewWidget::updateCommand(const QModelIndex &commandIdx)
{
  Command *command = static_cast<Command*>(commandIdx.internalPointer());
  if (!command) {
    hide();
    return;
  } else show();

  ui.lbIcon->setPixmap(command->getIcon().pixmap(64,64));
  ui.lbName->setText(command->getTrigger());

  QLayoutItem *child;
  while (ui.flDetails->count() > 0) {
    child = ui.flDetails->takeAt(0);
    ui.flDetails->removeItem(child);

    QWidget *widget = child->widget();
    if (widget) widget->deleteLater();
    delete child;
  }

  QMap<QString,QVariant> details = command->getValueMap();
  QStringList keys = details.keys();

  for (int i=0; i < keys.count(); i++) {
    QLabel *label = new QLabel(this);
    label->setOpenExternalLinks(true);
    label->setWordWrap(true);
    QVariant value = details.value(keys[i]);

    QString strValue;
    if (value.type() == QVariant::Url)
      strValue = QString("<a href=\"%1\">%1</a>").arg(value.toUrl().toString());
    else strValue = value.toString();

    label->setText(strValue);

    ui.flDetails->addRow(keys[i]+':', label);
  }

  //resize(sizeHint().height(), width());
  this->command = command;
}
