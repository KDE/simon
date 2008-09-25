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

#include "atobject.h"
#include "ato.h"
#include "../EventSimulation/eventhandler.h"
#include <kmessagebox.h>

ATObject::ATObject ( ATObject* parent, const QString& nam, const QString& cName, const QString& desc ) :
			QWidget(parent),className(cName), name(nam)
{
	this->description=desc;
}

void ATObject::addMenu(ATOMenu* menu)
{
// 	KMessageBox::critical(0, "sodfij", menu->title);
// 	for (int i=0; i < menu->actions.count(); i++)
// 		KMessageBox::information(0, "sodfij", menu->actions[i]->title);
	this->menuList.append(menu);
}

void ATObject::trigger()
{
	int xC = x()+(width()/2);
	int yC = y()+(height()/2);
	EventHandler::getInstance()->click(xC, yC);
}


ATObject::~ATObject()
{
}


