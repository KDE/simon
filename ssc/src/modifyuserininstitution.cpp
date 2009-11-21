/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "modifyuserininstitution.h"
#include "manageinstitutions.h"

#include <sscdaccess/sscdaccess.h>
#include <sscobjects/user.h>
#include <sscobjects/institution.h>
#include <sscobjects/userininstitution.h>
#include <KDebug>
#include <KMessageBox>
#include <KDialogButtonBox>

ModifyUserInInstitution::ModifyUserInInstitution(QWidget* parent) : KDialog(parent)
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);

	setMainWidget( widget );
	
	connect(ui.cbInstitution, SIGNAL(editTextChanged(QString)), this, SLOT(checkIfComplete()));
	connect(ui.leReferenceId, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));

	connect(ui.pbSelectInstitution, SIGNAL(clicked()), this, SLOT(findInstitution()));
	
	checkIfComplete();
	setCaption( i18n("User - Institution") );
}

void ModifyUserInInstitution::checkIfComplete()
{
	bool complete = true;
	complete = complete && !ui.leReferenceId->text().isEmpty();
	complete = complete && !ui.cbInstitution->currentText().isEmpty();

	enableButtonOk(complete);
}

void ModifyUserInInstitution::deleteLater()
{
	QObject::deleteLater();
}

void ModifyUserInInstitution::findInstitution()
{
	ManageInstitutions *manageInstitutions = new ManageInstitutions(this);
	Institution *i = manageInstitutions->getInstitution();
	if (i)
		ui.cbInstitution->setEditText(QString::number(i->id()));

	manageInstitutions->deleteLater();
}

UserInInstitution* ModifyUserInInstitution::add()
{
	int ret = KDialog::exec();
	if (ret) {
		bool ok;
		int institutionId = ui.cbInstitution->currentText().toInt(&ok);
		if (!ok) {
			KMessageBox::sorry(this, i18n("Please enter a valid institution id"));
			return NULL;
		}

		return new UserInInstitution(0, institutionId, ui.leReferenceId->text());
	}
	return NULL;
}


ModifyUserInInstitution::~ModifyUserInInstitution()
{
}


