//
// C++ Interface: runwindowsbackend
//
// Description: 
//
//
// Author: Gigerl Martin <gigmab030@htl-kaindorf.ac.at>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RUNWINDOWSBACKEND_H
#define RUNWINDOWSBACKEND_H

#include "runbackend.h"

/**
 *	@class RunWindowsBackend
 *	@brief The Windows Run Backend
 *
 *	Used as interface to the (Windows) Operating System
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Gigerl Martin
 */
class RunWindowsBackend : public RunBackend {
public:
    RunWindowsBackend();

    void goTo(QString place);
    void run(QString command);

    ~RunWindowsBackend();

};

#endif
