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

#include "modifyuser.h"

#include <sscdaccess/sscdaccess.h>
#include <sscobjects/user.h>
#include <sscobjects/language.h>
#include <KUrl>
#include <KDebug>
#include <KMessageBox>
#include <KKeySequenceWidget>
#include <KDialogButtonBox>

ModifyUser::ModifyUser(QWidget* parent) : KDialog(parent)
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);

	setMainWidget( widget );
	
	connect(ui.leSurname, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
	connect(ui.leGivenName, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
	connect(ui.rbMale, SIGNAL(toggled(bool)), this, SLOT(checkIfComplete()));
	connect(ui.rbFemale, SIGNAL(toggled(bool)), this, SLOT(checkIfComplete()));
	connect(ui.leBirthYear, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
	connect(ui.leZIPCode, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
	connect(ui.teEducation, SIGNAL(textChanged()), this, SLOT(checkIfComplete()));
	connect(ui.leCurrentOccupation, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
	
	checkIfComplete();
	setCaption( i18n("") );
}

void ModifyUser::checkIfComplete()
{
	bool complete = true;
	complete = complete && !ui.leSurname->text().isEmpty();
	complete = complete && !ui.leGivenName->text().isEmpty();
	complete = complete && (ui.rbMale->isChecked() || ui.rbFemale->isChecked());
	complete = complete && !ui.leBirthYear->text().isEmpty();
	complete = complete && !ui.leZIPCode->text().isEmpty();
	complete = complete && !ui.teEducation->toPlainText().isEmpty();
	complete = complete && !ui.leCurrentOccupation->text().isEmpty();

	enableButtonOk(complete);
}

void ModifyUser::deleteLater()
{
	QObject::deleteLater();
}

User* ModifyUser::createUser(qint32 userId)
{
	kDebug() << "Language: " << ui.cbMothersTongue->itemData(ui.cbMothersTongue->currentIndex(), Qt::UserRole).toString();
	User *u = new User(userId,
			ui.leSurname->text(),
			ui.leGivenName->text(),
			ui.rbMale->isChecked() ? 'M' : 'F',
			ui.leBirthYear->text().toInt(),
			ui.leZIPCode->text(),
			ui.teEducation->toPlainText(),
			ui.leCurrentOccupation->text(),
			ui.cbMothersTongue->itemData(ui.cbMothersTongue->currentIndex(), Qt::UserRole).toString(),
			ui.cbMothersTongue->currentText(),
			ui.teDiagnosis->toPlainText(),
			ui.cbOrientation->currentIndex()+1,
			ui.cbMotorFunction->currentIndex()+1,
			ui.cbCommunication->currentIndex()+1,
			ui.teMouthMotoric->toPlainText(),
			ui.cbInterviewPossible->isChecked(),
			ui.cbRepeatingPossible->isChecked());
	return u;
}

void ModifyUser::displayUser(User *u)
{
	ui.leSurname->setText(u->surname());
	ui.leGivenName->setText(u->givenName());
	if (u->sex() == 'M')
		ui.rbMale->setChecked(true);
	else ui.rbFemale->setChecked(true);

	ui.leBirthYear->setText(QString::number(u->birthYear()));
	ui.leZIPCode->setText(u->zipCode());
	ui.teEducation->setPlainText(u->education());
	ui.leCurrentOccupation->setText(u->currentOccupation());
	ui.cbMothersTongue->setCurrentIndex(ui.cbMothersTongue->findData(u->motherTongueId()));
	ui.teDiagnosis->setPlainText(u->diagnosis());
	ui.cbOrientation->setCurrentIndex(u->orientation()-1);
	ui.cbMotorFunction->setCurrentIndex(u->motorFunction()-1);
	ui.cbCommunication->setCurrentIndex(u->communication()-1);
	ui.teMouthMotoric->setPlainText(u->mouthMotoric());
	ui.cbInterviewPossible->setChecked(u->interviewPossible());
	ui.cbRepeatingPossible->setChecked(u->repeatingPossible());
}

void ModifyUser::displayLanguages()
{
	ui.cbMothersTongue->clear();

	bool ok;
	QList<Language*> langs = SSCDAccess::getInstance()->getLanguages(&ok);
	if (!ok) {
		KMessageBox::sorry(this, i18n("Couldn't retrieve languages: %1", SSCDAccess::getInstance()->lastError()));
		return;
	}

	foreach (Language *l, langs) {
		ui.cbMothersTongue->addItem(l->name(), l->id());
	}

	//TODO: make default configurable
	//default to "de" language
	ui.cbMothersTongue->setCurrentIndex(ui.cbMothersTongue->findData("de"));
}

int ModifyUser::newUser()
{
	displayLanguages();

	int ret = KDialog::exec();
	if (ret) {
		//creating
		User *newUser = createUser();
		if (!SSCDAccess::getInstance()->addUser(newUser)) {
			KMessageBox::sorry(this, i18n("Couldn't add user: %1", SSCDAccess::getInstance()->lastError()));
			return false;
		}
		delete newUser;
	}
	return ret;
}

int ModifyUser::modifyUser(User *u)
{
	displayLanguages();
	displayUser(u);

	int ret = KDialog::exec();
	if (ret) {
		//creating
		User *newUser = createUser(u->userId());
		if (!SSCDAccess::getInstance()->modifyUser(newUser)) {
			KMessageBox::sorry(this, i18n("Couldn't modify user \"%1\": %2", u->userId(), SSCDAccess::getInstance()->lastError()));
			return false;
		}
		delete newUser;	
	}
	return ret;
}

ModifyUser::~ModifyUser()
{
}

