/*
 *  Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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
#include "sscdaccess.h"

#include <simonprogresstracking/statusmanager.h>
#include <simonprogresstracking/compositeprogresswidget.h>

#include <sscobjects/user.h>
#include <sscobjects/userininstitution.h>

#include <QStringList>

#include <KCMultiDialog>
#include <KIcon>
#include <KLocalizedString>
#include <KStatusBar>
#include <KAction>
#include <KActionCollection>
#include <KMessageBox>
#include <KStandardDirs>

SSCView::SSCView(QWidget* parent) : KXmlGuiWindow(parent),
  wantToDisconnect(true)
{
	ui.setupUi ( this );
	setupActions();

	if (SSCConfig::autoConnect()) {
		connectToServer();

	}

	disconnected();

	connect(SSCDAccess::getInstance(), SIGNAL(connected()), this, SLOT(connected()));
	connect(SSCDAccess::getInstance(), SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(SSCDAccess::getInstance(), SIGNAL(error(const QString&)), this, SLOT(displayError(const QString&)));
	connect(SSCDAccess::getInstance(), SIGNAL(warning(const QString&)), this, SLOT(displayWarning(const QString&)));

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
	User *u = NULL;
	SSCConfig::self()->readConfig();
	kDebug() << SSCConfig::useInstitutionSpecificIDs();
	kDebug() << SSCConfig::referenceInstitute();
	if (SSCConfig::useInstitutionSpecificIDs())  {
		u = getInstituteSpecificUser();
	} else {
		bool ok;
		qint32 id = ui.cbPatientId->currentText().toInt(&ok);
		if (!ok) {
			KMessageBox::information(this, i18n("Please enter a valid user id."));
			return NULL;
		}
		u = SSCDAccess::getInstance()->getUser(id);
	}

	if (!u) {
		KMessageBox::sorry(this, i18n("Couldn't retrieve user: %1", SSCDAccess::getInstance()->lastError()));
		return NULL;
	}
	return u;
}

/*
 * Only call this if the user set to use institute specific ids; Otherwise
 * the behaviour is undefined
 */
User* SSCView::getInstituteSpecificUser()
{
	if (ui.cbPatientId->currentText().isEmpty()) return NULL;

	bool ok;
	QList<User*> users = SSCDAccess::getInstance()->getUsers(new User(0, "", "", ' ', 0, "",
				"", "", "", "", "", -1, -1, -1, "", 2, 2) /* dummy user */,
				SSCConfig::referenceInstitute(),
				ui.cbPatientId->currentText(), &ok);

	if (ok && (users.count() == 1)) 
		return users[0];
	else qDeleteAll(users);
	return NULL;
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
			QList<UserInInstitution*> uiis = SSCDAccess::getInstance()->getUserInInstitutions(u->userId(), &ok);
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
				KMessageBox::sorry(this, i18n("Couldn't resolve institution reference id of the given user.\n\nMaybe he is not on your institution? You might want to disable institution specific ids in the configuration."));
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
	KAction* connectAction = new KAction(this);
	connectAction->setText(i18n("Connect"));
	connectAction->setIcon(KIcon("network-connect"));
	actionCollection()->addAction("connect", connectAction);
	connect(connectAction, SIGNAL(triggered(bool)),
				this, SLOT(connectToServer()));

	KAction* disconnectAction = new KAction(this);
	disconnectAction->setText(i18n("Disconnect"));
	disconnectAction->setIcon(KIcon("network-disconnect"));
	actionCollection()->addAction("disconnect", disconnectAction);
	connect(disconnectAction, SIGNAL(triggered(bool)),
				this, SLOT(disconnectFromServer()));

	KAction* addUserAction = new KAction(this);
	addUserAction->setText(i18n("Add user"));
	addUserAction->setIcon(KIcon("list-add-user"));
	actionCollection()->addAction("adduser", addUserAction);
	connect(addUserAction, SIGNAL(triggered(bool)),
				this, SLOT(addUser()));

	KAction* usersAction = new KAction(this);
	usersAction->setText(i18n("Manage users"));
	usersAction->setIcon(KIcon("user-properties"));
	actionCollection()->addAction("users", usersAction);
	connect(usersAction, SIGNAL(triggered(bool)),
				this, SLOT(listUsers()));

	KAction* institutionsAction = new KAction(this);
	institutionsAction->setText(i18n("Manage institutions"));
	institutionsAction->setIcon(KIcon("user-group-properties"));
	actionCollection()->addAction("institutions", institutionsAction);
	connect(institutionsAction, SIGNAL(triggered(bool)),
				this, SLOT(listInstitutions()));


		   
	KStandardAction::quit(this, SLOT(close()), actionCollection());

	actionCollection()->addAction(KStandardAction::Preferences, "configuration",
                               this, SLOT(showConfigurationDialog()));

	setupGUI();
	statusBar()->insertItem(i18n("Disconnected"),0);
	statusBar()->insertItem("",1,10);
	statusBar()->insertPermanentWidget(2,StatusManager::global(this)->createWidget(this));
}

/**
 * Creates and displays the configuration dialog;
 */
void SSCView::showConfigurationDialog()
{
	KCMultiDialog *configDialog = new KCMultiDialog(this);
	configDialog->addModule("sscconfiguration", QStringList() << "");
	configDialog->addModule("simonsoundconfig", QStringList() << "");
	configDialog->exec();
	delete configDialog;
}

void SSCView::listUsers()
{
	ManageUsers *manageUsers = new ManageUsers(this);
	manageUsers->exec();
	manageUsers->deleteLater();
}

void SSCView::listInstitutions()
{
	ManageInstitutions *manageInstitutions = new ManageInstitutions(this);
	manageInstitutions->exec();
	manageInstitutions->deleteLater();
}

void SSCView::addUser()
{
	ModifyUser *m = new ModifyUser(this);
	m->newUser();
	m->deleteLater();
}

void SSCView::deleteUser()
{
	User *u = retrieveUser();
	if (!u) return;

	if (KMessageBox::questionYesNo(this, i18n("Do you really want to delete the user \"%1\" (\"%2\")?\n\nAll collected samples associated with this user will be irreversibly destroyed!", 
					u->userId(), QString("%1, %2").arg(u->surname()).arg(u->givenName()))) ==
					KMessageBox::Yes) {
		if (!SSCDAccess::getInstance()->deleteUser(u))
			KMessageBox::sorry(this, i18n("Could not delete user: %1", SSCDAccess::getInstance()->lastError()));
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

	ModifyUser *m = new ModifyUser(this);
	if (m->modifyUser(u))
		getUser();

	m->deleteLater();
	delete u;
}


/**
 * Connects to the server; Does nothing (but displays info message) 
 * if already connected
 */
void SSCView::connectToServer()
{
	if (SSCDAccess::getInstance()->isConnected()) {
		KMessageBox::information(this, i18n("You are already connected."));
		return;
	}

	displayConnectionStatus(i18n("Connecting..."));

	SSCConfig::self()->readConfig();
	SSCDAccess::getInstance()->connectTo(SSCConfig::host(), SSCConfig::port(), SSCConfig::useEncryption());
}

/**
 * The sscdaccess reported that it connected to the server
 */
void SSCView::connected()
{
	displayConnectionStatus(i18n("Connected"));

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

	ui.cbPatientId->setEnabled(true);
}

/**
 * The sscdaccess reported that it disconnected from the server
 */
void SSCView::disconnected()
{
	displayConnectionStatus(i18n("Not connected"));

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

  if (!wantToDisconnect)
  {
    //server quit
    if (KMessageBox::questionYesNo(this, i18n("The server closed the connection.\n\nDo you want to try to re-connect?")) == 
          KMessageBox::Yes)
    {
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
	if (!SSCDAccess::getInstance()->isConnected()) {
		KMessageBox::information(this, i18n("You are not connected."));
		return;
	}

  wantToDisconnect = true;

	displayConnectionStatus(i18n("Disconnecting..."));
	SSCDAccess::getInstance()->disconnectFromServer();
}

/**
 * Displays the given connections status in the status bar
 */
void SSCView::displayConnectionStatus(const QString &status)
{
	statusBar()->changeItem(status, 0);
}


void SSCView::repeat()
{
	User *u = retrieveUser();
	if (!u) return;
	if (u->repeatingPossible()) {
		TrainingsWizard *trainingsWizard = new TrainingsWizard(this);
		trainingsWizard->collectSamples(TrainingsWizard::Repeating, u->userId());
		trainingsWizard->deleteLater();
	}
	delete u;
}

void SSCView::training()
{
	User *u = retrieveUser();
	if (!u) return;

	if (u->repeatingPossible()) {
		TrainingsWizard *trainingsWizard = new TrainingsWizard(this);
		trainingsWizard->collectSamples(TrainingsWizard::Training, u->userId());
		trainingsWizard->deleteLater();
	}
	delete u;
}

void SSCView::interview()
{
	User *u = retrieveUser();
	if (!u) return;

	if (u->interviewPossible()) {
		TrainingsWizard *trainingsWizard = new TrainingsWizard(this);
		trainingsWizard->collectSamples(TrainingsWizard::Interview, u->userId());
		trainingsWizard->deleteLater();
	}
	delete u;
}


SSCView::~SSCView()
{
}
