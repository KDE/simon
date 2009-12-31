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

#include "manageinstitutions.h"
#include "institutionmodel.h"

#include "sscdaccess.h"
#include <sscobjects/institution.h>
#include <QSortFilterProxyModel>
#include <QInputDialog>
#include <KDebug>
#include <KMessageBox>
#include <KDialogButtonBox>

ManageInstitutions::ManageInstitutions(QWidget* parent) : 
	KDialog(parent), model(NULL), proxyModel(new QSortFilterProxyModel(this))
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);

	ui.tvInstitutions->setModel(proxyModel);
	proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	proxyModel->setFilterKeyColumn(1);

	setMainWidget( widget );
	
//	connect(ui.leSurname, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));

	connect(ui.pbAdd, SIGNAL(clicked()), this, SLOT(addInstitution()));
	connect(ui.pbEdit, SIGNAL(clicked()), this, SLOT(editInstitution()));
	connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(deleteInstitution()));

	connect(ui.leInstitutionName, SIGNAL(returnPressed(const QString&)), proxyModel, SLOT(setFilterRegExp(const QString&)));
	connect(ui.pbFilter, SIGNAL(clicked()), this, SLOT(filter()));
	setCaption( i18n("Institutions") );

	ui.tvInstitutions->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tvInstitutions->setSortingEnabled(true);

	//set return button to filter instead of closing the dialog
	ui.pbFilter->setDefault(true);
}


void ManageInstitutions::filter()
{
	proxyModel->setFilterRegExp(ui.leInstitutionName->text());
}


void ManageInstitutions::addInstitution()
{
	QString name = QInputDialog::getText(this, i18n("New Institution"), i18n("Institution name:"), 
					QLineEdit::Normal);
	if (name.isEmpty()) return;

	if (!SSCDAccess::getInstance()->addInstitution(new Institution(0, name)))
		KMessageBox::sorry(this, i18n("Could not add institution: %1", SSCDAccess::getInstance()->lastError()));

	updateList();
}

Institution* ManageInstitutions::getCurrentlySelectedInstitution()
{
	QModelIndex selectedIndex = proxyModel->mapToSource(ui.tvInstitutions->currentIndex());
	if (!selectedIndex.isValid()) {
		KMessageBox::information(this, i18n("Please select an institution"));
		return NULL;
	}

	return static_cast<Institution*>(selectedIndex.internalPointer());
}

void ManageInstitutions::editInstitution()
{
	Institution *i = getCurrentlySelectedInstitution();
	if (!i) return;

	QString name = QInputDialog::getText(this, i18n("Modify \"%1\"", i->id()), 
			i18n("New institution name for institution \"%1\":", i->id()), 
					QLineEdit::Normal, i->name());

	if (name.isEmpty()) return;

	i->setName(name);
	if (!SSCDAccess::getInstance()->modifyInstitution(i)) {
		KMessageBox::sorry(this, i18n("Could not modify institution: %1", SSCDAccess::getInstance()->lastError()));
		updateList();
	}
}

void ManageInstitutions::deleteInstitution()
{
	if ((KMessageBox::questionYesNo(this, i18n("Do you really want to delete the institution? All user associations to this institution will also be deleted.")) != KMessageBox::Yes))
		return;

	Institution *i = getCurrentlySelectedInstitution();
	if (!i) return;

	if (!SSCDAccess::getInstance()->deleteInstitution(i))
		KMessageBox::sorry(this, i18n("Could not delete institution: %1", SSCDAccess::getInstance()->lastError()));

	updateList();
}


/**
 * Fetches the list of institutions and displays them
 */
void ManageInstitutions::updateList()
{
	bool ok;
	QList<Institution*> institutions = SSCDAccess::getInstance()->getInstitutions(&ok);

	if (!ok) {
		KMessageBox::sorry(this, i18n("Could not retrieve institutions: %1", SSCDAccess::getInstance()->lastError()));
		return;
	}


	if (!model)  {
		model = new InstitutionModel(institutions, this);
		proxyModel->setSourceModel(model);
	} else 
		model->replaceData(institutions);

	ui.tvInstitutions->resizeColumnsToContents();
}

int ManageInstitutions::exec()
{
	updateList();
	ui.wgModify->show();
	return KDialog::exec();
}

Institution* ManageInstitutions::getInstitution()
{
	updateList();
	ui.wgModify->hide();
	int ret = KDialog::exec();
	if (ret) {
		return getCurrentlySelectedInstitution();
	}
	return NULL;
}

void ManageInstitutions::deleteLater()
{
	QObject::deleteLater();
}

ManageInstitutions::~ManageInstitutions()
{
	model->deleteLater();
	proxyModel->deleteLater();
}

