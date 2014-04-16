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


#include "visualdialogview.h"
#include "ui_dialogwidget.h"

#include <simondialogengine/dialogturn.h>
#include <simondialogengine/dialogcommand.h>
#include <simondialogengine/avatar.h>
#include <simondialogengine/dialogmanager.h>
#include "dialogcommandbutton.h"

#include <simonactions/actionmanager.h>

#include <QVBoxLayout>
#include <QList>

#include <KIcon>

VisualDialogView::VisualDialogView(DialogManager *dialog, QWidget *parent, Qt::WindowFlags flags) : 
  QWidget(parent,flags),
  DialogView(dialog),
  ui(new Ui::DialogView)
{
  setFont(ActionManager::getInstance()->pluginBaseFont());

  setWindowIcon(KIcon("im-user"));

  ui->setupUi(this);
  hide();

  ui->wgAnswers->setLayout(new QVBoxLayout());

  ui->frmWrongInput->hide();
} 

void VisualDialogView::transitionDestroyed()
{
  m_buttons.removeAll(static_cast<DialogCommandButton*>(sender()));
}

void VisualDialogView::closeEvent(QCloseEvent*)
{
  requestClose();
}

void VisualDialogView::requestClose()
{
  m_dialog->initState(0);
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


bool VisualDialogView::present(const DialogTurn& turn)
{
  while (!m_buttons.isEmpty())
    delete m_buttons.takeFirst();
  m_buttons.clear();

  Avatar* a = m_dialog->getAvatar(turn.getAvatarId());
  if (a && turn.getDisplayAvatar())
  {
    ui->wgAvatar->show();
    QString avatarName = a->name();
    if (!m_dialog->getDisplayAvatarNames() || avatarName.isEmpty()) {
      ui->wgAvatarName->hide();
    } else {
      ui->lbName->setText(avatarName);
      ui->wgAvatarName->show();
    }
    int size = m_dialog->getAvatarSize();
    ui->lbAvatar->setPixmap(QPixmap::fromImage(a->image().scaled(size,size, Qt::KeepAspectRatio)));
  } else
    ui->wgAvatar->hide();

  ui->lbText->setText(turn.getText());

  QList<DialogCommand*> transitions = turn.getTransitions();

  kDebug() << "Presenting " << transitions.count() << " transitions";

  foreach (DialogCommand* transition, transitions)
  {
    DialogCommandButton *button = new DialogCommandButton(transition);
    static_cast<QVBoxLayout*>(ui->wgAnswers->layout())->addWidget(button);
    connect(button, SIGNAL(destroyed()), this, SLOT(transitionDestroyed()));
    m_buttons << button;
  }

  return true;
}

void VisualDialogView::correctInputReceived()
{
  ui->frmWrongInput->hide();
}

void VisualDialogView::warnOfInvalidInput(const QString& input)
{
  ui->lbInvalidInput->setText(i18nc("%1 is the invalid input provided", "Invalid input: %1\nPlease say one of the displayed options.", input));
  ui->frmWrongInput->show();
}


void VisualDialogView::setFont(const QFont& font)
{
  QWidget::setFont(font);
}

