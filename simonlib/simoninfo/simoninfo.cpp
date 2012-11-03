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

#include "simoninfo.h"
#include <QCoreApplication>
#include <QPixmap>
#include <KSplashScreen>
#include <KStandardDirs>
#include "osd.h"

/**
 * @brief Constructor
 *
 * Constructor of the Info-Class
 *
 * @author Peter Grasch
 *
 */

SimonInfo::SimonInfo(QWidget* parent_)
: splash(0),
parent(parent_)
{
}


/**
 * @brief Shows a splash screen
 *
 * Shows the simon splash screen to increase the feel of responsiveness
 * The splash will be shown until it is destroyed
 *	@author Peter Grasch
 *
 * @see hideSplash(); writeToSplash();
 *
 */
void SimonInfo::showSplash()
{
  if (this->splash) this->splash->deleteLater();

  this->splash = new KSplashScreen( QPixmap(KStandardDirs::locate("appdata", "themes/default/splash.png")) );
  this->splash->show();

}


/**
 * @brief Updates the Splash message
 *
 * This message adds a short message to the splash screen to let the user
 * know what is going on at the moment.
 * This also greatly enhances the feel of responsiveness
 *
 *	@author Peter Grasch
 * @param QString status
 * The status that is written to the Splash Screen
 * @see hideSplash(); showsplash();
 *
 */

void SimonInfo::writeToSplash(QString status)
{
  if (!this->splash) return;

  this->splash->showMessage(status, Qt::AlignLeft|Qt::AlignBottom,
    QColor(175, 190, 200, 255));
  QCoreApplication::processEvents();
}


/**
 * @brief Shows a short message on the screen
 *
 * Shows a short message. Replacement to QMessageBox::information(...)
 * Doesn't interfere with the users work as the message is passive
 * Message disappears after n-Seconds
 *
 *	@author Peter Grasch
 *
 */
void SimonInfo::showMessage(QString message, short time, KIcon *icon)
{
  //show an "OSD"-like Message
  new OSD(message, time, icon);
}


/**
 * @brief Hides the splash screen
 *
 * Hides the simon splash screen. This is usually done after the loading
 * of the main application is complete and before the main window is shown.
 *
 *	@author Peter Grasch
 * @see showSplash(); writeToSplash();
 *
 */
void SimonInfo::hideSplash()
{
  if (!splash) return;

  this->splash->finish(this->parent);
  splash->deleteLater();
  splash=0;
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
SimonInfo::~SimonInfo()
{
  if (this->splash) this->splash->deleteLater();
}
