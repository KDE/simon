/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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


#include "addserverconnection.h"

#include <QLabel>
#include <KLocalizedString>
#include <KLineEdit>
#include <KIntNumInput>
#include <QFormLayout>

AddServerConnection::AddServerConnection(QWidget *parent) : KDialog(parent)
{
	QWidget *widget = new QWidget( this );

	leAddress = new KLineEdit("localhost", widget);
	sbPort = new KIntNumInput(widget);
	sbPort->setMinimum(0);
	sbPort->setMaximum(65000);
	sbPort->setValue(4444);
	
	QFormLayout *lay = new QFormLayout(widget);
	lay->addRow(i18n("Addresse:"), leAddress);
	lay->addRow(i18n("Port:"), sbPort);
	
	setMainWidget( widget );
	setCaption( i18n("Serververbindung hinzufügen") );
}

QString AddServerConnection::getHost()
{
	return leAddress->text();
}

int AddServerConnection::getPort()
{
	return sbPort->value();
}
