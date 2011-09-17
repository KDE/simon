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
#include <KCmdLineArgs>

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
  
  connect(&timeoutTimer, SIGNAL(timeout()), this, SLOT(setInactive()));
  
  setInactive();
}

void SimonStatusWidget::disableSingleShot()
{
  singleShotActive = false;
  timeoutTimer.stop();
}


void SimonStatusWidget::setActive()
{
  disableSingleShot();
  showActive();
}

void SimonStatusWidget::setInactive()
{
  disableSingleShot();
  showInactive();
}

QString SimonStatusWidget::getPrefix()
{
  if (KCmdLineArgs::parsedArgs()->isSet("icons"))
    return KCmdLineArgs::parsedArgs()->getOption("icons");
  return "bubble";
}

void SimonStatusWidget::showActive()
{
  setPixmap(QPixmap(KStandardDirs::locate("appdata", "symbols/"+getPrefix()+"_on.png")));
}

void SimonStatusWidget::showInactive()
{
  setPixmap(QPixmap(KStandardDirs::locate("appdata", "symbols/"+getPrefix()+"_off.png")));
}

void SimonStatusWidget::setActiveOnce(int timeout)
{
  singleShotActive = true;
  showActive();
  
  timeoutTimer.stop();
  if (timeout != -1) {
    timeoutTimer.setInterval(timeout);
    timeoutTimer.start();
  }
}

void SimonStatusWidget::recognizedSomething()
{
  if (singleShotActive) {
    disableSingleShot();
    setInactive();
  }
}


