/*
 *   Copyright (C) 2008 Phillip Goriup <goriup@simon-listens.org>
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

#include "simonmainwindow.h"

SimonMainWindow::SimonMainWindow(QWidget *parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags)
{
	guievents = new GuiEvents;
}


SimonMainWindow::~SimonMainWindow()
{
	
}


void SimonMainWindow::registerControl(QString trigger, QObject* receiver, const char* slot)
{
	guievents->registerControl(trigger, receiver, slot);
}


void SimonMainWindow::doAction(QString action)
{
	guievents->doAction(action,(QObject*)this);
}


QObjectList SimonMainWindow::getChildren(QObject *current)
{
	return current->children();
}
