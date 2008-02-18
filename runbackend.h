//
// C++ Interface: runbackend
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RUNBACKEND_H
#define RUNBACKEND_H


#include <QString>

/**
	@interface RunBackend
	@brief Backend for the Run-System
	
	Defines all the function needed for a fully functional
	Backend to run commands.
	This includes the execution of commands

	@author Peter Grasch <bedahr@gmx.net>
*/
class RunBackend{
	
	public:
		virtual void run(QString command, QString workingDirectory = "")=0;
		virtual void goTo(QString place)=0;
		virtual ~RunBackend() {}
};

#endif
