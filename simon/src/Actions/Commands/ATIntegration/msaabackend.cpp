/*
 *   Copyright (C) 2008 Susanne Tschernegg <tschernegg@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "msaabackend.h"

#include <windows.h>
#include <winable.h>
#include <ole2.h>
#include <oleacc.h>
#include <commctrl.h>
/*#include <atlconv.h>
#include <comdef.h>*/

const int CLASS_COUNT = 40; 
const int WND_COUNT = 40; 

BOOL CALLBACK EnumWindowsProc (HWND hwnd, LPARAM lParam);


//---------------------------------------------------------------------- 
// Callback routine of EnumWindows() 
//---------------------------------------------------------------------- 
BOOL CALLBACK EnumWindowsProc (HWND hwnd, LPARAM lParam) 
{ 
/*  qDebug() << "--------------------";
    int nMaxCount = CLASS_COUNT;        // size of class name buffer 
    int i = 0; 
    TCHAR lpWindowName[CLASS_COUNT] = TEXT(""); 
    int iRet; 

//---------------------------------------------------------------------- 
// WindowText
//---------------------------------------------------------------------- 
	wchar_t wcharString[CLASS_COUNT];
	iRet = GetWindowText(hwnd, wcharString, nMaxCount);

    if(iRet == S_OK)
    {
        // Convert to a char*
        size_t originalSize = wcslen(wcharString) + 1;
        const size_t newsizeForWindowTitle = CLASS_COUNT;
        size_t convertedCharsIC = 0;
        char windowTitleString[newsizeForWindowTitle];
        wcstombs_s(&convertedCharsIC, windowTitleString, originalSize, wcharString, _TRUNCATE);

        qDebug() << "WindowTitle:	" << windowTitleString;
        
        windowNameList.append(windowTitleString);
    }
*/
    return TRUE; 
}

MSAABackend::MSAABackend(QObject* parent)
 : ATBackend(parent)
{
	
}

/**
 * \brief Starts the monitoring of the MSAA Bus
 * \author 
 * 
 * Call this method to start monitoring the MSAA Bus for running applications.
 * Once we find something on this bus (i.e. the user selected a msaa-aware application window with his mouse) we create a
 * tree-like structure (described in detail in the ATBackend class) that represents the window and its widgets.
 * 
 * This is then emitted with the objectFound(...) signal (also documented in the ATBackend class
 */
void MSAABackend::startMonitoring()
{
	//TODO: Implement monitoring
}


void MSAABackend::stopMonitoring()
{
	//TODO: Implement stopping
}

/**
 * \brief 
 * \author Susanne Tschernegg
**/
QStringList MSAABackend::getWindowNames()
{
    //CoInitialize(NULL);

//	TCHAR  lpTitle[WND_COUNT];  // title string of console 
    //LPARAM lParam = 0; 
    //BOOL   bRet; 

    //bRet = EnumWindows ((WNDENUMPROC)EnumWindowsProc, lParam); 

/*	size_t sizeOfTitle = wcslen((const wchar_t*)lpTitle) + 1;
    const size_t newsize3 = 300;
    size_t convertedCharsTheSecondO = 0;
    char titleString[newsize3];
    wcstombs_s(&convertedCharsTheSecondO, titleString, sizeOfTitle, (const wchar_t*)lpTitle, _TRUNCATE);

    qDebug() << titleString << "Title";

    //qDebug() << lpTitle; 

    getchar(); 
    */
    //CoUninitialize ( );
    return this->windowNameList;
}

/**
 * \brief 
 * \author Susanne Tschernegg
**/
QString MSAABackend::getWindowClassName(QString windowName)
{
/*    int nMaxCount = CLASS_COUNT;        // size of class name buffer 
    int i = 0; 
    TCHAR lpClassName[CLASS_COUNT] = TEXT(""); 
    int iRet; 
    
    HWND hwnd = FindWindow(NULL, (LPCTSTR)windowName);
    
//---------------------------------------------------------------------- 
// ClassName
//---------------------------------------------------------------------- 
	wchar_t wcharClassString[CLASS_COUNT];
	iRet = GetClassName(hwnd, wcharClassString, nMaxCount);

	// Convert to a char*
    size_t originalSizeTwo = wcslen(wcharClassString) + 1;*/
    const size_t newsizeForClassName = CLASS_COUNT;
/*    size_t convertedCharsICTwo = 0;*/
    char classNameString[newsizeForClassName];
/*    wcstombs_s(&convertedCharsICTwo, classNameString, originalSizeTwo, wcharClassString, _TRUNCATE);

	qDebug() << "ClassName:	" << classNameString;
*/    
    return classNameString;
}

/**
 * \brief 
 * \author Susanne Tschernegg
**/
QString MSAABackend::getForegroundWindowName()
{
    QString windowTitle;
/*    int nMaxCount = CLASS_COUNT;        // size of class name buffer 
    int i = 0; 
    TCHAR lpWindowName[CLASS_COUNT] = TEXT(""); 
    int iRet; 
    
    HWND hwnd = GetForegroundWindow(VOID);
    
    wchar_t wcharString[CLASS_COUNT];
	iRet = GetWindowText(hwnd, wcharString, nMaxCount);

    if(iRet == S_OK)
    {
        // Convert to a char*
        size_t originalSize = wcslen(wcharString) + 1;
        const size_t newsizeForWindowTitle = CLASS_COUNT;
        size_t convertedCharsIC = 0;
        char windowTitleString[newsizeForWindowTitle];
        wcstombs_s(&convertedCharsIC, windowTitleString, originalSize, wcharString, _TRUNCATE);

        qDebug() << "WindowTitle:	" << windowTitleString;
        windowTitle = windowTitleString;
    }*/
    return windowTitle;
}

/**
 * \brief 
 * \author Susanne Tschernegg
**/
ATOLocation MSAABackend::getLocation(QString windowName)
{
/*    HWND hwnd = FindWindow(NULL, (LPCTSTR)windowName);
    
//---------------------------------------------------------------------- 
// ClientRect
//---------------------------------------------------------------------- 
	RECT lpRect;
	GetWindowRect(hwnd, &lpRect);

	qDebug() << "left: " << lpRect.left;
	qDebug() << "right: " << lpRect.right;
	qDebug() << "top: " << lpRect.top;
	qDebug() << "bottom: " << lpRect.bottom;*/
    
    ATOLocation location;
    return location;
}

MSAABackend::~MSAABackend()
{
	
}


