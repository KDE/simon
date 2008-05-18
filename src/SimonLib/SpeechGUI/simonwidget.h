//
// C++ Interface: coreevents
//
// Description: 
//
//
// Author: Phillip Goriup (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SIMONWIDGET_H
#define SIMONWIDGET_H

#include "guievents.h"
/**
 *	@class SimonWidget
 *	@brief Abstract class defining the methods that a EventBackend has to support
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Phillip Goriup
 */
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
