//
// C++ Implementation: networksettings
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "networksettings.h"
#include <KIcon>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <KLineEdit>
#include "../SimonLib/Settings/settings.h"
#include "../SimonLib/ServerAddressSelector/serveraddressselector.h"


/**
 * \brief Constructor - inits the help text and the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
NetworkSettings::NetworkSettings(QWidget* parent): QWidget(parent) //SystemWidget(i18n("Juliusd"), KIcon("network-disconnect"), i18n("Hier können Sie Adressen zu anderen Teilen des Programmes konfigurieren"), parent)
{
	ui.setupUi(this);
	
// 	help = "simon verwendet für die Erkennung selbst die Open Source Spracherkennungs-engine \"Julius\".\nDie Verbindung zu Julius wird über das Netzwerk aufgebaut.\nDie Addresse zum Juliusd wird entweder mit einem Hostnamen oder einer direkten \"IP Adresse\" angegeben (4x3 stellige Netzwerksidentifikationsnummer - xxx.xxx.xxx.xxx)\nDie Port Nummer muss beim Juliusd und bei simon gleich sein.\n";

	ServerAddressSelector *saSelector = new ServerAddressSelector(this);
	ui.kcfg_JuliusdServers->setCustomEditor(*(new KEditListBox::CustomEditor(saSelector, saSelector->lineEdit())));

// 	connect ( ui.pbAddAddress, SIGNAL ( clicked() ), this, SLOT ( addAddress() ) );
// 	connect ( ui.pbDeleteAddress, SIGNAL ( clicked() ), this, SLOT ( deleteAddress() ) );
// 	connect ( ui.tbMoveUp, SIGNAL ( clicked() ), this, SLOT ( moveUp() ) );
// 	connect ( ui.tbMoveDown, SIGNAL ( clicked() ), this, SLOT ( moveDown() ) );
	
// 	connect( ui.twJuliusAddresses, SIGNAL(itemSelectionChanged()), this, SLOT(enableButtons()));
// 	connect( ui.twJuliusAddresses, SIGNAL(cellChanged(int, int)), this, SIGNAL(changed()));
// 	connect(ui.cbAutoConnect, SIGNAL(stateChanged(int)), this, SIGNAL(changed()));
// 	connect(ui.leUser, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
// 	connect(ui.lePass, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	
// 	guessChildTriggers(this);

// 	ui.tbMoveUp->setIcon(KIcon("go-up"));
// 	ui.tbMoveDown->setIcon(KIcon("go-down"));
// 	ui.pbAddAddress->setIcon(KIcon("list-add"));
// 	ui.pbDeleteAddress->setIcon(KIcon("list-remove"));
}


/**
 * \brief Enables/Diaables the Remove/Move Up/Move Down buttons according to the current selection
 * \author Peter Grasch
 */
// void NetworkSettings::enableButtons()
// {
// 	int row = ui.twJuliusAddresses->currentRow();
// 	int rowCount = ui.twJuliusAddresses->rowCount();
// 	if (row != -1)
// 	{
// 		ui.tbMoveUp->setEnabled((row > 0) ? true : false);
// 		ui.tbMoveDown->setEnabled((row < rowCount-1) ? true : false);
// 		ui.pbDeleteAddress->setEnabled(true);
// 	} else
// 	{
// 		ui.tbMoveUp->setEnabled(false);
// 		ui.tbMoveDown->setEnabled(false);
// 		ui.pbDeleteAddress->setEnabled(false);
// 	}
// }

/**
 * \brief Returns true if there is at least one address configured
 * \author Peter Grasch
 * @return True, if we have one juliusd host
 */
// bool NetworkSettings::isComplete()
// {
// 	return (ui.twJuliusAddresses->rowCount() > 0) && 
// 		(!ui.leUser->text().isEmpty()) && 
// 		(!ui.lePass->text().isEmpty());
// }

/**
 * \brief Applys the new list of servers
 * \author Peter Grasch
 * @return success
 */
// bool NetworkSettings::apply()
// {
// 	//juliusd
// // 	Settings::set("Network/JuliusdAddress", ui.cbAddress->currentText());
// 	QString juliusServers;
// 	for (int i=0; i < ui.twJuliusAddresses->rowCount(); i++)
// 	{
// 		juliusServers += ui.twJuliusAddresses->item(i, 0)->text()+
// 				":"+ui.twJuliusAddresses->item(i, 1)->text()+";";
// 	}
// 	Settings::set("Network/JuliusdServers", juliusServers);
// 	Settings::set("Network/Timeout", ui.sbTimeout->value());
// 
// 	Settings::set("Juliusd/Username", ui.leUser->text());
// 	Settings::set("Juliusd/Password", ui.lePass->text());
// 
// 	Settings::set("Juliusd/Encrypted", ui.cbUseEncryption->isChecked());
// 	Settings::set("Juliusd/ContinueOnWarning", ui.cbIgnoreWarnings->isChecked());
// 	Settings::set("Juliusd/AutoConnect", ui.cbAutoConnect->isChecked());
// 
// 	return true;
// }


/**
 * \brief Inits the list of servers
 * \author Peter Grasch
 * @return success
 */
// bool NetworkSettings::init()
// {
// 	ui.sbTimeout->setValue(Settings::getI("Network/Timeout"));
// 	
// 	QTableWidgetItem *header1 = new QTableWidgetItem(i18n("Adresse"));
// 	ui.twJuliusAddresses->setHorizontalHeaderItem(0, header1);
// 	QTableWidgetItem *header2 = new QTableWidgetItem(i18n("Port"));
// 	ui.twJuliusAddresses->setHorizontalHeaderItem(1, header2);
// 
// 	
// 	//juliusd
// 	QString juliusServers=Settings::getS("Network/JuliusdServers");
// 	if (juliusServers.isEmpty()) return true;
// 	
// 	while (juliusServers.contains(";"))
// 	{
// 		int portstart = juliusServers.indexOf(":");
// 		int end = juliusServers.indexOf(";");
// 		insertAddress(juliusServers.left(portstart), juliusServers.mid(portstart+1, 
// 				end-portstart-1).toInt());
// 		juliusServers = juliusServers.mid(end+1);
// 	}
// 
// 	ui.leUser->setText(Settings::getS("Juliusd/Username"));
// 	ui.lePass->setText(Settings::getS("Juliusd/Password"));
// 	ui.cbAutoConnect->setChecked(Settings::getB("Juliusd/AutoConnect"));
// 
// 
// 	ui.cbIgnoreWarnings->setChecked(Settings::getB("Juliusd/ContinueOnWarning"));
// 
// 	ui.cbUseEncryption->setChecked(Settings::getB("Juliusd/Encrypted"));
// 	
// 	return true;
// }


/**
 * \brief Resets the list of servers
 * \author Peter Grasch
 * @return success
 */
// bool NetworkSettings::reset()
// {
// 	ui.twJuliusAddresses->clear();
// 	ui.twJuliusAddresses->setRowCount(0);
// 	return init();
// }

/**
 * \brief Moves the selected address up by one
 * \author Peter Grasch
 */
// void NetworkSettings::moveUp()
// {
// 	int row = ui.twJuliusAddresses->currentRow();
// 	QString above1 = ui.twJuliusAddresses->item(row-1,0)->text();
// 	QString above2 = ui.twJuliusAddresses->item(row-1,1)->text();
// 	QString tomove1 = ui.twJuliusAddresses->item(row,0)->text();
// 	QString tomove2 = ui.twJuliusAddresses->item(row,1)->text();
// 	
// 	
// 	ui.twJuliusAddresses->item(row,0)->setText(above1);
// 	ui.twJuliusAddresses->item(row,1)->setText(above2);
// 	ui.twJuliusAddresses->item(row-1,0)->setText(tomove1);
// 	ui.twJuliusAddresses->item(row-1,1)->setText(tomove2);
// 	
// 	ui.twJuliusAddresses->setCurrentCell(row-1, 0);
// 	enableButtons();
// }

/**
 * \brief Moves the selected address down by one
 * \author Peter Grasch
 */
// void NetworkSettings::moveDown()
// {
// 	int row = ui.twJuliusAddresses->currentRow();
// 	QString below1 = ui.twJuliusAddresses->item(row+1,0)->text();
// 	QString below2 = ui.twJuliusAddresses->item(row+1,1)->text();
// 	QString tomove1 = ui.twJuliusAddresses->item(row,0)->text();
// 	QString tomove2 = ui.twJuliusAddresses->item(row,1)->text();
// 	
// 	
// 	ui.twJuliusAddresses->item(row,0)->setText(below1);
// 	ui.twJuliusAddresses->item(row,1)->setText(below2);
// 	ui.twJuliusAddresses->item(row+1,0)->setText(tomove1);
// 	ui.twJuliusAddresses->item(row+1,1)->setText(tomove2);
// 	
// 	ui.twJuliusAddresses->setCurrentCell(row+1, 0);
// 	enableButtons();
// }


/**
 * \brief Deletes an address from the list
 * \author Peter Grasch
 */
// void NetworkSettings::deleteAddress()
// {
// 	ui.twJuliusAddresses->removeRow(ui.twJuliusAddresses->currentRow());
// 	enableButtons();
// }

// void NetworkSettings::insertAddress(QString host, int port)
// {
// 	int newRow = ui.twJuliusAddresses->rowCount();
// 	ui.twJuliusAddresses->setRowCount(newRow+1);
// 	ui.twJuliusAddresses->setItem(newRow, 0, new QTableWidgetItem(host));
// 	ui.twJuliusAddresses->setItem(newRow, 1, new QTableWidgetItem(QString::number(port)));
// 	ui.twJuliusAddresses->resizeColumnToContents(0);
// 	enableButtons();
// }

/**
 * \brief Adds an address to the list
 * \author Peter Grasch
 */
// void NetworkSettings::addAddress()
// {
// 	AddServerConnection *addDlg = new AddServerConnection(this);
// 	if (addDlg->exec())
// 	{
// 		QString host=addDlg->getHost();
// 		int port = addDlg->getPort();
// 		
// 		if (host.isEmpty())
// 			KMessageBox::information(this, i18n("Sie haben eine leere Hostadresse angegeben. Die Eingaben werden verworfen."));
// 		else insertAddress(host, port);
// 	}
// 	addDlg->deleteLater();
// }


NetworkSettings::~NetworkSettings()
{}


