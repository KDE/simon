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
#include <QtConcurrentRun>
#include <QFutureWatcher>

#include <KTitleWidget>
#include <KMessageBox>
#include <KProgressDialog>
#include <KDialog>
#include <KDebug>
#include "simonsampledataprovider.h"
#include <sscobjects/microphone.h>
#include <sscobjects/soundcard.h>


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
  
  connect(ui->cbLicense, SIGNAL(clicked(bool)), this, SLOT(checkCompletion()));
  connect(ui->cbDialect, SIGNAL(editTextChanged(const QString&)), this, SLOT(checkCompletion()));
  
  connect(ui->cbLanguage, SIGNAL(currentIndexChanged(int)), this, SLOT(listDialects()));
  
  connect(server, SIGNAL(error(const QString&)), this, SLOT(transmissionError(const QString&)));
  connect(server, SIGNAL(warning(const QString&)), this, SLOT(transmissionWarning(const QString&)));
  connect(server, SIGNAL(status(const QString&, int, int)), this, SLOT(transmissionStatus(const QString&)));
  
  connect(server, SIGNAL(connected()), this, SLOT(connected()));
  connect(server, SIGNAL(disconnected()), this, SLOT(disconnected()));
  
  KTitleWidget *titleWidget = new KTitleWidget(this);
  titleWidget->setText(i18n("Upload samples"));
  titleWidget->setComment(i18n("Give back to the community by sharing your training data."));
  titleWidget->setPixmap(KIcon("repository").pixmap(22, 22));
  dynamic_cast<QVBoxLayout*>(sampleShareWidget->layout())->insertWidget(0, titleWidget);
  
  futureWatcher = new QFutureWatcher<bool>(this);
  connect(futureWatcher, SIGNAL(finished()), this, SLOT(transmissionFinished()));
}

void SampleShare::transmissionFinished()
{
  kDebug() << "Transmission finished";
  progressWidget->update();

  enableButtonOk(true);
  setButtonText(Ok, i18n("Ok"));
  
  if (!futureWatcher->result())
    transmissionError(server->lastError());
}

void SampleShare::checkCompletion()
{
  enableButton(Ok, (ui->cbLicense->isChecked() && !(ui->cbDialect->currentText().isEmpty())));
}


void SampleShare::connected()
{
  connectionProgressDialog->progressBar()->setValue(1);
  //fetch options
  bool ok = true;
  bool allOk = true;
  ui->stackedWidget->setCurrentIndex(1);
  setButtonText(Ok, i18n("Upload"));

  QList<Microphone*> microphones = server->getMicrophones(&ok);
  if (ok) {
    foreach (Microphone* m, microphones) {
      QString model = m->model();
      QString type = m->type();
      QString name;
      if (model.isEmpty() || model == "Unspecified") //do not translate
	name = type;
      else
	name = QString("%1 (%2)").arg(type).arg(model);
      
      ui->cbMicrophone->addItem(name, m->id());
    }
  } else {
    allOk = false;
    transmissionError(server->lastError());
  }
  
  QList<Language*> languages = server->getLanguages(&ok);
  if (ok) {
    foreach (Language* l, languages)
      ui->cbLanguage->addItem(l->name(), l->id());
  } else {
    allOk = false;
    transmissionError(server->lastError());
  }
  QString userLanguage = KGlobal::locale()->language();
  userLanguage = userLanguage.left(userLanguage.indexOf("_")); //strip e.g. "_US"
  ui->cbLanguage->setCurrentIndex(ui->cbLanguage->findData(userLanguage));
  
  if (allOk) {   
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
  progressWidget->disconnect();
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

void SampleShare::listDialects()
{
  kDebug() << "Listing dialects...";
  bool ok = true;
  User* filter = new User(-1, QString(), QString(), 
			  ' ', 0, QString(), QString(), QString(), 
			  ui->cbLanguage->itemData(ui->cbLanguage->currentIndex()).toString(),
			  ui->cbLanguage->currentText(), QString(), 0, 0, 0, QString(), 2, 2);
  QList<User*> users = server->getUsers(filter, 0, QString(), &ok);
  
  ui->cbDialect->clear();
  if (!ok) {
    transmissionError(server->lastError());
  } else {
    bool otherAlreadyIn = false;
    kDebug() << "got " << users.count() << "users";
    foreach (User *u, users) {
      QString dialect = u->surname();
      if (!u->givenName().isEmpty())
	dialect += " "+u->givenName();
      
      if (dialect == i18n("Other / None"))
	otherAlreadyIn = true;
      
      if (!ui->cbDialect->contains(dialect)) //no duplicates (gender, etc.)
	ui->cbDialect->addItem(dialect, u->givenName());
    }
    if (!otherAlreadyIn)
      ui->cbDialect->addItem(i18n("Other / None"));
  }
}


void SampleShare::startTransmission()
{
  cleanup();
  
  if (TrainingManager::getInstance()->getPrompts()->count() == 0) {
    KMessageBox::information(this, i18n("No samples to upload.\n\n"
		    "Please collect training data by reading training texts or "
		    "running individual training sessions.\n\n"
		    "Afterwards you can submit your samples with this assistant."));
    accept();
    return;
  }
  
  //find (or create "unspecified" sound card)
  bool ok;
  QList<SoundCard*> cards = server->getSoundCards(&ok);
  SoundCard *card = new SoundCard(-1, "Unspecified", "Unspecified"); // do not translate
  qint32 soundCardId = server->getOrCreateSoundCard(card, &ok);
  delete card;
  if (!ok) {
    transmissionError(server->lastError());
    return;
  }
  
  qint32 microphoneId = ui->cbMicrophone->itemData(ui->cbMicrophone->currentIndex()).toInt();
  
  kDebug() << "Sound card: " << soundCardId << "Microphone: " << microphoneId;
  
  //First, find available "Dialects" (user surnames matching a query for the language)
  QChar userGender((ui->cbGender->currentIndex() == 0) ? 'M' : 'F');
  QString languageCode(ui->cbLanguage->itemData(ui->cbLanguage->currentIndex()).toString());
  QString language(ui->cbLanguage->currentText());
  
  QString configuredDialect = ui->cbDialect->currentText();
  QString configuredDialectGivenName = ui->cbDialect->itemData(ui->cbDialect->currentIndex()).toString();
  QString configuredDialectSurname = configuredDialectGivenName.isEmpty() ? configuredDialect :
				      configuredDialect.left(configuredDialect.length()-
					configuredDialectGivenName.length()-1);
  
  if (configuredDialectGivenName.isNull()) configuredDialectGivenName = "";
  qint32 userId = -1;
  kDebug() << configuredDialectGivenName.isNull();
  kDebug() << configuredDialectGivenName.isEmpty();
  kDebug() << "Given name: " << configuredDialectGivenName;
  User* filter = new User(-1, configuredDialectSurname, configuredDialectGivenName, 
			  userGender, 0, "", "", "", languageCode,
			  language, "", 0, 0, 0, "", 2, 2);
  QList<User*> users = server->getUsers(filter, 0, QString(), &ok);
  
  //encode birth range in "years":
  //	10000 = Youth
  //	10001 = Adult
  //	10002 = Senior
  int encodedBirthYear = 10000 + ui->cbAgeRange->currentIndex();
  
  if (!users.isEmpty()) {
    kDebug() << "Got users: ";
    foreach (User *u, users) {
      //if we find one real user, use that one
      //or in other words: ssc > simon
      if (u->birthYear() < 10000) {
	userId = u->userId();
	break;
      }
      if (u->birthYear() == encodedBirthYear)
	userId = u->userId();
    }
  }
  if (userId == -1) {
    kDebug() << "User empty - creating a new one with encoded age";
    filter->setBirthYear(encodedBirthYear);
    filter->setInterviewPossible(1); //set to true for lack of better knowledge
    filter->setRepeatingPossible(1);
    filter->setMotorFunction(1);
    filter->setCommunication(1);
    filter->setOrientation(1);
    userId = server->addUser(filter);
    if (userId < 1) {
      transmissionError(server->lastError());
      return;
    }
  }
  
  qDeleteAll(users);
  delete filter;
  
  /* mic, soundcard? */
  worker = new SendSampleWorker(server, new SimonSampleDataProvider(userId, 
							    new Microphone(microphoneId, QString(), QString()),
							    new SoundCard(soundCardId, QString(), QString()),
							    Sample::Training, i18n("simon training data")));
  
  transmissionOperation = new Operation(QThread::currentThread(), i18n("Uploading samples..."), i18n("Initializing"), 0, 0, false);
  progressWidget = new ProgressWidget(transmissionOperation, ProgressWidget::Large, this);
  connect(worker, SIGNAL(aborted()), transmissionOperation, SLOT(canceled()), Qt::QueuedConnection);
  connect(worker, SIGNAL(finished()), transmissionOperation, SLOT(finished()), Qt::QueuedConnection);
  connect(transmissionOperation, SIGNAL(aborting()), worker, SLOT(abort()), Qt::QueuedConnection);
  
  connect(worker, SIGNAL(error(QString)), this, SLOT(displayError(QString)), Qt::QueuedConnection);
  connect(worker, SIGNAL(status(QString, int, int)), this, SLOT(displayStatus(QString, int, int)), Qt::QueuedConnection);
  
  connect(worker, SIGNAL(sendSample(Sample*)), this, SLOT(sendSample(Sample*)));

  static_cast<QVBoxLayout*>(ui->swMainUploadPage->layout())->addWidget(progressWidget);
  progressWidget->show();
  
  QFuture<bool> future = QtConcurrent::run(worker, &SendSampleWorker::sendSamples);
  futureWatcher->setFuture(future);
  kDebug() << "Thread started";
  enableButtonCancel(false); //job has it's own cancel button
}

void SampleShare::displayError(QString error)
{
  KMessageBox::error(this, error);
}

void SampleShare::displayStatus(QString message, int now, int max)
{
  transmissionOperation->update(message, now, max);
  progressWidget->update();
}

void SampleShare::sendSample(Sample *s)
{
  if (!server->sendSample(s))
    KMessageBox::error(this, i18n("Could not send sample"));
}
