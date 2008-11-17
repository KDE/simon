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

#include <QStringList>

#include <KCMultiDialog>
#include <KProcess>
#include <KIcon>
#include <KLocalizedString>
#include <KAction>
#include <KActionCollection>
#include <KMessageBox>
#include <KStandardDirs>

KSimondView::KSimondView(QObject *parent):QObject(parent)
{
	trayIconMgr = new TrayIconManager();
	trayIconMgr->createIcon(KIcon("simond"), i18n("simond"));
	//add actions
	startProcess = new KAction(0);
	startProcess->setText(i18n("simond starten"));
	startProcess->setIcon(KIcon("media-playback-start"));
	connect(startProcess, SIGNAL(triggered(bool)),
			this, SLOT(startSimond()));


	stopProcess = new KAction(0);
	stopProcess->setText(i18n("simond stoppen"));
	stopProcess->setIcon(KIcon("process-stop"));
	stopProcess->setEnabled(false);
	connect(stopProcess, SIGNAL(triggered(bool)),
			this, SLOT(stopSimond()));

	configure = new KAction(0);
	configure->setText(i18n("Konfiguration"));
	configure->setIcon(KIcon("configure"));
	connect(configure, SIGNAL(triggered(bool)),
			this, SLOT(showConfigurationDialog()));

	trayIconMgr->addAction(i18n("simond starten"), startProcess);
	trayIconMgr->addAction(i18n("simond stoppen"), stopProcess);
	trayIconMgr->addAction(i18n("Konfiguration"), configure);

	process = new KProcess();
	connect(process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(matchDisplayToState()));
	connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slotError(QProcess::ProcessError)));
}

void KSimondView::showConfigurationDialog()
{
	KCMultiDialog *configDialog = new KCMultiDialog(0);
//	configDialog->addModule("ksimondconfiguration");
	configDialog->addModule("simonduserconfiguration");
	configDialog->addModule("simondnetworkconfiguration");
	configDialog->show();

	//destroy when done
	connect(configDialog, SIGNAL(finished(int)), configDialog, SLOT(deleteLater()));
}


void KSimondView::startSimond()
{
	if (process->state() != QProcess::NotRunning)
	{
		KMessageBox::information(0, i18n("simond läuft bereits"));
		return;
	}
	QString exePath = KStandardDirs::findExe("simond");
	process->setProgram(QStringList() << exePath);
	process->start();
}

void KSimondView::matchDisplayToState()
{
	switch (process->state())
	{
		case QProcess::NotRunning:
		{
			startProcess->setEnabled(true);
			stopProcess->setEnabled(false);
			break;
		}
		case QProcess::Starting:
		{
			startProcess->setEnabled(false);
			stopProcess->setEnabled(false);
			break;
		}
		case QProcess::Running:
		{
			startProcess->setEnabled(false);
			stopProcess->setEnabled(true);
			break;
		}
	}
}

void KSimondView::slotError(QProcess::ProcessError err)
{
	switch (err)
	{
		case QProcess::FailedToStart:
			KMessageBox::error(0, i18n("Konnte simond nicht starten.\n\nBitte überprüfen Sie die ksimond Konfiguration.\n\nAusgeführter Befehl:\"%1\"", process->program().join(", ")));
			break;
		case QProcess::Crashed:
			KMessageBox::error(0, i18n("simond ist abgestürzt. (Status: %1)", process->exitStatus()));
			break;
		case QProcess::Timedout:
			KMessageBox::error(0, i18n("Zeitüberschreitung."));
			break;
		case QProcess::WriteError:
			KMessageBox::error(0, i18n("Konnte nicht mit simond kommunizieren: Schreiben fehlgeschlagen."));
			break;
		case QProcess::ReadError:
			KMessageBox::error(0, i18n("Konnte nicht mit simond kommunizieren: Lesen fehlgeschlagen."));
			break;
		default:
			KMessageBox::error(0, i18n("Unbekannter simond Fehler"));
			break;
	}
}


void KSimondView::stopSimond()
{
	process->terminate();
	if (!process->waitForFinished())
		process->kill();
}

KSimondView::~KSimondView()
{
	stopSimond();
	process->deleteLater();
	trayIconMgr->deleteLater();
	startProcess->deleteLater();
	stopProcess->deleteLater();
	configure->deleteLater();
}
