//
// C++ Interface: passworddlg
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PASSWORDDLG_H
#define PASSWORDDLG_H

#include <QDialog>
class QLineEdit;

/**
	@author Susanne Tschernegg
*/
/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date 12.09.2007
 * \version 0.1
 * \brief If the user has blocked the settings with a password, this dialog will be called to enter the password and to change the settings.
 */
class PasswordDlg : public QDialog
{
    Q_OBJECT
    
public:
        PasswordDlg(QWidget *parent = 0);

       // ~PasswordDlg();

        QLineEdit *lePassword;
};

#endif
