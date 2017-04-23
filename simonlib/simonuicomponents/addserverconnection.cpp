/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include <KI18n/klocalizedstring.h>
#include <QLineEdit>
#include <KDELibs4Support/knuminput.h>
#include <QFormLayout>
#include <KLocalizedString>

AddServerConnection::AddServerConnection(QWidget *parent) : QDialog(parent)
{
  QWidget *widget = new QWidget( this );

  leAddress = new QLineEdit("localhost", widget);
  leAddress->setToolTip(i18n("IP address or host name"));
  sbPort = new KIntNumInput(widget);
  sbPort->setMinimum(0);
  sbPort->setMaximum(65000);
  sbPort->setValue(4444);

  QFormLayout *lay = new QFormLayout(widget);
  lay->addRow(i18nc("IP address or host name", "Address:"), leAddress);
  lay->addRow(i18nc("Network port", "Port:"), sbPort);

//PORTING: Verify that widget was added to mainLayout: //PORTING: Verify that widget was added to mainLayout: //PORTING: Verify that widget was added to mainLayout:   setMainWidget( widget );
// Add mainLayout->addWidget(widget); if necessary
// Add mainLayout->addWidget(widget); if necessary
// Add mainLayout->addWidget(widget); if necessary
  setWindowTitle( i18n("Add Server Address") );
}


QString AddServerConnection::getHost()
{
  return leAddress->text();
}


int AddServerConnection::getPort()
{
  return sbPort->value();
}
