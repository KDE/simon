//
// C++ Interface: firstrunmodelsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FIRSTRUNMODELSETTINGS_H
#define FIRSTRUNMODELSETTINGS_H

#include <systemwidgetpage.h>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class FirstRunModelSettings : public SystemWidgetPage
{
public:
    FirstRunModelSettings(QWidget* parent);

	int nextId() const;
    ~FirstRunModelSettings();

};

#endif
