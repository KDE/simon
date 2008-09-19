//
// C++ Interface: internetextensionsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef ADDSERVERCONNECTION_H
#define ADDSERVERCONNECTION_H

#include "ui_addserverconnectiondlg.h"
#include <KDialog>

/**
 \class AddServerConnection
 \author Peter Grasch
 \version 0.1
 \date 12.08.2007

 \brief Little Dialog to enter server address and port into

*/
class AddServerConnection : public KDialog
{
Q_OBJECT

private:
	Ui::AddServerConnectionDlg ui; //!< UI definition - made by uic from the QTDesigner .ui
	
public:
    explicit AddServerConnection(QWidget* parent=0);
    QString getHost();
    int getPort();

};

#endif
