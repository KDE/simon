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

#include "dialogcommandbutton.h"
#include <simondialogengine/dialogcommand.h>

DialogCommandButton::DialogCommandButton(DialogCommand* transition, QWidget *parent) :
  KPushButton(parent),
  m_transition(transition)
{
  setText(transition->text());
  setObjectName(QString("dialogOption%1").arg(transition->getTrigger()));

  if (transition->showIcon())
    setIcon(transition->getIcon());

  //setDescription(transition->getDescription());
  setToolTip(transition->getDescription());

  connect(transition, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
  connect(this, SIGNAL(clicked()), this, SLOT(go()));
  connect(transition, SIGNAL(destroyed()), this, SLOT(deleteLater()));
}

void DialogCommandButton::go()
{
  m_transition->trigger(0);
}

