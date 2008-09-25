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

#include "passwordsettings.h"
#include "../SimonLib/Settings/settings.h"
#include <KMessageBox>
#include <KIcon>
#include <QCryptographicHash>
#include <KLocalizedString>


//TODO: PORT ME TO KNEWPASSWORDDIALOG

PasswordSettings::PasswordSettings(QWidget* parent): QWidget(parent) //SystemWidget(i18n("Passwort-Sicherung"), KIcon("document-encrypt"), i18n("Sichern Sie simons Systemeinstellungen mit einem Passwort"), parent)
{
	ui.setupUi(this);
// 	help=i18n("Wenn der Passwortschutz deaktiviert ist, muss nur auf den Knopf \"Systemverwaltungsmodus\" geklickt werden, damit alle (System-) Optionen angezeigt werden.");
//	connect(ui.lePassword2, SIGNAL(editingFinished()), this, SLOT(checkPassword()));
//	connect(ui.lePassword, SIGNAL(textChanged(QString)), this, SLOT(unsetHashed()));
//	connect(ui.lePassword2, SIGNAL(textChanged(QString)), this, SLOT(unsetHashed()));
//	connect(ui.pbResetPassword, SIGNAL(toggled(bool)), this, SLOT(resetButtonClicked(bool)));


//	connect(ui.cbCheckPassword, SIGNAL(stateChanged(int)), this, SIGNAL(changed()));
//	connect(ui.lePassword, SIGNAL(editingFinished()), this, SIGNAL(changed()));
//	connect(ui.lePassword2, SIGNAL(editingFinished()), this, SIGNAL(changed()));
}

void PasswordSettings::checkPassword()
{
	if (!checkIfPasswordsSane())
		KMessageBox::error(this, i18n("Die eingegebenen Passwörter des simon-Passwortschutz' stimmen nicht überein.\n\nBitte geben Sie sie erneut ein."));
}

bool PasswordSettings::checkIfPasswordsSane()
{
	return true;
//	return (ui.lePassword->text() == ui.lePassword2->text());
}


PasswordSettings::~PasswordSettings()
{
}

void PasswordSettings::resetButtonClicked(bool isDown)
{
/*	if (isDown) {
		ui.lePassword->clear();
		ui.lePassword2->clear();
	} else
	{
		ui.lePassword->setText(originalHash);
		ui.lePassword2->setText(originalHash);
	}*/
}

/*bool PasswordSettings::apply()
{
	//password
	Settings::set("Passwordprotected", ui.cbCheckPassword->isChecked());
	if (checkIfPasswordsSane())
	{
		QString hash;
		if (!isHashed)
		{
			QCryptographicHash *hasher = new QCryptographicHash(QCryptographicHash::Md5);
//			hasher->addData(ui.lePassword->text().toUtf8());
			hash = hasher->result();
			delete hasher;
		} //else hash = ui.lePassword->text();

		Settings::set("Password", hash);
	} else {
		checkPassword(); //shows error
		return false;
	}
	if (ui.pbResetPassword->isChecked())
	{
		ui.pbResetPassword->animateClick();
	}

	return true;
}*/
/*
bool PasswordSettings::isComplete()
{
	return true;
//	return (!ui.cbCheckPassword->isChecked() || ((checkIfPasswordsSane()) && (!ui.lePassword->text().isEmpty())));
}

bool PasswordSettings::init()
{
	//password
	ui.cbCheckPassword->setChecked(Settings::getB("Passwordprotected"));
	originalHash = Settings::getS("Password");
	//ui.lePassword->setText(originalHash);
	//ui.lePassword2->setText(originalHash);
	isHashed = true;
	return true;
}*/

// bool PasswordSettings::reset()
// {
// 	return init();
// }

