//
// C++ Interface: windowsresourcehandler
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef  WINDOWSRESOURCEHANDLER_H
#define  WINDOWSRESOURCEHANDLER_H

class QIcon;
class QString;

/**
	@author Susanne Tschernegg
*/
/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date
 * \version 0.1
 * \brief
 */
class WindowsResourceHandler
{
public:
    WindowsResourceHandler();
    ~WindowsResourceHandler(){}

    QIcon retrieveIcon(QString file, int resourceId);
};

#endif
