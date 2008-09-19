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

#include "programmanager.h"
#include "program.h"
#include "programcategory.h"

class RegistryManager;
/**
 * \class WindowsProgramManager
 * \author Susanne Tschernegg
 * \date 31.08.2007
 * \version 0.1
 * \brief This class handles the programs in Windows. There you can get programs, which are currently installed on the computer (for this, the registryManager is needed).
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
