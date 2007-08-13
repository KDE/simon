//
// C++ Interface: systemwidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SYSTEMWIDGET_H
#define SYSTEMWIDGET_H

#include "inlinewidget.h"

class QString;

/**
 \class SystemWidget
 \brief Extends InlineWidget with methods like apply/reset/... for use in the system menue
 \author Peter Grasch
 \date 12.08.2007
 \version 0.1
*/
class SystemWidget : public InlineWidget
{
	Q_OBJECT
signals:
	void busy();
	void idle();

protected:
	QString help;

public slots:
	virtual bool apply()=0;
	virtual bool reset()=0;
	virtual bool init()=0;
	
public:
    SystemWidget(QString title, QIcon icon, QString desc, QWidget *parent);


	QString getHelp() { return help; };

    ~SystemWidget();

};

#endif
