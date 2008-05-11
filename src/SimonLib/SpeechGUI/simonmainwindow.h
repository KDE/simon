//
// C++ Interface: SimonMainWindow
//
// Description: 
//
//
// Author: Phillip Goriup (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SIMONMAINWINDOW_H
#define SIMONMAINWINDOW_H

#include <QObject>
#include <QMainWindow>

#include "simonwidget.h"


/**
 * \class SimonMainWindow
 * \brief 
 * \note 
 * \author Phillip Goriup
 * \version 
 * \date 
*/
class SimonMainWindow : public QMainWindow,  public  SimonWidget
{
Q_OBJECT

private:
	GuiEvents *guievents;

signals:


public:
	SimonMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);	
	~SimonMainWindow();
	QObjectList getChildren(QObject *current);
	


public slots:
	
	void registerControl(QString trigger, QObject* receiver, const char* slot);
	void doAction(QString action);
	

};

#endif
