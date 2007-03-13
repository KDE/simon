//
// C++ Implementation: runwindowsbackend
//
// Description: 
//
//
// Author: Gigerl Martin <gigmab03@htl-kaindorf.ac.at>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "runwindowsbackend.h"


/**
 *	@brief Constructor
 *
 *	@author Gigerl Martin
 */
RunWindowsBackend::RunWindowsBackend()
{
}

/**
 *	@brief Passes the given URL to the systems URL Handler
 *	
 *	Uses QDesktopServices
 *
 *	@param QString place
 *	The URL
 *	@author Gigerl Martin
 */
void RunWindowsBackend::goTo(QString place)
{
	QDesktopServices::openUrl(QUrl(place));
}

/**
 *	@brief Runs the command
 *	
 *	This doesn't need the full path as long as the command lies within $PATH
 *
 *	@param QString command
 *	The command - it will be passed to the system as-is
 *	@author Gigerl Martin
 */
void RunWindowsBackend::run(QString command)
{
	QChar* com = command.data();
	char asciicommand[500];
	
	for (int i=0; i < command.size(); i++)
		asciicommand[i] = com[i].toAscii();
	
	system(asciicommand);
}


/**
 *	@brief Destructor
 *
 *	@author Gigerl Martin
 */
RunWindowsBackend::~RunWindowsBackend()
{
}


