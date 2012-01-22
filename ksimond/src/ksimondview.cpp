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

#include "ksimondview.h"
#include "ksimondconfiguration.h"

#include <QStringList>

#include <KCMultiDialog>
#include <KProcess>
#include <KIcon>
#include <KLocalizedString>
#include <KAction>
#include <KActionCollection>
#include <KMessageBox>
#include <KStandardDirs>
#include <simoninfo/simoninfo.h>

KSimondView::KSimondView(QObject* parent) : QObject(parent),
trayIconMgr(0),
configDialog(new KCMultiDialog(0)),
process(new KProcess()),
startSimonAction(new KAction(0)),
startProcess(new KAction(0)),
restartProcess(new KAction(0)),
stopProcess(new KAction(0)),
configure(new KAction(0)),
stopIntended(false),
wantReload(false)
{
  KGlobal::locale()->insertCatalog("simonlib");
  configDialog->addModule("ksimondconfiguration");
  configDialog->addModule("simondconfiguration");
  configDialog->addModule("simonmodelcompilationconfig");
  //configDialog->addModule("simonduserconfiguration");
  //configDialog->addModule("simondnetworkconfiguration");

  trayIconMgr = new TrayIconManager(configDialog);
  trayIconMgr->createIcon(KIcon("simond"), i18n("simond"));
  //add actions

  startProcess->setText(i18n("Start simond"));
  startProcess->setIcon(KIcon("media-playback-start"));
  connect(startProcess, SIGNAL(triggered(bool)),
    this, SLOT(startSimond()));

  startSimonAction->setText(i18n("Start simon"));
  startSimonAction->setIcon(KIcon("simon"));
  connect(startSimonAction, SIGNAL(triggered(bool)),
    this, SLOT(startSimon()));

  restartProcess->setText(i18n("Restart simond"));
  restartProcess->setIcon(KIcon("view-refresh"));
  restartProcess->setEnabled(false);
  connect(restartProcess, SIGNAL(triggered(bool)),
    this, SLOT(restartSimond()));

  stopProcess->setText(i18n("Stop simond"));
  stopProcess->setIcon(KIcon("process-stop"));
  stopProcess->setEnabled(false);
  connect(stopProcess, SIGNAL(triggered(bool)),
    this, SLOT(stopSimond()));

  configure->setText(i18n("Configuration"));
  configure->setIcon(KIcon("configure"));
  connect(configure, SIGNAL(triggered(bool)),
    this, SLOT(showConfigurationDialog()));

  trayIconMgr->addAction(i18n("Start simon"), startSimonAction);
  trayIconMgr->addAction(i18n("Start simond"), startProcess);
  trayIconMgr->addAction(i18n("Restart simond"), restartProcess);
  trayIconMgr->addAction(i18n("Stop simond"), stopProcess);
  trayIconMgr->addAction(i18n("Configuration"), configure);

  connect(process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(matchDisplayToState()));
  connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slotError(QProcess::ProcessError)));
  connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(simondFinished()));

  if (KSimondConfiguration::autoStartSimond())
    startSimond();
}


void KSimondView::showConfigurationDialog()
{
  configDialog->show();

  //destroy when done
  //connect(configDialog, SIGNAL(finished(int)), configDialog, SLOT(deleteLater()));
}


void KSimondView::startSimon()
{
  QProcess::startDetached('"'+KStandardDirs::findExe("simon")+'"');
}


void KSimondView::startSimond()
{
  if (process->state() != QProcess::NotRunning) {
    KMessageBox::information(0, i18n("simond is already running"));
    return;
  }
  QString exePath = KStandardDirs::findExe("simond");
  process->setProgram(QStringList() << exePath);
  process->start();
}


void KSimondView::restartSimond()
{
  wantReload=true;
  stopSimond();
}


void KSimondView::matchDisplayToState()
{
  switch (process->state()) {
    case QProcess::NotRunning:
    {
      SimonInfo::showMessage(i18n("simond stopped"), 2000, new KIcon("simond"));
      startProcess->setEnabled(true);
      restartProcess->setEnabled(false);
      stopProcess->setEnabled(false);
      break;
    }
    case QProcess::Starting:
    {
      startProcess->setEnabled(false);
      restartProcess->setEnabled(false);
      stopProcess->setEnabled(false);
      break;
    }
    case QProcess::Running:
    {
      SimonInfo::showMessage(i18n("simond started"), 2000, new KIcon("simond"));
      startProcess->setEnabled(false);
      stopProcess->setEnabled(true);
      restartProcess->setEnabled(true);
      break;
    }
  }
}


void KSimondView::simondFinished()
{
  //using the kconfigxt way does not reflect changes on the fly
  KConfig config(KStandardDirs::locate("config", "ksimondrc"));
  KConfigGroup cGroup(&config, "KSimond");
  bool crashRecovery = cGroup.readEntry("AutoReStartSimond", true);
  if ((!stopIntended && crashRecovery) || wantReload) {
    startSimond();
    stopIntended=false;
    wantReload=false;
  }
}


void KSimondView::slotError(QProcess::ProcessError err)
{
  switch (err) {
    case QProcess::FailedToStart:
      KMessageBox::error(0, i18n("Could not start simond.\n\nPlease check your configuration.\n\nCommand: \"%1\"", process->program().join(", ")));
      break;
    case QProcess::Crashed:
      if (!stopIntended)
        KMessageBox::error(0, i18nc("%1 is a status message", "simond crashed. (Status: %1)", process->exitStatus()));

      break;
    case QProcess::Timedout:
      KMessageBox::error(0, i18n("Timeout."));
      break;
    case QProcess::WriteError:
      KMessageBox::error(0, i18n("Could not communicate with simond: Write failed."));
      break;
    case QProcess::ReadError:
      KMessageBox::error(0, i18n("Could not communicate with simond: Read failed."));
      break;
    default:
      KMessageBox::error(0, i18n("Unknown Error"));
      break;
  }
}


void KSimondView::stopSimond()
{
  stopIntended=true;
  process->terminate();
  if (!process->waitForFinished(1000))
    process->kill();
}


void KSimondView::closeEvent(QCloseEvent*)
{
  trayIconMgr->parentWidgetTrayClose();
}


KSimondView::~KSimondView()
{
  stopSimond();
  process->deleteLater();
  trayIconMgr->deleteLater();
  startProcess->deleteLater();
  stopProcess->deleteLater();
  configure->deleteLater();
  configDialog->deleteLater();
}
