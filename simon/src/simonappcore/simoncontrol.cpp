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


#include "simoncontrol.h"

#include <simonactions/actionmanager.h>

#include "coreconfiguration.h"

#include <KMessageBox>
#include <KLocalizedString>
#include <simonlogging/logger.h>
#include <simoninfo/simoninfo.h>
#include <simonrecognitionresult/recognitionresult.h>
#include <simonsound/soundserver.h>

#include <QFileInfo>
#include <KDebug>

/**
 * @brief Constructor
 * 
 * Sets the activitionstate to true
 *
 *	@author Peter Grasch
*/
SimonControl::SimonControl(QWidget *parent) : QObject (parent)
{
	setStatus(SimonControl::Disconnected);
	this->recognitionControl = RecognitionControl::getInstance(parent);
	QObject::connect(recognitionControl, SIGNAL(connected()), this, SLOT(connectedToServer()));
	QObject::connect(recognitionControl, SIGNAL(disconnected()), this, SLOT(disconnectedFromServer()));
	
	QObject::connect(recognitionControl, SIGNAL(connectionError(const QString&)), this, SLOT(slotConnectionError(const QString&)));
	QObject::connect(recognitionControl, SIGNAL(simondSystemError(const QString&)), this, SLOT(slotSimondSystemError(const QString&)));
	QObject::connect(recognitionControl, SIGNAL(synchronisationError(const QString&)), this, SLOT(slotSynchronisationError(const QString&)));
	QObject::connect(recognitionControl, SIGNAL(recognitionError(const QString&, const QString&)), this, SLOT(slotRecognitionError(const QString&, const QString&)));
	QObject::connect(recognitionControl, SIGNAL(compilationError(const QString&, const QString&)), this, SLOT(slotCompilationError(const QString&, const QString&)));

	QObject::connect(recognitionControl, SIGNAL(simondSystemWarning(const QString&)), this, SLOT(slotSimondSystemWarning(const QString&)));
	QObject::connect(recognitionControl, SIGNAL(synchronisationWarning(const QString&)), this, SLOT(slotSynchronisationWarning(const QString&)));
	QObject::connect(recognitionControl, SIGNAL(recognitionWarning(const QString&)), this, SLOT(slotRecognitionWarning(const QString&)));
	QObject::connect(recognitionControl, SIGNAL(compilationWarning(const QString&)), this, SLOT(slotCompilationWarning(const QString&)));

	QObject::connect(recognitionControl, SIGNAL(loggedIn()), this, SLOT(loggedIn()));
	
	QObject::connect(recognitionControl, SIGNAL(recognised(RecognitionResultList*)), this, SLOT(wordRecognised(RecognitionResultList*)));
	QObject::connect(recognitionControl, SIGNAL(recognitionStatusChanged(RecognitionControl::RecognitionStatus)), this, SLOT(recognitionStatusChanged(RecognitionControl::RecognitionStatus)));

	ActionManager::getInstance(); // initializing action manager

	if (!ScenarioManager::getInstance()->init())
		KMessageBox::error(0, i18n("Couldn't initialize scenarios and shadow dictionary."));

	connect(SoundServer::getInstance(), SIGNAL(error(const QString&)), this, SLOT(slotSoundError(const QString&)));
}
void SimonControl::startup()
{
	recognitionControl->startup();
}

bool SimonControl::startMinimized()
{ return CoreConfiguration::startMinimized(); }

bool SimonControl::firstRunWizardCompleted()
{
	return CoreConfiguration::firstRunWizardCompleted();
}

void SimonControl::setFirstRunWizardCompleted(bool completed)
{
	CoreConfiguration::setFirstRunWizardCompleted(completed);
	CoreConfiguration::self()->writeConfig();
}

void SimonControl::loggedIn()
{
	SimonInfo::showMessage(i18n("User athenticated"), 1500);
}

void SimonControl::slotConnectionError(const QString &err)
{
	KMessageBox::error(0, i18n("Connection Error: \n%1", err));
	setStatus(SimonControl::Disconnected);
}

void SimonControl::slotSimondSystemError(const QString &err)
{
	KMessageBox::error(0, i18n("The Recognition Server returned the following fatal error: \n%1", err));
}

void SimonControl::slotSoundError(const QString &err)
{
	KMessageBox::error(0, i18n("The sound system returned the following error: \n%1", err));
}

void SimonControl::slotSynchronisationError(const QString &err)
{
	KMessageBox::error(0, i18n("The model synchronization reported the following error: \n%1", err));	
}

void SimonControl::slotRecognitionError(const QString &err, const QString& log)
{
	KMessageBox::detailedError(0, i18n("The Recognition reported the following error: \n%1", err), log);
}

void SimonControl::slotCompilationError(const QString &err, const QString& protocol)
{
	KMessageBox::detailedError(0, i18n("As the server compiled the model the following error occured:\n%1", err), protocol);
}


void SimonControl::slotSimondSystemWarning(const QString& warning)
{
	SimonInfo::showMessage(i18n("simond: %1", warning), 5000);
}

void SimonControl::slotSynchronisationWarning(const QString& warning)
{
	SimonInfo::showMessage(i18n("Model Synchronisation %1", warning), 5000);
}

void SimonControl::slotRecognitionWarning(const QString& warning)
{
	SimonInfo::showMessage(i18n("Recognition: %1", warning), 5000);
}

void SimonControl::slotCompilationWarning(const QString& warning)
{
	SimonInfo::showMessage(i18n("Modelmanagement: %1", warning), 5000);
}



/**
 * @brief Connects to recognitionControl
 *
 *	@author Peter Grasch
 */
void SimonControl::connectToServer()
{
	setStatus(SimonControl::Connecting);
	recognitionControl->startConnecting();
}


/**
 * @brief disconnects from recognitionControl
 *
 *	@author Peter Grasch
 */
void SimonControl::disconnectFromServer()
{
	recognitionControl->disconnectFromServer();
}


/**
 * @brief Word recognised
 * 
 * usually called when the server recognised a word
 *
 *	@author Peter Grasch
 */
void SimonControl::wordRecognised(RecognitionResultList* recognitionResults)
{
	if (status != SimonControl::ConnectedActivated) return;

	kDebug() << "Received recognition results...";
	ActionManager::getInstance()->processRawResults(recognitionResults);
}


void SimonControl::recognitionStatusChanged(RecognitionControl::RecognitionStatus status)
{
	switch (status)
	{
		case RecognitionControl::Ready:
		{
			setStatus(SimonControl::ConnectedDeactivatedReady);
			break;
		}
		
		case RecognitionControl::Started:
		{
			setStatus(SimonControl::ConnectedActivated);
			break;
		}
		
		case RecognitionControl::Paused:
		{
			setStatus(SimonControl::ConnectedPaused);
			break;
		}
		
		case RecognitionControl::Resumed:
		{
			setStatus(SimonControl::ConnectedActivated);
			break;
		}
		
		case RecognitionControl::Stopped:
		{
			setStatus(SimonControl::ConnectedDeactivatedNotReady);
			break;
		}
		
	}
}

void SimonControl::setStatus(SimonControl::SystemStatus status)
{
	this->status = status;
	emit systemStatusChanged(status);
}

/**
 * @brief Server connected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the recognitionControl socket was established
 *
 * @author Peter Grasch
 */
void SimonControl::connectedToServer()
{
	setStatus(SimonControl::ConnectedDeactivatedNotReady);
	Logger::log(i18n("[INF]")+" "+i18n("Connected to the Server"));
}

/**
 * @brief Server disconnected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the recognitionControl socket was lost
 *
 * @author Peter Grasch
 */
void SimonControl::disconnectedFromServer()
{
	setStatus(SimonControl::Disconnected);
	Logger::log(i18n("[INF] Connection lost"));
}

/**
 * @brief We want to abort connecting to recognitionControl
 * 
 * @author Peter Grasch
 */
void SimonControl::abortConnecting()
{
	Logger::log(i18n("[INF] Connecting aborted"));
	this->recognitionControl->disconnectFromServer();
}



/**
 * @brief Toggles the activition
 *
 *	@author Peter Grasch
 */
SimonControl::SystemStatus SimonControl::toggleActivition()
{
	if (status==SimonControl::ConnectedActivated)
	{
		deactivateSimon();
	} else if ((status==SimonControl::ConnectedDeactivatedReady) || (status==SimonControl::ConnectedPaused))
		activateSimon();
	
	return status;
}

/**
 * @brief Activates Simon
 *
 *	@author Peter Grasch
 */
SimonControl::SystemStatus SimonControl::activateSimon()
{
	if (status == SimonControl::ConnectedDeactivatedReady)
	{
		Logger::log(i18n("[INF] simon activated"));
		setStatus(SimonControl::ConnectedActivating);
		recognitionControl->startRecognition();
	}
	if (status == SimonControl::ConnectedPaused)
	{
		Logger::log(i18n("[INF] Continuing recognition"));
		setStatus(SimonControl::ConnectedResuming);
		recognitionControl->resumeRecognition();
	}
	return status;
}


/**
 * @brief Deactivates Simon
 *
 *	@author Peter Grasch
 */
SimonControl::SystemStatus SimonControl::deactivateSimon()
{
	if (status == SimonControl::ConnectedActivated)
	{
		setStatus(SimonControl::ConnectedDeactivating);
		Logger::log(i18n("[INF] simon deactivated"));
		recognitionControl->pauseRecognition();
	}
	return status;
}

void SimonControl::compileModel()
{
	recognitionControl->startSynchronisation();
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
SimonControl::~SimonControl()
{
	delete recognitionControl;
}
