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
#include "sscdaccess.h"
#include "sampledataprovider.h"
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

SendSamplePage::SendSamplePage(SampleDataProvider *dataProvider, QWidget *parent) :
	QWizardPage(parent),
	worker(new SendSampleWorker(dataProvider)),
	layout(new QVBoxLayout()),
	m_transmitOperation(NULL),
	m_progressWidget(NULL)
{
	setTitle(i18n("Transmitting samples..."));

	QLabel *desc = new QLabel(i18n("The recorded samples are now sent to the server.\n\nPlease be patient..."), this);
	desc->setWordWrap(true);
	layout->addWidget(desc);
  pbReSendData = new KPushButton(KIcon("view-refresh"), i18n("Send samples"), this);
  connect(pbReSendData, SIGNAL(clicked()), this, SLOT(prepareDataSending()));
	layout->addWidget(pbReSendData);

	futureWatcher = new QFutureWatcher<bool>(this);
	connect(futureWatcher, SIGNAL(finished()), this, SLOT(transmissionFinished()));
	//make sure we got the last bit
//	connect(futureWatcher, SIGNAL(finished()), this, SLOT(displayStatus()));
	setLayout(layout);

	connect(worker, SIGNAL(error(QString)), this, SLOT(displayError(QString)), Qt::QueuedConnection);
	connect(worker, SIGNAL(status(QString, int, int)), this, SLOT(displayStatus(QString, int, int)), Qt::QueuedConnection);
	connect(worker, SIGNAL(sendSample(Sample*)), this, SLOT(sendSample(Sample*)), Qt::QueuedConnection);
}

void SendSamplePage::transmissionFinished()
{
  kDebug() << "Transmission finished";
	updateStatusDisplay();
	emit completeChanged();

  if (!futureWatcher->result())
    pbReSendData->setEnabled(true);
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
	if (worker) worker->deleteLater();
	if (m_progressWidget) m_progressWidget->deleteLater();
}

void SendSamplePage::initializePage()
{
	if (KMessageBox::questionYesNo(this, i18n("Do you want to send the samples to the server?")) == KMessageBox::Yes) {
    prepareDataSending();
	} else
		kDebug() << "Nothing";
}

/*
 *	void connected();
	void disconnected();

	void error(const QString& errStr);

	void warning(const QString&);
  */


void SendSamplePage::prepareDataSending()
{
  if (!SSCDAccess::getInstance()->isConnected())
  {
    if (KMessageBox::questionYesNo(this, i18n("Not connected to the server.\n\nDo you want to establish a connection now?")) == KMessageBox::Yes) {
      //connect
      connect(SSCDAccess::getInstance(), SIGNAL(connected()), this, SLOT(prepareDataSending()));
      connect(SSCDAccess::getInstance(), SIGNAL(connected()), this, SLOT(disassociateFromSSCDAccess()));
      connect(SSCDAccess::getInstance(), SIGNAL(disconnected()), this, SLOT(disassociateFromSSCDAccess()));
      connect(SSCDAccess::getInstance(), SIGNAL(error(const QString&)), this, SLOT(disassociateFromSSCDAccess()));

      SSCDAccess::getInstance()->connectTo(SSCConfig::host(), SSCConfig::port(), SSCConfig::useEncryption());
    }
  }
  else
    sendData();
}

void SendSamplePage::disassociateFromSSCDAccess()
{
  disconnect(SSCDAccess::getInstance(), SIGNAL(connected()), this, SLOT(prepareDataSending()));
  disconnect(SSCDAccess::getInstance(), SIGNAL(connected()), this, SLOT(disassociateFromSSCDAccess()));
  disconnect(SSCDAccess::getInstance(), SIGNAL(disconnected()), this, SLOT(disassociateFromSSCDAccess()));
  disconnect(SSCDAccess::getInstance(), SIGNAL(error(const QString&)), this, SLOT(disassociateFromSSCDAccess()));
}


void SendSamplePage::sendData()
{
  if (!SSCDAccess::getInstance()->isConnected())
    KMessageBox::information(this, i18n("Not connected to server."));

  kDebug() << "Sending data";
  pbReSendData->setEnabled(false);

  if (m_progressWidget) {
    layout->removeWidget(m_progressWidget);
    m_progressWidget->deleteLater();
  }

  m_transmitOperation = new Operation(QThread::currentThread(), i18n("Transmitting samples"), i18n("Initializing"), 0, 0, false);
  m_progressWidget = new ProgressWidget(m_transmitOperation, ProgressWidget::Large, this);
  connect(worker, SIGNAL(aborted()), m_transmitOperation, SLOT(canceled()), Qt::QueuedConnection);
  connect(worker, SIGNAL(finished()), m_transmitOperation, SLOT(finished()), Qt::QueuedConnection);
  connect(m_transmitOperation, SIGNAL(aborting()), worker, SLOT(abort()), Qt::QueuedConnection);

  layout->addWidget(m_progressWidget);
  m_progressWidget->show();

  QFuture<bool> future = QtConcurrent::run(worker, &SendSampleWorker::sendSamples);
  futureWatcher->setFuture(future);
  kDebug() << "Thread started";
}


void SendSamplePage::sendSample(Sample *s)
{
	if (!SSCDAccess::getInstance()->sendSample(s)) 
		KMessageBox::error(this, i18n("Could not send sample"));
}


/**
 * Does the actual server communication. This will take LONG. This is why this 
 * function should be called from outside the GUI thread. It will communicate 
 * with the rest of the world through the shared transmitOperation object.
 */
bool SendSampleWorker::sendSamples()
{
	shouldAbort = false;
	bool successful = true;

	int i=0;
	int retryAmount = 0;

	if (!m_dataProvider->startTransmission())
	{
		kDebug() << "Couldn't start transmission";

		emit error(i18n("Failed to start the sample transmission.\n\nMost likely this is caused by problems to send the information about the used input devices."));
		shouldAbort = true;
    successful = false;
	}
	kDebug() << "Transmission started...";

	int maxProgress = m_dataProvider->sampleToTransmitCount();

	while (!shouldAbort && (m_dataProvider->hasSamplesToTransmit())) {
		Sample *s = m_dataProvider->getSample();
		emit status(i18n("Sending: %1", s->path()), i, maxProgress);


		QFile f(s->path());
		if (f.open(QIODevice::ReadOnly)) {
			emit sendSample(s);

			kDebug() << "Emit signal";
			if (!SSCDAccess::getInstance()->processSampleAnswer() && (retryAmount < 3))  {
				kDebug() << "Error processing sample";
				if (!SSCDAccess::getInstance()->isConnected())
				{
					shouldAbort = true;
					successful = false;
				}
				else
					emit error(i18n("Server couldn't process sample: %1", SSCDAccess::getInstance()->lastError()));
				retryAmount++;
			} else {
				m_dataProvider->sampleTransmitted();
				retryAmount = 0;
			}
			kDebug() << "Done processing sample";

			//if sample could not be opened we probably skipped this sample; ignore that silently
		} else {
			kDebug() << "File not found";
			m_dataProvider->sampleTransmitted();
		}

		i++;
	}
	kDebug() << "Done";
	m_dataProvider->stopTransmission();
	if (!shouldAbort) {
		emit finished();
	} else 
		emit aborted();

	return successful;
}

SendSampleWorker::~SendSampleWorker()
{
	delete m_dataProvider;
}

