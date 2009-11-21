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
#include <sscdaccess/sscdaccess.h>

#include <simonprogresstracking/statusmanager.h>
#include <simonprogresstracking/statusmanager.h>
#include <simonprogresstracking/compositeprogresswidget.h>

#include <sscobjects/user.h>

#include <QStringList>

#include <KCMultiDialog>
#include <KIcon>
#include <KLocalizedString>
#include <KStatusBar>
#include <KAction>
#include <KActionCollection>
#include <KMessageBox>
#include <KStandardDirs>

SSCView::SSCView(QWidget* parent) : KXmlGuiWindow(parent)
{
	ui.setupUi ( this );
	setupActions();

	if (SSCConfig::autoConnect()) {
		connectToServer();

	}

	connect(SSCDAccess::getInstance(), SIGNAL(connected()), this, SLOT(connected()));
	connect(SSCDAccess::getInstance(), SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(SSCDAccess::getInstance(), SIGNAL(error(const QString&)), this, SLOT(displayError(const QString&)));
	connect(SSCDAccess::getInstance(), SIGNAL(warning(const QString&)), this, SLOT(displayWarning(const QString&)));

	connect(ui.cbPatientId, SIGNAL(returnPressed()), this, SLOT(getUser()));
	connect(ui.cbPatientId, SIGNAL(currentIndexChanged(int)), this, SLOT(getUser()));
	connect(ui.pbSelectPatient, SIGNAL(clicked()), this, SLOT(getUser()));
	connect(ui.pbDetails, SIGNAL(clicked()), this, SLOT(userDetails()));
	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(deleteUser()));
}

User* SSCView::retrieveUser()
{
	bool ok;
	qint32 id = ui.cbPatientId->currentText().toInt(&ok);
	if (!ok) {
		KMessageBox::information(this, i18n("Please enter a valid user id."));
		return NULL;
	}

	User *u = SSCDAccess::getInstance()->getUser(id);
	if (!u) {
		KMessageBox::sorry(this, i18n("Couldn't retrieve user: %1", SSCDAccess::getInstance()->lastError()));
		return NULL;
	}
	return u;
}

/*
 * Fetches user information from sscd; Fired after the user enters the id in the user input field
 */
void SSCView::getUser()
{
	User *u = retrieveUser();
	if (!u) return;

	ui.lbPatientName->setText(u->surname()+", "+u->givenName());
	delete u;
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

	kDebug() << "Deleting user " << u->surname();
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
}

void SSCView::userDetails()
{
	User *u = retrieveUser();
	if (!u) return;

	ModifyUser *m = new ModifyUser(this);
	m->modifyUser(u);
	m->deleteLater();
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


SSCView::~SSCView()
{
}
