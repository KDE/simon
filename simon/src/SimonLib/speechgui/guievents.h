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


#ifndef GUIEVENTS_H
#define GUIEVENTS_H

#include "speechgui_export.h"
#include <QObject>
#include <QHash>


struct actionValues
	{
		QObject* receiver;
		const char* slot;
	};
struct uniqueKey
	{
		QObject* parent;
		QString trigger;
	};
typedef QHash<uniqueKey*,  actionValues*> GuiAction;


class SPEECHGUI_EXPORT GuiEvents :  public  QObject {
	Q_OBJECT
	
private:
	
	GuiAction *guiItems;
	

public:
	
	GuiEvents(QObject *parent=0);
	~GuiEvents();

public slots:
	
	void registerControl(QString trigger, QObject* receiver, const char* slot);


	void doAction(QString action, QObject * parentItem);

signals:
	void dummy(QString);
	
};

#endif
