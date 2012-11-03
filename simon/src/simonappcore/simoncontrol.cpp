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
#include <simontts/simontts.h>
#include <simoncontextdetection/contextmanager.h>
#include <simonscenarios/scenariomanager.h>
#include <simoncontextdetection/processinfo.h>

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
  
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(connected()), this, SLOT(connectedToServer()));
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(disconnected()), this, SLOT(disconnectedFromServer()));

  QObject::connect(RecognitionControl::getInstance(), SIGNAL(connectionError(QString)), this, SLOT(slotConnectionError(QString)));
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(simondSystemError(QString)), this, SLOT(slotSimondSystemError(QString)));
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(synchronisationError(QString)), this, SLOT(slotSynchronisationError(QString)));
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(recognitionError(QString,QString)), this, SLOT(slotRecognitionError(QString,QString)));
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(compilationError(QString,QString)), this, SLOT(slotCompilationError(QString,QString)));

  QObject::connect(RecognitionControl::getInstance(), SIGNAL(simondSystemWarning(QString)), this, SLOT(slotSimondSystemWarning(QString)));
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(synchronisationWarning(QString)), this, SLOT(slotSynchronisationWarning(QString)));
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(recognitionWarning(QString)), this, SLOT(slotRecognitionWarning(QString)));
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(compilationWarning(QString)), this, SLOT(slotCompilationWarning(QString)));

  QObject::connect(RecognitionControl::getInstance(), SIGNAL(loggedIn()), this, SLOT(loggedIn()));

  QObject::connect(RecognitionControl::getInstance(), SIGNAL(recognised(RecognitionResultList)), this, SLOT(wordRecognised(RecognitionResultList)));
  QObject::connect(RecognitionControl::getInstance(), SIGNAL(recognitionStatusChanged(RecognitionControl::RecognitionStatus)), this, SLOT(recognitionStatusChanged(RecognitionControl::RecognitionStatus)));

  QObject::connect(ScenarioManager::getInstance(), SIGNAL(deactivatedScenarioListChanged()), this, SIGNAL(deactivatedScenarioListChanged()));
  QObject::connect(this, SIGNAL(deactivatedScenarioListChanged()), RecognitionControl::getInstance(), SLOT(sendDeactivatedScenarioList()));
  QObject::connect(ContextManager::instance(), SIGNAL(sampleGroupChanged(QStringList)), RecognitionControl::getInstance(), SLOT(sendDeactivatedSampleGroups(QStringList)));
  ContextManager::instance()->getSampleGroupCondition()->loadSampleGroupContext();

  ActionManager::getInstance();                   // initializing action manager
  SimonTTS::getInstance();                   // initializing TTS system for dbus interface

  if (!ScenarioManager::getInstance()->init()) {
    KMessageBox::error(0, i18n("Could not initialize scenarios and shadow dictionary."));
    if (!ScenarioManager::getInstance()->getCurrentScenario()) {
      kDebug() << "Aborting due to fatal error while loading scenarios";
      exit(-1); // this is fatal
    }
  }

  connect(SoundServer::getInstance(), SIGNAL(error(QString)), this, SLOT(slotSoundError(QString)));
}


void SimonControl::startup()
{
  RecognitionControl::getInstance()->startup();
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
  SimonInfo::showMessage(i18n("User authenticated"), 1500); // krazy:exclude=qmethods
}


void SimonControl::slotConnectionError(const QString &err)
{
  KMessageBox::error(0, i18nc("%1 is error message", "Connection error: \n%1", err));
  setStatus(SimonControl::Disconnected);
}


void SimonControl::slotSimondSystemError(const QString &err)
{
  KMessageBox::error(0, i18n("The recognition server returned the following fatal error: \n%1", err));
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
  KMessageBox::detailedError(0, i18n("The recognition reported the following error: \n%1", err), log);
}


void SimonControl::slotCompilationError(const QString &err, const QString& protocol)
{
  //only display the last ~300kb; The HTML renderer copes fine with broken html so this is
  //far better than to bring the whole app to its knees just to display the detailed error message.
  QString shortProtocol = protocol.right(300000);
  KMessageBox::detailedError(0, i18n("As the server compiled the model the following error occurred:\n%1", err), shortProtocol);
}


void SimonControl::slotSimondSystemWarning(const QString& warning)
{
  SimonInfo::showMessage(i18nc("%1 is warning message", "Simond: %1", warning), 5000);
}


void SimonControl::slotSynchronisationWarning(const QString& warning)
{
  SimonInfo::showMessage(i18nc("%1 is warning message", "Model synchronization %1", warning), 5000);
}


void SimonControl::slotRecognitionWarning(const QString& warning)
{
  SimonInfo::showMessage(i18nc("%1 is warning message", "Recognition: %1", warning), 5000);
}


void SimonControl::slotCompilationWarning(const QString& warning)
{
  SimonInfo::showMessage(i18nc("%1 is warning message", "Model management: %1", warning), 5000);
}


/**
 * @brief Connects to RecognitionControl::getInstance()
 *
 *	@author Peter Grasch
 */
void SimonControl::connectToServer()
{
  setStatus(SimonControl::Connecting);
  RecognitionControl::getInstance()->startConnecting();
}


/**
 * @brief disconnects from RecognitionControl::getInstance()
 *
 *	@author Peter Grasch
 */
void SimonControl::disconnectFromServer()
{
  RecognitionControl::getInstance()->disconnectFromServer();
}


/**
 * @brief Word recognised
 *
 * usually called when the server recognised a word
 *
 *	@author Peter Grasch
 */
void SimonControl::wordRecognised(RecognitionResultList recognitionResults)
{
  if (status != SimonControl::ConnectedActivated) return;

  kDebug() << "Received recognition results...";
  ActionManager::getInstance()->processRawResults(recognitionResults);
}


void SimonControl::recognitionStatusChanged(RecognitionControl::RecognitionStatus status)
{
  switch (status) {
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


void SimonControl::setStatus(SimonControl::SystemStatus newStatus)
{
  status = newStatus;
  emit systemStatusChanged(newStatus);
}


/**
 * @brief Server connected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the RecognitionControl::getInstance() socket was established
 *
 * @author Peter Grasch
 */
void SimonControl::connectedToServer()
{
  setStatus(SimonControl::ConnectedDeactivatedNotReady);
  Logger::log(i18n("Connected to the Server"));
}


/**
 * @brief Server disconnected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the RecognitionControl::getInstance() socket was lost
 *
 * @author Peter Grasch
 */
void SimonControl::disconnectedFromServer()
{
  setStatus(SimonControl::Disconnected);
  Logger::log(i18n("Connection lost"));
}


/**
 * @brief We want to abort connecting
 *
 * @author Peter Grasch
 */
void SimonControl::abortConnecting()
{
  Logger::log(i18n("Connecting aborted"));
  RecognitionControl::getInstance()->disconnectFromServer();
}


/**
 * @brief Toggles the activition
 *
 *	@author Peter Grasch
 */
SimonControl::SystemStatus SimonControl::toggleActivition()
{
  if (status==SimonControl::ConnectedActivated) {
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
  RecognitionControl::getInstance()->setBlockAutoStart(false);
  if (status == SimonControl::ConnectedDeactivatedReady) {
    Logger::log(i18n("Simon activated"));
    setStatus(SimonControl::ConnectedActivating);
    if (!RecognitionControl::getInstance()->startRecognition())
            setStatus(SimonControl::ConnectedDeactivatedReady);
  }
  if (status == SimonControl::ConnectedPaused) {
    Logger::log(i18n("Continuing recognition"));
    setStatus(SimonControl::ConnectedResuming);
    if (!RecognitionControl::getInstance()->resumeRecognition())
            setStatus(SimonControl::ConnectedPaused);
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
  RecognitionControl::getInstance()->setBlockAutoStart(true);
  if (status == SimonControl::ConnectedActivated) {
    setStatus(SimonControl::ConnectedDeactivating);
    Logger::log(i18n("Simon deactivated"));
    RecognitionControl::getInstance()->pauseRecognition();
  }
  return status;
}


void SimonControl::compileModel()
{
  RecognitionControl::getInstance()->startSynchronisation();
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
SimonControl::~SimonControl()
{
    delete ContextManager::instance();
}
