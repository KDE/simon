    //simonIconButton abgeleitet von QToolButton ...
    //iconname, get ... //name dem commandloader übergeben und dort auf dem objekt zwischenspeichern
    //icondialog anzeigen
    //signal: iconselected(QString ...)
    
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

#include <QMessageBox>

#include "windowsresourcehandler.h"

/**
*   \brief constructor, which creates a new iconDialog
*   @author Susanne Tschernegg
*/
IconButton::IconButton(QWidget *parent): QToolButton(parent)
{
    iconName = "";
    iconDialog = new IconDialog();
   // connect(this, SIGNAL(clicked()), this, SLOT(changeIcon()));
    connect(iconDialog, SIGNAL(iconSelected(QString)), this, SLOT(iconSelected(QString)));
}

/**
* \brief This method opens a dialog, to change the current Icon.
*
* \author Susanne Tschernegg
*/
void IconButton::changeIcon()
{
  /*  int currRow = twCommand->currentRow();
    QToolButton *pbIcon = dynamic_cast<QToolButton*>(twCommand->cellWidget(currRow,0));
    if(!pbIcon)
    {
        //QMessageBox::information(this,"commandsettings::changeIcon","!pbIcon");
        return;
    }*/
    
    //simonIconButton abgeleitet von QToolButton ...
    //iconname, get ... //name dem commandloader übergeben und dort auf dem objekt zwischenspeichern
    //icondialog anzeigen
    //signal: iconselected(QString ...)
    iconDialog = new IconDialog(this);
    iconDialog->showIcons(getIconName());
    int success = iconDialog->exec();
    
    if(success)
    {
        QString resourceId = iconDialog->getIcon();
        
        QIcon icon;
        if(resourceId.contains(QRegExp(".dll,\n*")))
        {    
            QStringList iconResources = resourceId.split(",");      
            WindowsResourceHandler *windowsResourceHandler = new WindowsResourceHandler();
            icon = windowsResourceHandler->retrieveIcon(iconResources.at(0), iconResources.at(1).toInt());
        }
        else
        {
            QPixmap pixmap(resourceId);
            icon.addPixmap(pixmap);
        }
        setIconName(resourceId);
        setIcon(icon);
    }
}
