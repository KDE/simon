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


#include "visualdialogview.h"
#include "ui_dialogwidget.h"

#include <simonactions/actionmanager.h>

#include <KIcon>

VisualDialogView::VisualDialogView(DialogCommandManager *dialog, QWidget *parent, Qt::WindowFlags flags) : 
  QWidget(parent,flags),
  DialogView(dialog),
  ui(new Ui::DialogView)
{
  setFont(ActionManager::getInstance()->pluginBaseFont());

  setWindowIcon(KIcon("im-user"));
  ui->setupUi(this);
  hide();
} 

bool VisualDialogView::start()
{
  show();
  return true;
}

bool VisualDialogView::stop()
{
  hide();
  return true;
}


bool VisualDialogView::present(const DialogState& state)
{
  Q_UNUSED(state);

  kDebug() << "Presenting state...";
  return true;
}


void VisualDialogView::setFont(const QFont& font)
{
  QWidget::setFont(font);
}

