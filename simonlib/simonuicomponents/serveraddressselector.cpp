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

#include "serveraddressselector.h"
#include "addserverconnection.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QPointer>
#include <QIcon>
#include <KLineEdit>
#include <KI18n/klocalizedstring.h>
#include <KWidgetsAddons/kmessagebox.h>
#include <KLocale>
#include <KWidgetsAddons/KMessageBox>

ServerAddressSelector::ServerAddressSelector(QWidget *parent) : QWidget(parent)
{
  QHBoxLayout *lay = new QHBoxLayout(this);
  leServerAddress = new KLineEdit(this);
  leServerAddress->setToolTip(i18n("IP address or host name and port using &lt;host&gt;:&lt;port&gt;"));
  pbSelectServerAddress = new QToolButton(this);

  pbSelectServerAddress->setIcon(QIcon::fromTheme("go-previous"));
  pbSelectServerAddress->setText(i18n("Add new host"));
  pbSelectServerAddress->setToolTip(i18n("Select a new host / port."));

  // 	leServerAddress->setReadOnly(true);

  lay->addWidget(leServerAddress);
  lay->addWidget(pbSelectServerAddress);

  connect(pbSelectServerAddress , SIGNAL(clicked()), this, SLOT(displayAddDialog()));
}


KLineEdit* ServerAddressSelector::lineEdit()
{
  return leServerAddress;
}


void ServerAddressSelector::displayAddDialog()
{
  QPointer<AddServerConnection> addDlg = new AddServerConnection(this);
  if (addDlg->exec()) {
    QString host=addDlg->getHost();
    int port = addDlg->getPort();

    if (host.isEmpty())
      KMessageBox::information(this, i18n("You have entered an empty host address. The input will be discarded."));
    else leServerAddress->setText(host+':'+QString::number(port));
  }
  delete addDlg;
}


ServerAddressSelector::~ServerAddressSelector()
{
  leServerAddress->deleteLater();
  pbSelectServerAddress->deleteLater();
}
