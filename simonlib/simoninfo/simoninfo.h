/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SIMONINFO_H_D2600C6D435E4D26A75AC74A42A31C60
#define SIMON_SIMONINFO_H_D2600C6D435E4D26A75AC74A42A31C60

#include "simoninfo_export.h"
#include <QString>

/**
 *	@class SimonInfo
 *	@brief Helper class providing convenient methods on how to display short information on the screen
 *
 *	Provides methods to display information on the screen. Some of these methods are public and static to be used
 *	from every context.
 *	This Info-Viewer includes OSD like messages but also splashscreens,etc.
 *
 *	@version 0.1
 *	@date 10.01.2006
 *	@author Peter Grasch
 */
class QWidget;
class KIcon;

class KSplashScreen;

class SIMONINFO_EXPORT SimonInfo
{

  private:
    KSplashScreen *splash;                        //!< Splashscreen-Pointer
    QWidget *parent;

  public:
    SimonInfo(QWidget *parent = 0);

    void showSplash();
    void writeToSplash(QString status);
    void hideSplash();
    static void showMessage(QString message, short time, KIcon *icon=0);
    ~SimonInfo();

};
#endif
