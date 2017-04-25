/*
 *  Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "sscview.h"
#include "sscconfig.h"
#include "modifyuser.h"
#include "manageinstitutions.h"
#include "manageusers.h"
#include "trainingswizard.h"
#include "uploadsamples.h"

#include <sscdaccess/sscdaccess.h>

#include <simonprogresstracking/statusmanager.h>
#include <simonprogresstracking/compositeprogresswidget.h>

#include <sscobjects/user.h>
#include <sscobjects/userininstitution.h>

#include <QStringList>
#include <QPointer>

#include <KCMUtils/KCMultiDialog>
#include <QIcon>
#include <KLocalizedString>
#include <KStatusBar>
#include <QAction>
#include <KActionCollection>

#include <sscdaccess/sscdaccesssingleton.h>
#include <KMessageBox>

SSCView::SSCView(QWidget* parent) : KXmlGuiWindow(parent),
wantToDisconnect(true)
{
  //apply configured timeout
  SSCDAccessSingleton::getInstance()->setTimeout(SSCConfig::timeout());
  
  ui.setupUi ( this );
  setupActions();

  if (SSCConfig::autoConnect()) {
    connectToServer();
  }

  disconnected();

  connect(SSCDAccessSingleton::getInstance(), SIGNAL(connected()), this, SLOT(connected()));
  connect(SSCDAccessSingleton::getInstance(), SIGNAL(disconnected()), this, SLOT(disconnected()));
  connect(SSCDAccessSingleton::getInstance(), SIGNAL(error(QString)), this, SLOT(displayError(QString)));
  connect(SSCDAccessSingleton::getInstance(), SIGNAL(warning(QString)), this, SLOT(displayWarning(QString)));

  connect(ui.cbPatientId, SIGNAL(returnPressed()), this, SLOT(getUser()));
  connect(ui.cbPatientId, SIGNAL(currentIndexChanged(int)), this, SLOT(getUser()));
  connect(ui.pbSelectPatient, SIGNAL(clicked()), this, SLOT(getUser()));
  connect(ui.pbSearchPatient, SIGNAL(clicked()), this, SLOT(findUser()));
  connect(ui.pbDetails, SIGNAL(clicked()), this, SLOT(userDetails()));
  connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(deleteUser()));

  connect(ui.pbRepeat, SIGNAL(clicked()), this, SLOT(repeat()));
  connect(ui.pbTraining, SIGNAL(clicked()), this, SLOT(training()));
  connect(ui.pbInterview, SIGNAL(clicked()), this, SLOT(interview()));
}


User* SSCView::retrieveUser()
{
  User *u = 0;

  if (offlineMode()) {
    bool ok = true;
    qint32 userId = ui.cbPatientId->currentText().toInt(&ok);
    if (!ok) return 0;

    u = new User(userId, "(Not available in offline mode)", "", '?', 0, 0, "", "", "", "", "", 0, 0, 0, 0, 1, 1);
  }
  else {
    SSCConfig::self()->readConfig();

    if (SSCConfig::useInstitutionSpecificIDs()) {
      u = getInstituteSpecificUser();
    }
    else {
      bool ok;
      qint32 id = ui.cbPatientId->currentText().toInt(&ok);
      if (!ok) {
        KMessageBox::information(this, i18n("Please enter a valid user ID."));
        return 0;
      }
      u = SSCDAccessSingleton::getInstance()->getUser(id);
    }
  }

  if (!u) {
    KMessageBox::sorry(this, i18nc("%1 is error message", "Could not retrieve user: %1", SSCDAccessSingleton::getInstance()->lastError()));
    return 0;
  }
  return u;
}


/*
 * Only call this if the user set to use institute specific ids; Otherwise
 * the behaviour is undefined
 */
User* SSCView::getInstituteSpecificUser()
{
  if (ui.cbPatientId->currentText().isEmpty()) return 0;

  bool ok;
  QList<User*> users = SSCDAccessSingleton::getInstance()->getUsers(new User(0, "", "", ' ', 0, "",
    "", "", "", "", "", -1, -1, -1, "", 2, 2) /* dummy user */,
    SSCConfig::referenceInstitute(),
    ui.cbPatientId->currentText(), &ok);

  if (ok && (users.count() == 1))
    return users[0];
  else qDeleteAll(users);
  return 0;
}


void SSCView::displayUser(User* u)
{
  ui.lbPatientName->setText(u->surname()+", "+u->givenName());
  ui.pbRepeat->setEnabled(u->repeatingPossible()==1);
  ui.pbTraining->setEnabled(u->repeatingPossible()==1);
  ui.pbInterview->setEnabled(u->interviewPossible()==1);
}


/*
 * Fetches user information from sscd; Fired after the user enters the id in the user input field
 */
void SSCView::getUser()
{
  User *u = retrieveUser();
  if (!u) return;

  displayUser(u);
  delete u;
}


void SSCView::findUser()
{
  ManageUsers *manageUsers = new ManageUsers(this);
  User *u = manageUsers->getUser();
  if (u) {
    displayUser(u);

    SSCConfig::self()->readConfig();
    if (SSCConfig::useInstitutionSpecificIDs()) {
      bool ok, found=false;
      QList<UserInInstitution*> uiis = SSCDAccessSingleton::getInstance()->getUserInInstitutions(u->userId(), &ok);
      if (ok) {
        foreach (UserInInstitution *uii, uiis) {
          if (uii->institutionId() == SSCConfig::referenceInstitute()) {
            ui.cbPatientId->setEditText(uii->referenceId());
            found = true;
            break;
          }
        }
      }

      if (!found) {
        KMessageBox::sorry(this, i18n("Could not resolve institution reference ID of the given user.\n\nMaybe they are not on your institution? You might want to disable institution specific IDs in the configuration."));
      }
    } else
    ui.cbPatientId->setEditText(QString::number(u->userId()));
  }
  manageUsers->deleteLater();
}


/**
 * Sets up the gui actions using the KXmlGuiAction workflow
 */
void SSCView::setupActions()
{
  QAction * connectAction = new QAction(this);
  connectAction->setText(i18n("Connect"));
  connectAction->setIcon(QIcon::fromTheme("network-connect"));
  connectAction->setStatusTip(i18n("Connect to the SSCd server"));
  actionCollection()->addAction("connect", connectAction);
  connect(connectAction, SIGNAL(triggered(bool)),
    this, SLOT(connectToServer()));

  QAction * disconnectAction = new QAction(this);
  disconnectAction->setText(i18n("Disconnect"));
  disconnectAction->setIcon(QIcon::fromTheme("network-disconnect"));
  disconnectAction->setStatusTip(i18n("Disconnect from the SSCd server"));
  actionCollection()->addAction("disconnect", disconnectAction);
  connect(disconnectAction, SIGNAL(triggered(bool)),
    this, SLOT(disconnectFromServer()));

  QAction * offlineAction = new QAction(this);
  offlineAction->setText(i18n("Offline mode"));
  offlineAction->setIcon(QIcon::fromTheme("document-open-remote"));
  offlineAction->setStatusTip(i18n("Switch to offline mode"));
  actionCollection()->addAction("offline", offlineAction);
  offlineAction->setCheckable(true);
  offlineAction->setChecked(false);
  connect(offlineAction, SIGNAL(triggered(bool)),
    this, SLOT(toggleOfflineMode(bool)));

  QAction * addUserAction = new QAction(this);
  addUserAction->setText(i18n("Add user"));
  addUserAction->setIcon(QIcon::fromTheme("list-add-user"));
  addUserAction->setStatusTip(i18n("Add a new user"));
  actionCollection()->addAction("adduser", addUserAction);
  connect(addUserAction, SIGNAL(triggered(bool)),
    this, SLOT(addUser()));

  QAction * usersAction = new QAction(this);
  usersAction->setText(i18n("Manage users"));
  usersAction->setStatusTip(i18n("Show, edit and add users"));
  usersAction->setIcon(QIcon::fromTheme("user-properties"));
  actionCollection()->addAction("users", usersAction);
  connect(usersAction, SIGNAL(triggered(bool)),
    this, SLOT(listUsers()));

  QAction * institutionsAction = new QAction(this);
  institutionsAction->setText(i18n("Manage institutions"));
  institutionsAction->setIcon(QIcon::fromTheme("user-group-properties"));
  institutionsAction->setStatusTip(i18n("Show, edit and add institutions"));
  actionCollection()->addAction("institutions", institutionsAction);
  connect(institutionsAction, SIGNAL(triggered(bool)),
    this, SLOT(listInstitutions()));

  QAction * storeAction = new QAction(this);
  storeAction->setText(i18n("Upload stored data"));
  storeAction->setIcon(QIcon::fromTheme("folder-sync"));
  storeAction->setStatusTip(i18n("Browse, edit, delete and upload stored sample collections"));
  actionCollection()->addAction("store", storeAction);
  connect(storeAction, SIGNAL(triggered(bool)),
    this, SLOT(uploadStored()));

  KStandardAction::quit(this, SLOT(close()), actionCollection());

  actionCollection()->addAction(KStandardAction::Preferences, "configuration",
    this, SLOT(showConfigurationDialog()));

  setupGUI();
  statusBar()->showMessage(i18nc("Disconnected from server", "Disconnected"));
  // statusBar()->insertItem("",1,10);  //QT5TODO: What was this? 
  statusBar()->insertPermanentWidget(2,StatusManager::global(this)->createWidget(this));
}


void SSCView::toggleOfflineMode(bool offline)
{
  if (offline) {
    if (SSCDAccessSingleton::getInstance()->isConnected()) {
      if (KMessageBox::questionYesNoCancel(this, i18n("You are currently connected. If you switch to offline mode you will first be disconnected from the server.\n\nDo you want to continue?")) != KMessageBox::Yes) {
        actionCollection()->action("offline")->setChecked(false);
        return;
      }

      disconnectFromServer();
    }

    KMessageBox::information(this, i18n("If you continue you will get access to the sample training despite not being connected to the server.\n\nBecause of this, ssc cannot make sure that the specified user exists. If it does not you will run into trouble later.\n\nOnly use this function if you really know what you are doing! It is also not possible to use institution specific ids in offline mode."), QString(), "AskOfflineMode");

    ui.cbPatientId->setEnabled(true);
    ui.pbSelectPatient->setEnabled(true);
    displayConnectionStatus(i18n("Not connected (Offline mode)"));
    qDebug() << "set to offline mode";
  }
  else {
    ui.cbPatientId->setEnabled(false);
    ui.pbInterview->setEnabled(false);
    ui.pbRepeat->setEnabled(false);
    ui.pbTraining->setEnabled(false);
    displayConnectionStatus(i18n("Not connected"));
  }
  clearUserSelection();
}


bool SSCView::offlineMode()
{
  return actionCollection()->action("offline")->isChecked();
}


/**
 * Creates and displays the configuration dialog;
 */
void SSCView::showConfigurationDialog()
{
  QPointer<KCMultiDialog> configDialog = new KCMultiDialog(this);
  configDialog->addModule("sscconfiguration", QStringList() << "");
  configDialog->addModule("simonsoundconfig", QStringList() << "");
  configDialog->exec();
  delete configDialog;
}


void SSCView::listUsers()
{
  QPointer<ManageUsers> manageUsers = new ManageUsers(this);
  manageUsers->exec();
  delete manageUsers;
}


void SSCView::listInstitutions()
{
  QPointer<ManageInstitutions> manageInstitutions = new ManageInstitutions(this);
  manageInstitutions->exec();
  delete manageInstitutions;
}


void SSCView::addUser()
{
  QPointer<ModifyUser> m = new ModifyUser(this);
  m->newUser();
  delete m;
}


void SSCView::deleteUser()
{
  User *u = retrieveUser();
  if (!u) return;

  if (KMessageBox::questionYesNo(this, i18nc("%1 is user id, %2 is user name", 
    "Do you really want to delete the user \"%1\" (\"%2\")?\n\nAll collected samples associated with this user will be irreversibly destroyed.",
    u->userId(), QString("%1, %2").arg(u->surname()).arg(u->givenName()))) ==
  KMessageBox::Yes) {
    if (!SSCDAccessSingleton::getInstance()->deleteUser(u))
      KMessageBox::sorry(this, i18nc("%1 is error message", "Could not delete user: %1", SSCDAccessSingleton::getInstance()->lastError()));
    else {
      ui.lbPatientName->clear();
      ui.cbPatientId->clear();
    }
  }
  delete u;
}


void SSCView::userDetails()
{
  User *u = retrieveUser();
  if (!u) return;

  QPointer<ModifyUser> m = new ModifyUser(this);
  if (m->modifyUser(u))
    getUser();

  delete m;
  delete u;
}


/**
 * Connects to the server; Does nothing (but displays info message)
 * if already connected
 */
void SSCView::connectToServer()
{
  actionCollection()->action("offline")->setChecked(false);

  if (SSCDAccessSingleton::getInstance()->isConnected()) {
    KMessageBox::information(this, i18n("You are already connected."));
    return;
  }

  displayConnectionStatus(i18nc("Connecting to server", "Connecting..."));

  SSCConfig::self()->readConfig();
  SSCDAccessSingleton::getInstance()->connectTo(SSCConfig::host(), SSCConfig::port(), SSCConfig::useEncryption());
}


/**
 * The sscdaccess reported that it connected to the server
 */
void SSCView::connected()
{
  actionCollection()->action("offline")->setChecked(false);
	  displayConnectionStatus(i18nc("Connected to server", "Connected"));

  ui.pbSelectPatient->setEnabled(true);
  ui.pbSearchPatient->setEnabled(true);
  ui.pbDetails->setEnabled(true);
  ui.pbDelete->setEnabled(true);

  ui.pbRepeat->setEnabled(false);
  ui.pbTraining->setEnabled(false);
  ui.pbInterview->setEnabled(false);

  actionCollection()->action("adduser")->setEnabled(true);
  actionCollection()->action("users")->setEnabled(true);
  actionCollection()->action("institutions")->setEnabled(true);

  clearUserSelection();
  ui.cbPatientId->setEnabled(true);
}


void SSCView::clearUserSelection()
{
  ui.lbPatientName->setText("");
  ui.cbPatientId->clearEditText();
}


/**
 * The sscdaccess reported that it disconnected from the server
 */
void SSCView::disconnected()
{
  displayConnectionStatus(i18n("Not connected"));
  qDebug() << "set to disconnected mode";

  ui.pbSelectPatient->setEnabled(false);
  ui.pbSearchPatient->setEnabled(false);
  ui.pbDetails->setEnabled(false);
  ui.pbDelete->setEnabled(false);

  ui.pbRepeat->setEnabled(false);
  ui.pbTraining->setEnabled(false);
  ui.pbInterview->setEnabled(false);

  actionCollection()->action("adduser")->setEnabled(false);
  actionCollection()->action("users")->setEnabled(false);
  actionCollection()->action("institutions")->setEnabled(false);

  ui.cbPatientId->setEnabled(false);

  clearUserSelection();

  if (!wantToDisconnect) {
    //server quit
    if (KMessageBox::questionYesNo(this,
      i18n("The server closed the connection.\n\nDo you want to try to re-connect?")) ==
    KMessageBox::Yes) {
      connectToServer();
    }
  }
  wantToDisconnect = false;
}


/**
 * Displays a generic message box; To keep dependencies slim on supporting libraries
 */
void SSCView::displayError(const QString& error)
{
  KMessageBox::sorry(this, error);
}


/**
 * Displays a generic message box; To keep dependencies slim on supporting libraries
 */
void SSCView::displayWarning(const QString& warning)
{
  KMessageBox::information(this, warning);
}


/**
 * Disconnects from the server; Does nothing (but displays info message)
 * if not connected
 */
void SSCView::disconnectFromServer()
{
  if (!SSCDAccessSingleton::getInstance()->isConnected()) {
    KMessageBox::information(this, i18n("You are not connected."));
    return;
  }

  wantToDisconnect = true;

  displayConnectionStatus(i18n("Disconnecting..."));
  SSCDAccessSingleton::getInstance()->disconnectFromServer();
}


/**
 * Displays the given connections status in the status bar
 */
void SSCView::displayConnectionStatus(const QString &status)
{
  statusBar()->showMessage(status, 0);
}


void SSCView::repeat()
{
  User *u = retrieveUser();
  if (!u) return;
  u->interviewPossible();

  if (u->repeatingPossible()) {
    QPointer<TrainingsWizard> trainingsWizard = new TrainingsWizard(this);
    trainingsWizard->collectSamples(Sample::Repeating, u->userId());
    delete trainingsWizard;
  }
  delete u;
}


void SSCView::training()
{
  User *u = retrieveUser();
  if (!u) return;
  u->interviewPossible();

  if (u->repeatingPossible()) {
    QPointer<TrainingsWizard> trainingsWizard = new TrainingsWizard(this);
    trainingsWizard->collectSamples(Sample::Training, u->userId());
    delete trainingsWizard;
  }
  delete u;
}


void SSCView::interview()
{
  User *u = retrieveUser();
  if (!u) return;
  u->interviewPossible();

  if (u->interviewPossible()) {
    QPointer<TrainingsWizard> trainingsWizard = new TrainingsWizard(this);
    trainingsWizard->collectSamples(Sample::Interview, u->userId());
    delete trainingsWizard;
  }
  delete u;
}


void SSCView::uploadStored()
{
  QPointer<UploadSamples> samples = new UploadSamples(this);
  samples->exec();
  delete samples;
}


SSCView::~SSCView()
{
}
