/* 
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "speechcalview.h"
#include "calendarmodel.h"
#include "speechcal.h"
#include <simonlogging/logger.h>

#include <KStandardAction>
#include <KActionCollection>
#include <KAction>
#include <KMenuBar>
#include <KStatusBar>


SpeechCalView::SpeechCalView(CalendarModel* model, SpeechCal* c, QWidget* parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags), 
	controller(c)
{
  ui.setupUi(this);
  
  //add quit action with escape and ctrl+q as shortcuts
  KStandardAction::quit(this, SLOT(close()), actionCollection())->setShortcut(
	      KShortcut(QKeySequence(Qt::Key_Escape), QKeySequence("Backspace")));
  setupGUI();
  menuBar()->hide();
  statusBar()->hide();
  ui.lvCalendar->setModel(model);
  
  ui.pbPreviousDay->setIcon(KIcon("go-previous"));
  ui.pbNextDay->setIcon(KIcon("go-next"));
  
  
  KAction* previousDayAction = new KAction(this);
  previousDayAction->setText(i18n("Previous day"));
  previousDayAction->setIcon(KIcon("go-previous"));
  previousDayAction->setShortcut(Qt::Key_Down);
  actionCollection()->addAction("previousDay", previousDayAction);
  connect(previousDayAction, SIGNAL(triggered(bool)),
    this, SLOT(previousDay()));
  
  KAction* nextDayAction = new KAction(this);
  nextDayAction->setText(i18n("Previous day"));
  nextDayAction->setIcon(KIcon("go-next"));
  nextDayAction->setShortcut(Qt::Key_Up);
  actionCollection()->addAction("nextDay", nextDayAction);
  connect(nextDayAction, SIGNAL(triggered(bool)),
    this, SLOT(nextDay()));
  
  connect(ui.pbNextDay, SIGNAL(clicked()), this, SLOT(nextDay()));
  connect(ui.pbPreviousDay, SIGNAL(clicked()), this, SLOT(previousDay()));
  connect(ui.pbClose, SIGNAL(clicked()), qApp, SLOT(quit()));
}

void SpeechCalView::updateDisplay(const QString& name)
{
  ui.lbDate->setText(name);
}

void SpeechCalView::nextDay()
{
  controller->nextDay();
}

void SpeechCalView::previousDay()
{
  controller->previousDay();
}


SpeechCalView::~SpeechCalView()
{
  controller->quit();
}
