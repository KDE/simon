//
// C++ Implementation: wordlistview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "addserverconnection.h"

AddServerConnection::AddServerConnection(QWidget *parent) : KDialog(parent)
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);
	setMainWidget( widget );
	setCaption( i18n("Serververbindung hinzufügen") );
}

QString AddServerConnection::getHost()
{
	return ui.leAddress->text();
}

int AddServerConnection::getPort()
{
	return ui.sbPort->value();
}