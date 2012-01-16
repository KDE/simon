/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "sendsamplespage.h"
#include "sscconfig.h"
#include "sscsampledataprovider.h"

#include <sscdaccess/sscdaccess.h>
#include <sscdaccess/sendsampleworker.h>
#include <sscobjects/sample.h>
#include <simonprogresstracking/operation.h>
#include <simonprogresstracking/progresswidget.h>

#include <QDir>
#include <QLabel>
#include <QtConcurrentRun>

#include <QThread>
#include <QVBoxLayout>
#include <QFuture>
#include <QFutureWatcher>

#include <KMessageBox>
#include <KLocalizedString>
#include <KPushButton>
#include <KDebug>
#include <sscdaccess/sscdaccesssingleton.h>

SendSamplePage::SendSamplePage(AbstractSampleDataProvider* dataProvider, bool isStored, const QString& ini, QWidget* parent) :
    QWizardPage(parent),
    m_isStored(isStored),
    worker(new SendSampleWorker(SSCDAccessSingleton::getInstance(), dataProvider, isStored, ini)),
    layout(new QVBoxLayout()),
    m_transmitOperation(0),
    m_progressWidget(0)
{
  setTitle(i18n("Transmitting samples..."));

  QLabel *desc = new QLabel(i18n("The recorded samples are now ready to be sent to the server or stored locally to be sent later."), this);
  desc->setWordWrap(true);
  layout->addWidget(desc);
  pbReSendData = new KPushButton(KIcon("view-refresh"), i18n("Send samples"), this);
  connect(pbReSendData, SIGNAL(clicked()), this, SLOT(prepareDataSending()));
  layout->addWidget(pbReSendData);

  pbStoreData  = new KPushButton(KIcon("document-save"), i18n("Store samples (send later)"), this);
  connect(pbStoreData, SIGNAL(clicked()), this, SLOT(storeData()));
  layout->addWidget(pbStoreData);

  if (isStored) {
    pbStoreData->hide();
  }

  futureWatcher = new QFutureWatcher<bool>(this);
  connect(futureWatcher, SIGNAL(finished()), this, SLOT(transmissionFinished()));
  //make sure we got the last bit
  //	connect(futureWatcher, SIGNAL(finished()), this, SLOT(displayStatus()));
  setLayout(layout);

  connect(worker, SIGNAL(error(QString)), this, SLOT(displayError(QString)), Qt::QueuedConnection);
  connect(worker, SIGNAL(status(QString, int, int)), this, SLOT(displayStatus(QString, int, int)), Qt::QueuedConnection);
  connect(worker, SIGNAL(sendSample(Sample*)), this, SLOT(sendSample(Sample*)), Qt::QueuedConnection);
}


void SendSamplePage::setupOperation(const QString& name)
{
  if (m_progressWidget) {
    layout->removeWidget(m_progressWidget);
    m_progressWidget->deleteLater();
  }

  if (m_transmitOperation && m_transmitOperation->isRunning())
    m_transmitOperation->canceled();

  m_transmitOperation = new Operation(QThread::currentThread(), name, i18n("Initializing"), 0, 0, false);
  m_progressWidget = new ProgressWidget(m_transmitOperation, ProgressWidget::Large, this);
  connect(worker, SIGNAL(aborted()), m_transmitOperation, SLOT(canceled()), Qt::QueuedConnection);
  //connect(worker, SIGNAL(aborted()), pbReSendData, SLOT(setEnabled()), Qt::QueuedConnection);
  //connect(worker, SIGNAL(aborted()), pbStoreData, SLOT(setEnabled()), Qt::QueuedConnection);
  connect(worker, SIGNAL(finished()), m_transmitOperation, SLOT(finished()), Qt::QueuedConnection);
  connect(m_transmitOperation, SIGNAL(aborting()), worker, SLOT(abort()), Qt::QueuedConnection);

  layout->addWidget(m_progressWidget);
  m_progressWidget->show();
}


void SendSamplePage::storeData()
{
  setupOperation(i18n("Storing data..."));

  pbReSendData->setEnabled(false);
  pbStoreData->setEnabled(false);

  //storing data
  QFuture<bool> future = QtConcurrent::run(worker, &SendSampleWorker::storeData);
  futureWatcher->setFuture(future);
  //worker->storeData();

  transmissionFinished();
}


void SendSamplePage::transmissionFinished()
{
  kDebug() << "Transmission finished";

  updateStatusDisplay();
  emit completeChanged();

  wizard()->button(QWizard::CancelButton)->setEnabled(true);

  if (!futureWatcher->result() || worker->getShouldAbort()) {
    kDebug() << "Result: " << futureWatcher->result() << " should abort: " << worker->getShouldAbort();
    pbReSendData->setEnabled(true);
    pbStoreData->setEnabled(true);
    wizard()->button(QWizard::BackButton)->setEnabled(true);
  }
  else {
    if (m_isStored) {
      //if the data is stored anyways we can simple restart
      wizard()->button(QWizard::BackButton)->setEnabled(false);
    } else
    wizard()->button(QWizard::BackButton)->setEnabled(true);
  }
}


void SendSamplePage::displayStatus(QString message, int now, int max)
{
  m_transmitOperation->update(message, now, max);
  updateStatusDisplay();
}


void SendSamplePage::updateStatusDisplay()
{
  m_progressWidget->update();
}


void SendSamplePage::cleanupPage()
{
}


void SendSamplePage::displayError(QString error)
{
  KMessageBox::error(this, error);
}


bool SendSamplePage::isComplete() const
{
  return m_transmitOperation && m_transmitOperation->isFinished();
}


SendSamplePage::~SendSamplePage()
{
  if (worker) {
    //make sure the thread is done before we delete it
    if (futureWatcher->isRunning()) {
      m_transmitOperation->cancel();
      worker->deleteThatSometime();
    }
    else {
      worker->deleteLater();
    }
  }

  if (futureWatcher)
    futureWatcher->deleteLater();

  if (m_progressWidget) m_progressWidget->deleteLater();
}


void SendSamplePage::initializePage()
{
  /*
   * This really is nothing but annoying
   *
   * if (KMessageBox::questionYesNo(this, i18n("Do you want to send the samples to the server?")) == KMessageBox::Yes) {
    prepareDataSending();
  } else
    kDebug() << "Nothing";
  */
}


void SendSamplePage::prepareDataSending()
{
  if (!SSCDAccessSingleton::getInstance()->isConnected()) {
    if (KMessageBox::questionYesNo(this, i18n("Not connected to the server.\n\nDo you want to establish a connection now?")) == KMessageBox::Yes) {
      //connect
      connect(SSCDAccessSingleton::getInstance(), SIGNAL(connected()), this, SLOT(prepareDataSending()));
      connect(SSCDAccessSingleton::getInstance(), SIGNAL(connected()), this, SLOT(disassociateFromSSCDAccessSingleton()));
      connect(SSCDAccessSingleton::getInstance(), SIGNAL(disconnected()), this, SLOT(disassociateFromSSCDAccessSingleton()));
      connect(SSCDAccessSingleton::getInstance(), SIGNAL(error(const QString&)), this, SLOT(disassociateFromSSCDAccessSingleton()));

      SSCDAccessSingleton::getInstance()->connectTo(SSCConfig::host(), SSCConfig::port(), SSCConfig::useEncryption());
    }
  }
  else
    sendData();
}


void SendSamplePage::disassociateFromSSCDAccess()
{
  disconnect(SSCDAccessSingleton::getInstance(), SIGNAL(connected()), this, SLOT(prepareDataSending()));
  disconnect(SSCDAccessSingleton::getInstance(), SIGNAL(connected()), this, SLOT(disassociateFromSSCDAccessSingleton()));
  disconnect(SSCDAccessSingleton::getInstance(), SIGNAL(disconnected()), this, SLOT(disassociateFromSSCDAccessSingleton()));
  disconnect(SSCDAccessSingleton::getInstance(), SIGNAL(error(const QString&)), this, SLOT(disassociateFromSSCDAccessSingleton()));
}


void SendSamplePage::sendData()
{
  if (!SSCDAccessSingleton::getInstance()->isConnected())
    KMessageBox::information(this, i18n("Not connected to server."));

  kDebug() << "Sending data";
  pbReSendData->setEnabled(false);
  pbStoreData->setEnabled(false);

  setupOperation(i18n("Sending data..."));

  wizard()->button(QWizard::CancelButton)->setEnabled(false);
  wizard()->button(QWizard::BackButton)->setEnabled(false);

  QFuture<bool> future = QtConcurrent::run(worker, &SendSampleWorker::sendSamples);
  futureWatcher->setFuture(future);
  kDebug() << "Thread started";
}


void SendSamplePage::sendSample(Sample *s)
{
  if (!SSCDAccessSingleton::getInstance()->sendSample(s))
    KMessageBox::error(this, i18n("Could not send sample"));
}
