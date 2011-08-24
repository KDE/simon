/*
 *   Copyright (C) 2011 Alessandro Buggin <alessandrobuggin@gmail.com>
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#include "sampleshare.h"
#include "ui_sampleshare.h"

#include <sscdaccess/sscdaccess.h>
#include <sscdaccess/sendsampleworker.h>
#include <simonprogresstracking/operation.h>
#include <simonprogresstracking/progresswidget.h>

#include <simonscenarios/trainingmanager.h>

#include <QVBoxLayout>
#include <QThread>

#include <KMessageBox>
#include <KProgressDialog>
#include <KDialog>
#include <KDebug>
#include "simonsampledataprovider.h"


SampleShare::SampleShare(QWidget* parent): 
    KDialog(parent), ui(new Ui::SampleShareDlg),
    server(new SSCDAccess(this)),
    worker(0),
    transmissionOperation(0),
    progressWidget(0),
    connectionProgressDialog(0)
{
  QWidget *sampleShareWidget = new QWidget( this );
  ui->setupUi(sampleShareWidget);
  setMainWidget(sampleShareWidget);
  setWindowIcon(KIcon("repository"));
  enableButtonOk(true);
  setButtonText(Ok, i18n("Connect to server"));
  
  connect(ui->licenseBox, SIGNAL(clicked(bool)), this, SLOT(enableButtonOk(bool)));
  
  kDebug() << TrainingManager::getInstance()->getTrainingDir() << true;
  kDebug() << TrainingManager::getInstance()->getPrompts()->keys() <<true;
  
  connect(server, SIGNAL(error(const QString&)), this, SLOT(transmissionError(const QString&)));
  connect(server, SIGNAL(warning(const QString&)), this, SLOT(transmissionWarning(const QString&)));
  connect(server, SIGNAL(status(const QString&, int, int)), this, SLOT(transmissionStatus(const QString&)));
  
  connect(server, SIGNAL(connected()), this, SLOT(connected()));
  connect(server, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void SampleShare::connected()
{
  connectionProgressDialog->progressBar()->setValue(1);
  //fetch options
  bool ok = true;
  ui->stackedWidget->setCurrentIndex(1);
  setButtonText(Ok, i18n("Upload"));

  QList<User*> users = server->getUsers(new User(), 0, QString(), &ok);
  if (!ok)
    transmissionError(server->lastError());
  else {
    foreach (User* u, users)
      kDebug() << "Got user: " << u->givenName();
    qDeleteAll(users);
    
    connectionProgressDialog->deleteLater();
    connectionProgressDialog = 0;
  }
}

void SampleShare::disconnected()
{
  KMessageBox::sorry(this, i18n("Disconnected from remote server. Please try again."));
  ui->stackedWidget->setCurrentIndex(0);
  setButtonText(Ok, i18n("Connect to server"));
  connectionProgressDialog->hide();
  enableButtonOk(true);
}

void SampleShare::transmissionError(const QString& err)
{
  kDebug() << "Transmission error: " << err;
  KMessageBox::sorry(this, i18n("The following error occured:\n\n%1", err));
  
  if (connectionProgressDialog) {
    enableButtonOk(true); // let the user try again
  
    connectionProgressDialog->deleteLater();
    connectionProgressDialog = 0;
  }
}

void SampleShare::transmissionStatus(const QString& status)
{
  kDebug() << status;
}

void SampleShare::transmissionWarning(const QString& warning)
{
  kWarning() << "WARNING: " << warning;
}

SampleShare::~SampleShare(){
  cleanup();
  //server deleted by qt parent / child relationship
  delete ui;
}

void SampleShare::slotButtonClicked(int button) {
  switch (button) {
    case KDialog::Ok:
      switch (ui->stackedWidget->currentIndex()) {
	case 0:
	  connectToServer();
	  return;
	case 1:
	  ui->stackedWidget->setCurrentWidget(ui->swMainUploadPage);
	  startTransmission();
	  enableButton(Ok, false);
	  return;
	default:
	  break;
      }
    case KDialog::Cancel:
      break;
  }
  
  KDialog::slotButtonClicked(button);
}

void SampleShare::cleanup()
{
  if (progressWidget) {
    static_cast<QVBoxLayout*>(ui->swMainUploadPage->layout())->removeWidget(progressWidget);
    progressWidget->deleteLater();
  }
  if (transmissionOperation)
    transmissionOperation->canceled();
  
  if (worker)
    worker->deleteLater();
}

void SampleShare::connectToServer()
{
  if (connectionProgressDialog)
    connectionProgressDialog->deleteLater();
  
  connectionProgressDialog = new KProgressDialog(this, i18n("Connecting..."));
  
  connectionProgressDialog->progressBar()->setMaximum(2);
  connectionProgressDialog->progressBar()->setValue(0);
  
  bool ok = true;
  //config file
  QString remoteHost = ui->leRemoteHost->text();
  QString host = remoteHost.left(remoteHost.lastIndexOf(":"));
  qint16 port = remoteHost.mid(host.count()+1).toInt(&ok);
  bool encrypted = ui->cbUseEncryption->isChecked();
  
  if (!ok) {
    KMessageBox::information(this, i18n("Please provide a valid remote host in the format host:port"));
    return;
  }
  
  enableButtonOk(false);
  
  server->connectTo(host, port, encrypted);
}


void SampleShare::startTransmission()
{
  cleanup();
  
  qint32 userId;
  
  worker = new SendSampleWorker(new SimonSampleDataProvider(userId, Sample::Training, i18n("simon training data")));
  
  transmissionOperation = new Operation(QThread::currentThread(), i18n("Uploading samples..."), i18n("Initializing"), 0, 0, false);
  progressWidget = new ProgressWidget(transmissionOperation, ProgressWidget::Large, this);
  connect(worker, SIGNAL(aborted()), transmissionOperation, SLOT(canceled()), Qt::QueuedConnection);
  connect(worker, SIGNAL(finished()), transmissionOperation, SLOT(finished()), Qt::QueuedConnection);
  connect(transmissionOperation, SIGNAL(aborting()), worker, SLOT(abort()), Qt::QueuedConnection);

  static_cast<QVBoxLayout*>(ui->swMainUploadPage->layout())->addWidget(progressWidget);
  progressWidget->show();
}

