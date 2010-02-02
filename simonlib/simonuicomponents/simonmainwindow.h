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
#ifndef SIMON_SIMONMAINWINDOW_H_BA327087B7A84ED3B931E302C697A6D1
#define SIMON_SIMONMAINWINDOW_H_BA327087B7A84ED3B931E302C697A6D1

#include <QObject>
#include <kxmlguiwindow.h>

#include "simonwidget.h"
#include "speechgui_export.h"


/**
 * \class SimonMainWindow
 * \brief 
 * \note 
 * \author Phillip Goriup
 * \version 
 * \date 
*/
class SPEECHGUI_EXPORT SimonMainWindow : public KXmlGuiWindow,  public  SimonWidget
{
Q_OBJECT

private:
	GuiEvents *guievents;
	virtual void setupActions()=0;

signals:


public:
	explicit SimonMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);	
	~SimonMainWindow();
	QObjectList getChildren(QObject *current);
	


public slots:
	
	void registerControl(QString trigger, QObject* receiver, const char* slot);
	void doAction(QString action);
	

};

#endif
