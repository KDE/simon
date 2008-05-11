//
// C++ Interface: word
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SIMONDIRSELECTOR_H
#define SIMONDIRSELECTOR_H

#include "simonfilesystemselector.h"

/**
 *	@class SimonDirSelector
 *	@brief LineEdit coupled with a selector button to select directories
 *
 *	@version 0.1
 *	@date 08.05.2007
 *	@author Peter Grasch
 */
class SimonDirSelector : public SimonFileSystemSelector {



public:
	SimonDirSelector(QWidget *parent=0);
	~SimonDirSelector() {}

};

#endif
