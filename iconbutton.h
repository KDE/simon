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

class QIcon;
class IconDialog;

/**
	@author Susanne Tschernegg
*/
/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date 13.12.2007
 * \version 0.1
 * \brief This button is included at the commandsettings, where the user can choose a icon. There the iconDialog will be called.
 */
class IconButton : public QToolButton
{
    Q_OBJECT
    
private:
    QString iconName;
    IconDialog *iconDialog;
    
public:
    IconButton(QWidget *parent = 0);
    ~IconButton(){}

    QString getIconName(){return iconName;}
    void setIconName(QString iconName){this->iconName = iconName;}

private slots:
    void changeIcon();

signals:
    void iconSelected(QString resourceId);
};

#endif
