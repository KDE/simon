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

#include "systemwidgetpage.h"

/**
 \class FirstRunModelSettings
 \brief ModelSettings for the FirstRunWizard - custom nextId()
 \author Peter Grasch
 \date 6.1.2008
*/
class FirstRunModelSettings : public SystemWidgetPage
{
public:
    FirstRunModelSettings(QWidget* parent);

	int nextId() const;

};

#endif
