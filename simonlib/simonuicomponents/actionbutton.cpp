/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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


#include "actionbutton.h"
#include <QAction>
#include <KDebug>

ActionButton::ActionButton ( QWidget* parent ) : KPushButton ( parent ), m_action(0)
{
}

void ActionButton::setAction ( QAction* action )
{
  Q_ASSERT(action);
  
  if (m_action) {
    disconnect(m_action, SIGNAL(changed()), this, SLOT(updateUi()));
    disconnect(this, SIGNAL(toggled(bool)), m_action, SLOT(toggle()));
  }
  
  m_action = action;
  connect(m_action, SIGNAL(changed()), this, SLOT(updateUi()));
  connect(this, SIGNAL(clicked(bool)), m_action, SLOT(trigger()));
  updateUi();
}

void ActionButton::updateUi()
{
  setText(m_action->text());
  setIcon((KIcon) m_action->icon());
  setEnabled(m_action->isEnabled());
  setCheckable(m_action->isCheckable());
  setChecked(m_action->isChecked());
}
