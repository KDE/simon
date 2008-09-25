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


#ifndef SIMONWIDGET_H
#define SIMONWIDGET_H

#include "guievents.h"
#include <QVector>
/**
 *	@class SimonWidget
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Phillip Goriup
 */
class QLabel;

class SimonWidget{
	
private:
	
	QVector <QString> specialcharacter;

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
