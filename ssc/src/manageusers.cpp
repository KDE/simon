
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

#include "manageusers.h"
#include "usermodel.h"
#include "modifyuser.h"
#include "manageinstitutions.h"

#include <sscdaccess/sscdaccess.h>
#include <sscobjects/user.h>
#include <sscobjects/institution.h>
#include <QSortFilterProxyModel>
#include <QInputDialog>
#include <KDebug>
#include <KMessageBox>
#include <KDialogButtonBox>
#include <sscdaccess/sscdaccesssingleton.h>

ManageUsers::ManageUsers(QWidget* parent) :
KDialog(parent), model(0), proxyModel(new QSortFilterProxyModel(this))
{
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);

  ui.tvUsers->setModel(proxyModel);

  setMainWidget( widget );

  connect(ui.pbAdd, SIGNAL(clicked()), this, SLOT(addUser()));
  connect(ui.pbEdit, SIGNAL(clicked()), this, SLOT(editUser()));
  connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(deleteUser()));

  connect(ui.pbSearchInstitution, SIGNAL(clicked()), this, SLOT(findInstitution()));

  connect(ui.pbFilter, SIGNAL(clicked()), this, SLOT(filter()));
  setCaption( i18n("Users") );

  ui.tvUsers->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui.tvUsers->setSortingEnabled(true);

  //set return button to filter instead of closing the dialog
  ui.pbFilter->setDefault(true);
  displayLanguages();
}


void ManageUsers::filter()
{
  updateList();
}


void ManageUsers::displayLanguages()
{
  ui.cbMothersTongue->clear();
  ui.cbMothersTongue->addItem(i18n("Ignored"), "");

  bool ok;
  QList<Language*> langs = SSCDAccessSingleton::getInstance()->getLanguages(&ok);
  if (!ok) {
    KMessageBox::sorry(this, i18nc("%1 is error message", "Could not retrieve languages: %1", SSCDAccessSingleton::getInstance()->lastError()));
    return;
  }

  foreach (Language *l, langs) {
    ui.cbMothersTongue->addItem(l->name(), l->id());
  }
  qDeleteAll(langs);
}


void ManageUsers::findInstitution()
{
  ManageInstitutions *manageInstitutions = new ManageInstitutions(this);
  Institution *i = manageInstitutions->getInstitution();
  if (i) {
    ui.cbInstitutionId->setEditText(QString::number(i->id()));
    updateList();
  }

  manageInstitutions->deleteLater();
}


User* ManageUsers::getCurrentlySelectedUser()
{
  QModelIndex selectedIndex = proxyModel->mapToSource(ui.tvUsers->currentIndex());
  if (!selectedIndex.isValid()) {
    KMessageBox::information(this, i18n("Please select an user"));
    return 0;
  }

  return static_cast<User*>(selectedIndex.internalPointer());
}


void ManageUsers::addUser()
{
  ModifyUser *m = new ModifyUser(this);
  m->newUser();
  m->deleteLater();

  updateList();
}


void ManageUsers::editUser()
{
  User *u = getCurrentlySelectedUser();
  if (!u) return;

  ModifyUser *m = new ModifyUser(this);
  if (m->modifyUser(u))
    updateList();
  m->deleteLater();
}


void ManageUsers::deleteUser()
{
  User *u = getCurrentlySelectedUser();
  if (!u) return;

  if (KMessageBox::questionYesNo(this, i18nc("%1 is user id, %2 is name",
    "Do you really want to delete the user \"%1\" (\"%2\")?\n\n"
    "All collected samples associated with this user will be irreversibly destroyed!",
    u->userId(), QString("%1, %2").arg(u->surname()).arg(u->givenName()))) !=
    KMessageBox::Yes)
    return;

  if (!SSCDAccessSingleton::getInstance()->deleteUser(u))
    KMessageBox::sorry(this, i18nc("%1 is error message", "Could not delete user: %1", SSCDAccessSingleton::getInstance()->lastError()));

  updateList();
}


/**
 * Fetches the list of users and displays them
 */
void ManageUsers::updateList()
{
  bool ok;

  qint32 institutionId = ui.cbInstitutionId->currentText().toInt(&ok);
  if (!ok && !ui.cbInstitutionId->currentText().isEmpty()) {
    KMessageBox::information(this, i18n("InstitutionID is invalid"));
    return;
  }
  QString referenceId = ui.leReferenceId->text();

  User *filterUser = new User(ui.leUserId->text().toInt(),
    ui.leSurname->text(),
    ui.leGivenName->text(),
    getSex(),
    ui.leBirthYear->text().toInt(),
    ui.leZIPCode->text(),
    ui.teEducation->toPlainText(),
    ui.leCurrentOccupation->text(),
    ui.cbMothersTongue->itemData(ui.cbMothersTongue->currentIndex(), Qt::UserRole).toString(),
    ui.cbMothersTongue->currentText(),
    ui.teDiagnosis->toPlainText(),
    ui.cbOrientation->currentIndex(),
    ui.cbMotorFunction->currentIndex(),
    ui.cbCommunication->currentIndex(),
    ui.teMouthMotoric->toPlainText(),
    getInterviewingPossible(),
    getRepeatingPossible());

  QList<User*> users = SSCDAccessSingleton::getInstance()->getUsers(filterUser, institutionId, referenceId, &ok);

  delete filterUser;

  if (!ok) {
    KMessageBox::sorry(this, i18nc("%1 is error message", "Could not retrieve users: %1", SSCDAccessSingleton::getInstance()->lastError()));
    return;
  }

  if (!model) {
    model = new UserModel(users, this);
    proxyModel->setSourceModel(model);
  } else
  model->replaceData(users);

  ui.tvUsers->resizeColumnsToContents();
}


int ManageUsers::getInterviewingPossible()
{
  if (ui.rbInterviewIgnored->isChecked()) return 2;
  else if (ui.rbInterviewYes->isChecked()) return 1;
  else return 0;

}


int ManageUsers::getRepeatingPossible()
{
  if (ui.rbRepeatingIgnored->isChecked()) return 2;
  else if (ui.rbRepeatingYes->isChecked()) return 1;
  else return 0;
}


char ManageUsers::getSex()
{
  if (ui.rbIgnored->isChecked()) return ' ';
  else if (ui.rbMale->isChecked()) return 'M';
  else return 'F';
}


int ManageUsers::exec()
{
  updateList();
  ui.wgModify->show();
  return KDialog::exec();
}


User* ManageUsers::getUser()
{
  updateList();
  ui.wgModify->hide();
  int ret = KDialog::exec();
  if (ret) {
    return getCurrentlySelectedUser();
  }
  return 0;
}


void ManageUsers::deleteLater()
{
  QObject::deleteLater();
}


ManageUsers::~ManageUsers()
{
}
