/***************************************************************************
*   Copyright (C) 2006 by Peter Grasch   *
*   bedahr@gmx.net   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SIMONVIEW_H
#define SIMONVIEW_H

/**
 *	@class SimonInfo
 *	@brief Helper class providing convenient methods on how to display short infos on the screen
 *
 *	Provides methods to display infos on the screen. Some of these methods are public and static to be used
 *	from every context.
 *	This Info-Viewer includes OSD like messages but also splashscreens,etc.
 *
 *	@version 0.1
 *	@date 10.01.2006
 *	@author Peter Grasch
 *	@todo Implementing functions
*/

#include <QMainWindow>
#include <QPlastiqueStyle>
#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "osd.h"


class SimonInfo {
	
private:
	QSplashScreen *splash;  //!< Splashscreen-Pointer
	QWidget *parent;  //!< provides a pointer to the parenting object of the Splashscreen


public:
	SimonInfo(QWidget *parent = 0);
	
	void showSplash();
	void writeToSplash(QString status);
	void hideSplash();
	static void showMessage(QString message, short time);
	~SimonInfo();

};

#endif
