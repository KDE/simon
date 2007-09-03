//
// C++ Interface: windowsprogrammanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WINDOWSPROGRAMMANAGER_H
#define WINDOWSPROGRAMMANAGER_H

#include <programmanager.h>
#include "program.h"
#include "programcategory.h"
#include "registrymanager.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class WindowsProgramManager : public ProgramManager
{
public:
    WindowsProgramManager();

    ~WindowsProgramManager();

    ProgramList* getPrograms(ProgramCategory categorie);

private:
    RegistryManager *regMan;

};

#endif
