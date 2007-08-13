//
// C++ Interface: logview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <systemwidget.h>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class LogView : public SystemWidget
{
public:
    LogView(QWidget* parent);

    ~LogView();

};

#endif
