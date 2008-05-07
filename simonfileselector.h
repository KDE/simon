#ifndef SIMONFILESELECTOR_H
#define SIMONFILESELECTOR_H

#include "simonfilesystemselector.h"

/**
 *	@class SimonFileSelector
 *	@brief LineEdit coupled with a selector button to select files
 *
 *	@version 0.1
 *	@date 08.05.2007
 *	@author Peter Grasch
 */
class SimonFileSelector : public SimonFileSystemSelector {


public:
	SimonFileSelector(QWidget *parent=0);
	~SimonFileSelector() {}

};


#endif
