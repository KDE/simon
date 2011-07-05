/*   Copyright (C) 2011 Grasch Peter <grasch@simon-listens.org>
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

#include "simonstatuswidget.h"
#include "simonstatuswidgetadaptor.h"
#include <KIcon>
#include <KStandardDirs>

SimonStatusWidget::SimonStatusWidget(QWidget* parent, Qt::WindowFlags f) : 
  QLabel(parent, f|Qt::FramelessWindowHint), singleShotActive(false)
{
  new SimonStatusWidgetAdaptor(this);
  QDBusConnection dbus = QDBusConnection::sessionBus();
  dbus.registerObject("/SimonStatusWidget", this);
  dbus.registerService("org.simon-listens.SimonStatusWidget");
  
  setStyleSheet("background:transparent;");
  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_X11DoNotAcceptFocus);
  setAttribute(Qt::WA_TransparentForMouseEvents);
  setAttribute(Qt::WA_X11NetWmWindowTypeDock);
  
  setInactive();
}

void SimonStatusWidget::setActive()
{
  setPixmap(QPixmap(KStandardDirs::locate("appdata", "symbols/bubble_on.png")));
}

void SimonStatusWidget::setInactive()
{
  setPixmap(QPixmap(KStandardDirs::locate("appdata", "symbols/bubble_off.png")));
}

void SimonStatusWidget::setActiveOnce()
{
  singleShotActive = true;
  setActive();
}

void SimonStatusWidget::recognizedSomething()
{
  if (singleShotActive) {
    singleShotActive = false;
    setInactive();
  }
}


