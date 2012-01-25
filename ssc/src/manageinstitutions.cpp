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

#include <sscdaccess/sscdaccess.h>
#include <sscobjects/institution.h>
#include <QSortFilterProxyModel>
#include <KInputDialog>
#include <KDebug>
#include <KMessageBox>
#include <KDialogButtonBox>
#include <sscdaccess/sscdaccesssingleton.h>

ManageInstitutions::ManageInstitutions(QWidget* parent) :
KDialog(parent), model(0), proxyModel(new QSortFilterProxyModel(this))
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

  connect(ui.leInstitutionName, SIGNAL(returnPressed(QString)), proxyModel, SLOT(setFilterRegExp(QString)));
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
  QString name = KInputDialog::getText(i18n("New Institution"), i18n("Institution name:"));
  if (name.isEmpty()) return;

  if (!SSCDAccessSingleton::getInstance()->addInstitution(new Institution(0, name)))
    KMessageBox::sorry(this, i18nc("%1 is error message", "Could not add institution: %1", SSCDAccessSingleton::getInstance()->lastError()));

  updateList();
}


Institution* ManageInstitutions::getCurrentlySelectedInstitution()
{
  QModelIndex selectedIndex = proxyModel->mapToSource(ui.tvInstitutions->currentIndex());
  if (!selectedIndex.isValid()) {
    KMessageBox::information(this, i18n("Please select an institution"));
    return 0;
  }

  return static_cast<Institution*>(selectedIndex.internalPointer());
}


void ManageInstitutions::editInstitution()
{
  Institution *i = getCurrentlySelectedInstitution();
  if (!i) return;

  QString name = KInputDialog::getText(i18nc("%1 is institution id", "Modify \"%1\"", i->id()),
    i18nc("%1 is institution id", "New institution name for institution \"%1\":", i->id()),
    i->name());

  if (name.isEmpty()) return;

  i->setName(name);
  if (!SSCDAccessSingleton::getInstance()->modifyInstitution(i)) {
    KMessageBox::sorry(this, i18nc("%1 is error message", "Could not modify institution: %1", SSCDAccessSingleton::getInstance()->lastError()));
    updateList();
  }
}


void ManageInstitutions::deleteInstitution()
{
  if ((KMessageBox::questionYesNo(this, i18n("Do you really want to delete the institution? All user associations to this institution will also be deleted.")) != KMessageBox::Yes))
    return;

  Institution *i = getCurrentlySelectedInstitution();
  if (!i) return;

  if (!SSCDAccessSingleton::getInstance()->deleteInstitution(i))
    KMessageBox::sorry(this, i18nc("%1 is error message", "Could not delete institution: %1", SSCDAccessSingleton::getInstance()->lastError()));

  updateList();
}


/**
 * Fetches the list of institutions and displays them
 */
void ManageInstitutions::updateList()
{
  bool ok;
  QList<Institution*> institutions = SSCDAccessSingleton::getInstance()->getInstitutions(&ok);

  if (!ok) {
    KMessageBox::sorry(this, i18nc("%1 is error message", "Could not retrieve institutions: %1", SSCDAccessSingleton::getInstance()->lastError()));
    return;
  }

  if (!model) {
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
  return 0;
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
