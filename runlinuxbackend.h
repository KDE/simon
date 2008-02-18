//
// C++ Interface: runlinuxbackend
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RUNLINUXBACKEND_H
#define RUNLINUXBACKEND_H

#include "runbackend.h"


/**
 *	@class RunLinuxBackend
 *	@brief The Linux Run Backend
 *
 *	Used as interface to the (Linux) Operating System
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class RunLinuxBackend : public RunBackend {
public:
    RunLinuxBackend();
    
    void goTo(QString place);
    void run(QString command, QString workingDir);

    ~RunLinuxBackend();

};

#endif
