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
#include "progresswidget.h"

#include "operation.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QProgressBar>
#include <KPushButton>
#include <KLocalizedString>
#include <KLocale>

ProgressWidget::ProgressWidget(QPointer<Operation> O, ProgressWidgetStyle style, QWidget* parent) : QWidget(parent),
op(O)
{
  if (op) {
    name = new QLabel(op->name(), this);
    QFont font(name->font());
    font.setBold(true);
    name->setFont(font);
    currentAction = new QLabel(op->currentAction(), this);

    bar = new QProgressBar(this);
    bar->setValue(op->currentProgress());
    bar->setMaximum(op->maxProgress());

    cancelButton = new KPushButton(KIcon("process-stop"), i18n("Cancel"), this);
    if (op->isAtomic()) cancelButton->setEnabled(false);

    QHBoxLayout *hBox = new QHBoxLayout();
    hBox->addWidget(currentAction);
    if (style == Compact) {
      hBox->addWidget(bar);
      hBox->addWidget(cancelButton);
    }

    QVBoxLayout *vBox = new QVBoxLayout(this);
    vBox->addWidget(name);
    vBox->addLayout(hBox);
    if (style != Compact) {
      vBox->addWidget(bar);

      QHBoxLayout *hBox2 = new QHBoxLayout();
      hBox2->addStretch(2);
      hBox2->addWidget(cancelButton);
      vBox->addLayout(hBox2);
    }
    vBox->addWidget(new QSplitter(this));
    connect(cancelButton, SIGNAL(clicked()), op, SLOT(cancel()));
  }
}


void ProgressWidget::update()
{
  currentAction->setText(op->currentAction());
  bar->setValue(op->currentProgress());

  if (bar->maximum() != op->maxProgress())
    bar->setMaximum(op->maxProgress());

  if (!op->isRunning())
    cancelButton->setDisabled(true);
}


ProgressWidget::~ProgressWidget()
{
}
