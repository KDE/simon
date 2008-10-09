/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "settingsview.h"
#include "settings.h"
#include <QVariant>
#include <QSqlDatabase>
#include <QSslSocket>
#include <QSslCipher>
/**
 * @brief Constuctor
 * 
 * 	Sets up the UI
 *
 *	@author Peter Grasch
 *
 *	@param QWidget *parent
 *	The widgets parent
 *	@param Qt::WFlags flags
 *	The windowflags
 */
SettingsView::SettingsView(QWidget *parent, Qt::WFlags flags) : QDialog(parent,flags)
{
	ui.setupUi(this);
	ui.pbAccept->setDefault(true);
	connect(this, SIGNAL(accepted()), this, SLOT(save()));
}



/**
 * \brief Saves the current state to the config file
 * \author Peter Grasch
 */
void SettingsView::save()
{
	Settings::set("RestrictAccess", ui.cbRestrictAccess->isChecked());
	Settings::set("AllowedHost", ui.leHost->text());
	Settings::set("Port", ui.sbPort->value());
	Settings::set("Command", ui.leCommand->text());
	Settings::set("Arguments", ui.leArgs->text());
	Settings::set("OutArgs", (ui.cbWords->isChecked() ? WORD : 0) | 
				(ui.cbInfo->isChecked() ? INFO : 0) | 
				(ui.cbError->isChecked() ? ERROR : 0));
	Settings::set("StartArgs", (ui.cbStartMinimized->isChecked() ? STARTMINIMIZED : 0) | 
				(ui.cbConnectOnStart->isChecked() ? CONNECTONSTART : 0) | 
				(ui.cbStartOnSystemStart->isChecked() ? AUTOSTART : 0));

	Settings::set("Encryption", ui.gbEncryption->isChecked());
	Settings::set("Cert", ui.leCert->text());
	Settings::set("Cipher", ui.cbCipher->currentText());


	Settings::set("DbType", ui.cbDbType->currentText());
	Settings::set("DbHost", ui.leDbHost->text());
	Settings::set("DbUser", ui.leDbUser->text());
	Settings::set("DbPass", ui.leDbPass->text());
	Settings::set("DbPort", ui.sbDbPort->value());
	Settings::set("DbName", ui.leDbName->text());
}


/**
 * @brief Overrides the original exec function and sets the GUI options to the supplied values
 * 
 * 	This is used to set the GUI to the Options supplied
 * 	This shouldn't be nesscesary while executing as the values only change by changing the
 * 	corresponding GUI elements.
 * 	But if we do this that way we also take care about the loading of the defaults and so on
 *
 *	@author Peter Grasch
 *	@return int
 *	returns if the QDialog was either accepted or rejected
 */
int SettingsView::exec()
{
	
	if (Settings::get("RestrictAccess").toBool())
		ui.cbRestrictAccess->setCheckState(Qt::Checked);

	ui.leHost->setText(Settings::getS("AllowedHost"));
	ui.sbPort->setValue(Settings::get("Port").toInt());

	ui.leCommand->setText(Settings::getS("Command"));
	ui.leArgs->setText(Settings::getS("Arguments"));
	
	int outArgs = Settings::get("OutArgs").toInt();
	if (outArgs & WORD)
		ui.cbWords->setChecked(true);
	else ui.cbWords->setChecked(false);
	
	if (outArgs & INFO)
		ui.cbInfo->setChecked(true);
	else ui.cbInfo->setChecked(false);
	
	if (outArgs & ERROR)
		ui.cbError->setChecked(true);
	else ui.cbError->setChecked(false);
	
	
	int startArgs = Settings::get("StartArgs").toInt();
	if (startArgs & STARTMINIMIZED)
		ui.cbStartMinimized->setChecked(true);
	else ui.cbStartMinimized->setChecked(false);
	
	if (startArgs & CONNECTONSTART)
		ui.cbConnectOnStart->setChecked(true);
	else ui.cbConnectOnStart->setChecked(false);
	
	if (startArgs & AUTOSTART)
		ui.cbStartOnSystemStart->setChecked(true);
	else ui.cbStartOnSystemStart->setChecked(false);


	ui.gbEncryption->setChecked(Settings::getB("Encryption"));
	QString selectedCipher = Settings::getS("Cipher");
	int selectedIndex=0;
	QList<QSslCipher> ciphers = QSslSocket::supportedCiphers();
	QStringList cipherStrs;
	QString cipherName;
	for (int i=0; i < ciphers.count(); i++)
	{
		cipherName = ciphers[i].name();
		if (cipherName == selectedCipher)
			selectedIndex =i;
		cipherStrs << cipherName;
	}

	ui.cbCipher->clear();
	ui.cbCipher->addItems(cipherStrs);
	ui.cbCipher->setCurrentIndex(selectedIndex);

	ui.leCert->setText(Settings::getS("Cert"));



	ui.cbDbType->clear();
	QStringList drivers = QSqlDatabase::drivers();
	selectedIndex=0;
	QString selectedDriver=Settings::getS("DbType");
	for (int i=0; i<drivers.count(); i++)
		if (drivers[i] == selectedDriver) {
			selectedIndex=i;
			break;
		}

	ui.cbDbType->addItems(drivers);
	ui.cbDbType->setCurrentIndex(selectedIndex);

	ui.leDbHost->setText(Settings::getS("DbHost"));
	ui.sbDbPort->setValue(Settings::get("DbPort").toInt());
	ui.leDbUser->setText(Settings::getS("DbUser"));
	ui.leDbPass->setText(Settings::getS("DbPass"));
	ui.leDbName->setText(Settings::getS("DbName"));
	
	return QDialog::exec();
}



/**
 *	@brief Destructor
 * 
 *	@author Peter Grasch
 */
SettingsView::~SettingsView()
{
}


