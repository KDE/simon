//
// C++ Interface: serveraddressselector
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "serveraddressselector.h"
#include "addserverconnection.h"
#include <QHBoxLayout>
#include <KLineEdit>
#include <QToolButton>
#include <KIcon>
#include <KLocalizedString>
#include <KMessageBox>

ServerAddressSelector::ServerAddressSelector(QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *lay = new QHBoxLayout(this);
	leServerAddress = new KLineEdit(this);
	pbSelectServerAddress = new QToolButton(this);

	pbSelectServerAddress->setIcon(KIcon("go-previous"));
	pbSelectServerAddress->setText(i18n("Neuen Host anlegen"));

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
	AddServerConnection *addDlg = new AddServerConnection(this);
	if (addDlg->exec())
	{
		QString host=addDlg->getHost();
		int port = addDlg->getPort();
		
		if (host.isEmpty())
			KMessageBox::information(this, i18n("Sie haben eine leere Hostadresse angegeben. Die Eingaben werden verworfen."));
		else leServerAddress->setText(host+":"+QString::number(port));
	}
}

ServerAddressSelector::~ServerAddressSelector()
{
	leServerAddress->deleteLater();
	pbSelectServerAddress->deleteLater();
}
