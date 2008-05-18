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

#include <QProcess>
#include <QSettings>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <QFile>
#include <QStringList>
#include "../SimonLib/Logging/logger.h"
#include "../SimonLib/WindowsLib/registrymanager.h"



using namespace std;

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
 *	@author Susanne Tschernegg
 *	@param QString command
 *	     command - it will be passed to the system as-is
 *  @param QString workingDirectory
 *      holds the workingdirectory
 */
void RunWindowsBackend::run(QString command, QString workingDirectory)
{
   RegistryManager *rm = new RegistryManager();
    rm->startProcess(command, workingDirectory);
}


/**
 *	@brief Destructor
 *
 *	@author Gigerl Martin
 */
RunWindowsBackend::~RunWindowsBackend()
{
}


