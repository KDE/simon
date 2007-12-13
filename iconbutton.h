//
// C++ Interface: iconbutton
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ICONBUTTON_H
#define  ICONBUTTON_H

#include <QToolButton>
#include <QString>

#include "icondialog.h"

class QIcon;

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
class IconButton : public QToolButton
{
    Q_OBJECT
        //simonIconButton abgeleitet von QToolButton ...
    //iconname, get ... //name dem commandloader übergeben und dort auf dem objekt zwischenspeichern
    //icondialog anzeigen
    //signal: iconselected(QString ...)
    
private:
    QString iconName;
    IconDialog *iconDialog;
    
public:
    IconButton(QWidget *parent = 0);
    ~IconButton(){}

    QString getIconName(){return iconName;}
    void setIconName(QString iconName){this->iconName = iconName;}
    //QIcon loadIcon(QString file, int resourceId);

private slots:
    void changeIcon();

signals:
    void iconSelected(QString resourceId);
};

#endif
