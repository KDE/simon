//
// C++ Implementation: systemwidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "systemwidget.h"

/**
 * \brief Constructor - passes the arguments to the inlinewidget
 * \author Peter Grasch
 * @param title The title of the control
 * @param icon The icon of the control
 * @param desc A small description
 * @param parent The parent of the widget
 */
SystemWidget::SystemWidget(QString title, QIcon icon, QString desc, QWidget *parent)
 : InlineWidget(title, icon, desc, parent)
{
	
}

SystemWidget::~SystemWidget()
{
}


