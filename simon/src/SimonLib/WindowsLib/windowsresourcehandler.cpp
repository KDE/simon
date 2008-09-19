//
// C++ Implementation: windowsresourcehandler
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "windowsresourcehandler.h"

#include <windows.h>

#include <QString>
#include <QPixmap>
#include <QIcon>


/**
*   \brief constructor
*   @author Susanne Tschernegg
*/
WindowsResourceHandler::WindowsResourceHandler()
{

}

/**
*   \brief This method retrievs the chosen icon from the library
*   @author Susanne Tschernegg
*   @param QString file
*       holds the file, where the icon is saved ("shell32.dll", etc.)
*   @param int resourceId
*       holds the id of a special icon (the 3rd icon in the dll)
*   @return QIcon
*       returns a icon from the dll
*/
QIcon WindowsResourceHandler::retrieveIcon(QString file, int resourceId)
{
    QPixmap iconpixmap;
    int size = 32;
    HMODULE hmod = LoadLibraryA((const char*)file.toUtf8());
    if( hmod ) {
        HICON iconHandle = (HICON)LoadImage(hmod, MAKEINTRESOURCE(resourceId), IMAGE_ICON, size, size, 0);
        if( iconHandle )
        {
            ICONINFO iconInfo;
            GetIconInfo(iconHandle,&iconInfo);
            
            HBITMAP ic = iconInfo.hbmMask;
            iconpixmap =  QPixmap::fromWinHBITMAP( iconInfo.hbmColor, QPixmap::PremultipliedAlpha);
            DestroyIcon(iconHandle);
        }
    }
        
    QIcon icon(iconpixmap);
    return icon;
}
