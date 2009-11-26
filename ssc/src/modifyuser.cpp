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
#include "modifyuserininstitution.h"
#include "sscconfig.h"

#include <sscdaccess/sscdaccess.h>
#include <sscobjects/user.h>
#include <sscobjects/userininstitution.h>
#include <sscobjects/language.h>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <KDebug>
#include <KMessageBox>
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

	connect(ui.pbAddInstitution, SIGNAL(clicked()), this, SLOT(addInstitutionAssociation()));
	connect(ui.pbRemoveInstitution, SIGNAL(clicked()), this, SLOT(removeInstitutionAssociation()));
	
	checkIfComplete();
	setCaption( i18n("User management") );

	ui.twInstitutions->verticalHeader()->hide();
	ui.twInstitutions->setSelectionBehavior(QAbstractItemView::SelectRows);
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

void ModifyUser::addInstitutionAssociation()
{
	ModifyUserInInstitution *modifyUserInInstitution = new ModifyUserInInstitution(this);
	UserInInstitution *uiiN = modifyUserInInstitution->add();

	foreach (UserInInstitution* uii, uiisAdd) {
		//if already in here delete the new one
		if ((uii->institutionId() == uiiN->institutionId()) && (uii->referenceId() == uiiN->referenceId())) {
			delete uiiN;
			return; // can't also be in uiisDelete; we checked when we addded it to uiisToAdd...
		}
	}


	bool dontAdd = false;
	foreach (UserInInstitution* uii, uiisDelete) {
		//if in here delete the new one and simply remove the delete command
		if ((uii->institutionId() == uiiN->institutionId()) && (uii->referenceId() == uiiN->referenceId())) {
			dontAdd = true;
			uiisDelete.removeAll(uii);
			delete uii;
		}
	}

	if (!dontAdd)
		uiisAdd << uiiN;

	int index = ui.twInstitutions->rowCount();

	ui.twInstitutions->setRowCount(index+1);
	QTableWidgetItem *institutionItem = new QTableWidgetItem(QString::number(uiiN->institutionId()));
	institutionItem->setData(Qt::UserRole, QVariant::fromValue(uiiN));
	
	ui.twInstitutions->setItem(index, 0, institutionItem);
	ui.twInstitutions->setItem(index, 1, new QTableWidgetItem(uiiN->referenceId()));

	if (dontAdd)
		delete uiiN;
}

void ModifyUser::removeInstitutionAssociation()
{
	QTableWidgetItem *selectedItem = ui.twInstitutions->item(ui.twInstitutions->currentRow(), 0);
	if (!selectedItem) return;

	if (!KMessageBox::questionYesNo(this, i18n("Do you really want to remove this user - institution association?")) == KMessageBox::Yes)
		return;

	UserInInstitution *uiiN = selectedItem->data(Qt::UserRole).value<UserInInstitution*>();

	if (uiisAdd.removeAll(uiiN) == 0)
		uiisDelete << uiiN;
	else kDebug() << "Removed at least one uii to add";

	//move up each item after this item
	int thisRow = selectedItem->row();

	delete ui.twInstitutions->takeItem(thisRow, 0);
	delete ui.twInstitutions->takeItem(thisRow, 1);

	for (int i=thisRow; i < ui.twInstitutions->rowCount()-1; i++)
		for (int j=0; j < 2; j++)
			ui.twInstitutions->setItem(i, j, ui.twInstitutions->takeItem(i+1, j));

	ui.twInstitutions->setRowCount(ui.twInstitutions->rowCount()-1);
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

	ui.cbMothersTongue->setCurrentIndex(ui.cbMothersTongue->findData(SSCConfig::defaultLanguage()));
}

void ModifyUser::displayCurrentInstitutionAssociation(qint32 userId)
{
	bool ok;
	QList<UserInInstitution*> uiis = SSCDAccess::getInstance()->getUserInInstitutions(userId, &ok);
	if (!ok) {
		KMessageBox::sorry(this, i18n("Couldn't retrieve user - institution associations: %1", SSCDAccess::getInstance()->lastError()));
		return;
	}

	ui.twInstitutions->setRowCount(uiis.count());
	for (int i=0; i < uiis.count(); i++) {
		QTableWidgetItem *institutionItem = new QTableWidgetItem(QString::number((uiis[i])->institutionId()));
		institutionItem->setData(Qt::UserRole, QVariant::fromValue(uiis[i]));

		ui.twInstitutions->setItem(i, 0, institutionItem);
		ui.twInstitutions->setItem(i, 1, new QTableWidgetItem((uiis[i])->referenceId()));
	}
}

int ModifyUser::newUser()
{
	displayLanguages();

	int ret = KDialog::exec();
	if (ret) {
		bool succ = false;
		//creating
		User *newUser = createUser();
		int userId = SSCDAccess::getInstance()->addUser(newUser);
		if (!userId) {
			KMessageBox::sorry(this, i18n("Couldn't add user: %1", SSCDAccess::getInstance()->lastError()));
			succ = false;
		}

		if (succ)
			commitUserInInstitutions(userId);
				
		delete newUser;

		if (!succ) ret = 0;
	}
	return ret;
}

int ModifyUser::modifyUser(User *u)
{
	displayLanguages();
	displayUser(u);
	displayCurrentInstitutionAssociation(u->userId());

	int ret = KDialog::exec();
	if (ret) {
		//creating
		bool succ = true;
		User *newUser = createUser(u->userId());
		if (!SSCDAccess::getInstance()->modifyUser(newUser)) {
			KMessageBox::sorry(this, i18n("Couldn't modify user \"%1\": %2", u->userId(), SSCDAccess::getInstance()->lastError()));
			succ = false;
		}

		if (succ)
			commitUserInInstitutions(u->userId());

		delete newUser;

		if (!succ) ret = 0;
	}
	return ret;
}

void ModifyUser::commitUserInInstitutions(qint32 userId)
{
	foreach (UserInInstitution* uii, uiisDelete) {
		uii->setUserId(userId);
		if (!SSCDAccess::getInstance()->deleteUserInInstitution(uii)) {
			KMessageBox::sorry(this, i18n("Couldn't delete user - institution association: %1", SSCDAccess::getInstance()->lastError()));
		}
	}
	foreach (UserInInstitution* uii, uiisAdd) {
		uii->setUserId(userId);
		if (!SSCDAccess::getInstance()->addUserInInstitution(uii)) {
			KMessageBox::sorry(this, i18n("Couldn't add user - institution association: %1", SSCDAccess::getInstance()->lastError()));
		}
	}

	qDeleteAll(uiisAdd);
	qDeleteAll(uiisDelete);
	qDeleteAll(uiisCurrent);
	uiisAdd.clear();
	uiisDelete.clear();
	uiisCurrent.clear();
}

ModifyUser::~ModifyUser()
{
}

