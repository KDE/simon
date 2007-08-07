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

#include "logger.h"
#include <QProcess>
#include <QSettings>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>


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
 *	This doesn't need the full path as long as the command lies within $PATH
 *
 *	@param QString command
 *	The command - it will be passed to the system as-is
 *	@author Gigerl Martin / Goriup Phillip
 */
void RunWindowsBackend::run(QString command)
{

	//command = winword.exe

    HKEY    hKey = NULL;
    BYTE    bData[MAX_PATH + 1] = {0};
    DWORD   dwDataLen = MAX_PATH;
    string result;

    QByteArray path("\\Applications\\"+command.toAscii()+"\\shell\\open\\command");
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT ,(WCHAR*) path.data(), 0,KEY_QUERY_VALUE,&hKey) != ERROR_SUCCESS) result = "0";
    if (RegQueryValueEx(hKey,TEXT("App"),NULL,NULL,bData,&dwDataLen) == ERROR_SUCCESS) result = (string)((char*)bData);
    else result = "0";
    RegCloseKey(hKey);

    Logger::log(path+result.c_str());
    
    
    
	//QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
	//settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
	//Logger::log(settings.value("Personal").toString());

	//QSettings registry;
	//QString settingspath= "/Applications/"+command+"/shell/open/command";
	//QString pathToCommand = registry.value(settingspath, "gibsnet" ).toString();
	//Logger::log( "/Applications/"+command+"/shell/open/command: "+pathToCommand);
	//QProcess::startDetached(pathToCommand);


	/*char *asciicommand;
	asciicommand = command.toAscii().data();
	Logger::log(asciicommand);
	system(asciicommand);*/
}


/**
 *	@brief Destructor
 *
 *	@author Gigerl Martin
 */
RunWindowsBackend::~RunWindowsBackend()
{
}


