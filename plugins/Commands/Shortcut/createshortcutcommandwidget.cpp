/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
 *   Copyright (C) 2010 Manfred Scheucher <deadalps@gmail.com>
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

#include "createshortcutcommandwidget.h"
#include "shortcutcommand.h"

CreateShortcutCommandWidget::CreateShortcutCommandWidget(CommandManager *manager, QWidget *parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);

  #if KDE_IS_VERSION(4,1,60)
  ui.ksShortcut->setCheckForConflictsAgainst(KKeySequenceWidget::None);
  #endif
  ui.ksShortcut->setModifierlessAllowed(true);
  setWindowIcon(ShortcutCommand::staticCategoryIcon());
  setWindowTitle(ShortcutCommand::staticCategoryText());

  connect(ui.ksShortcut, SIGNAL(keySequenceChanged(QKeySequence)), this, SIGNAL(completeChanged()));
  #ifndef Q_OS_WIN
  ui.cbSpecialShortcut->hide();
  ui.lbSpecialShortcut->hide();
  ui.pbApplySpecialShortcut->hide();
  #else
  ui.pbApplySpecialShortcut->setIcon(KIcon("arrow-up"));
  connect(ui.pbApplySpecialShortcut, SIGNAL(clicked()), this, SLOT(applySpecialShortcut()));
  #endif
}


#ifdef Q_OS_WIN
void CreateShortcutCommandWidget::applySpecialShortcut()
{
  ui.ksShortcut->setKeySequence(QKeySequence(ui.cbSpecialShortcut->currentText()));
}
#endif

bool CreateShortcutCommandWidget::isComplete()
{
  return !(ui.ksShortcut->keySequence().isEmpty());
}


bool CreateShortcutCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  ShortcutCommand *shortcutCommand = dynamic_cast<ShortcutCommand*>(command);
  if (!shortcutCommand) return false;

  ui.ksShortcut->setKeySequence(shortcutCommand->getShortcut());
  ui.cbMode->setCurrentIndex(getShortcutModeIndex(shortcutCommand->getMode()));
  return true;
}


Command* CreateShortcutCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new ShortcutCommand(name, iconSrc, description, ui.ksShortcut->keySequence(), getSelectedShortcutMode());
}


CreateShortcutCommandWidget::~CreateShortcutCommandWidget()
{
}

EventSimulation::PressMode CreateShortcutCommandWidget::getSelectedShortcutMode()
{
  switch(ui.cbMode->currentIndex())
  {
    case 1: 
      return EventSimulation::Press;
    case 2: 
      return EventSimulation::Release;
    default: //case 0: 
      return (EventSimulation::PressMode) (EventSimulation::Press | EventSimulation::Release);
  }
}

int CreateShortcutCommandWidget::getShortcutModeIndex(EventSimulation::PressMode mode)
{
  switch(mode)
  {
    case EventSimulation::Press: 
      return 1;
    case EventSimulation::Release: 
      return 2;
    default: //both
      return 0;
  }  
}

