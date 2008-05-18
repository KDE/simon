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
#ifndef GUIEVENTS_H
#define GUIEVENTS_H

#include <QObject>
#include <QHash>
#include <QMessageBox>


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


class GuiEvents :  public  QObject {
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
