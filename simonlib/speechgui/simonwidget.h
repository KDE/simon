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


#ifndef SIMON_SIMONWIDGET_H_4A0AED03F9144881B12FCD79BC38EDEF
#define SIMON_SIMONWIDGET_H_4A0AED03F9144881B12FCD79BC38EDEF

#include "guievents.h"
#include "speechgui_export.h"
/**
 *	@class SimonWidget
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Phillip Goriup
 */
class QLabel;

class SPEECHGUI_EXPORT SimonWidget{
	
private:

	virtual QObjectList getChildren(QObject *current)
	{
		QObjectList temp;
		current->deleteLater();
		return temp;
	}
	protected: 
	GuiEvents *guievents;

public:
	virtual void doAction(QString action) = 0;
	virtual void registerControl(QString trigger, QObject* receiver, const char* slot) = 0;

	void guessChildTriggers(QObject *current);
	void registerBuddy(QLabel* label);
	QString editChildKeys(QString);
	virtual ~SimonWidget() {}
};

#endif
