/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "simoninfo.h"
#include <QCoreApplication>
#include <QPixmap>
#include <QSplashScreen>
#include "osd.h"

/**
 * @brief Constructor
 *
 * Constructor of the Info-Class
 *
 * @author Peter Grasch
 * 
*/

SimonInfo::SimonInfo(QWidget *parent)
{
	this->parent = parent;
}


/**
 * @brief Shows a splash screen
 *
 * Shows the simon splash screen to increase the feel of responsivness
 * The splash will be shown until it is destroyed
 *	@author Peter Grasch
 *
 * @see hideSplash(); writeToSplash();
 * 
*/
void SimonInfo::showSplash()
{
	QPixmap splashbg(":/images/splash.png");

	this->splash = new QSplashScreen( splashbg );
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
	this->splash->showMessage(status, Qt::AlignLeft|Qt::AlignBottom, 
					QColor(175, 190, 200, 255));
	QCoreApplication::processEvents();
}



/**
 * @brief Shows a short message on the screen
 *
 * Shows a short message. Replacement to QMessageBox::information(...)
 * Doesn't interfer with the users work as the message is passive
 * Message disappears after n-Seconds
 *
 *	@author Peter Grasch
 * 
 */
void SimonInfo::showMessage(QString message, short time)
{
	//show an "OSD"-like Message
	new OSD(message, time);
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
	this->splash->finish(this->parent);
	splash->deleteLater();
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
*/
SimonInfo::~SimonInfo()
{
}


