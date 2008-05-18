//
// C++ Implementation: iconbutton
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "iconbutton.h"

#include "icondialog.h"

#ifdef __WIN32
#include "../WindowsLib/windowsresourcehandler.h"
#endif

/**
*   \brief constructor, which creates a new iconDialog
*   @author Susanne Tschernegg
*   @param QWidget *parent
*/
IconButton::IconButton ( QWidget *parent ) : QToolButton ( parent )
{
	iconName = "";
	iconDialog = new IconDialog();
	connect ( this, SIGNAL ( clicked() ), this, SLOT ( changeIcon() ) );
}

/**
* \brief This method opens a dialog, to change the current Icon.
*
* \author Susanne Tschernegg
*/
void IconButton::changeIcon()
{
	iconDialog = new IconDialog ( this );
	iconDialog->showIcons ();// getIconName() );

	int success = iconDialog->exec();

	if ( success )
	{
		QString resourceId = iconDialog->getIcon();

		QIcon icon;
#ifdef __WIN32
		if ( resourceId.contains ( QRegExp ( ".dll,\n*" ) ) )
		{
			QStringList iconResources = resourceId.split ( "," );
			WindowsResourceHandler *windowsResourceHandler = new WindowsResourceHandler();
			icon = windowsResourceHandler->retrieveIcon ( iconResources.at ( 0 ), iconResources.at ( 1 ).toInt() );
	             delete windowsResourceHandler;
		}
		else
		{
			QPixmap pixmap ( resourceId );
			icon.addPixmap ( pixmap );
		}
#endif
#ifndef __WIN32
		QPixmap pixmap ( resourceId );
		icon.addPixmap ( pixmap );
#endif
		setIconName ( resourceId );
		setIcon ( icon );
	}
}


 
IconButton::~IconButton()
{
    iconDialog->deleteLater();
}
