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
#include <sscdaccess/sscdaccess.h>
#include <simonprogresstracking/operation.h>
#include <simonprogresstracking/progresswidget.h>

#include <QDir>
#include <QLabel>
#include <QtConcurrentRun>

#include <KLocalizedString>
#include <KMessageBox>
#include <QThread>
#include <QVBoxLayout>
#include <QFuture>
#include <QFutureWatcher>


SendSamplePage::SendSamplePage(qint32 userId, TrainingsWizard::TrainingsType sampleType, const QStringList& files, const QStringList& prompts, QWidget *parent) : QWizardPage(parent), 
	m_userId(userId),
	worker(new SendSampleWorker(userId, sampleType, files, prompts, parent)),
	layout(new QVBoxLayout()),
	m_transmitOperation(NULL),
	m_progressWidget(NULL)
{

	setTitle(i18n("Transmitting samples..."));

	QLabel *desc = new QLabel(i18n("The recorded samples are now sent to the server.\n\nPlease be patient..."), this);
	desc->setWordWrap(true);
	layout->addWidget(desc);
	futureWatcher = new QFutureWatcher<bool>(this);
	connect(futureWatcher, SIGNAL(finished()), this, SIGNAL(completeChanged()));
	connect(futureWatcher, SIGNAL(finished()), this, SLOT(updateStatusDisplay()));
	//make sure we got the last bit
//	connect(futureWatcher, SIGNAL(finished()), this, SLOT(displayStatus()));
	setLayout(layout);

	connect(worker, SIGNAL(error(QString)), this, SLOT(displayError(QString)), Qt::QueuedConnection);
	connect(worker, SIGNAL(status(QString, int, int)), this, SLOT(displayStatus(QString, int, int)), Qt::QueuedConnection);
	connect(worker, SIGNAL(sendSample(qint32, qint32, const QString&, const QByteArray&)), this, SLOT(sendSample(qint32, qint32, const QString&, const QByteArray&)), Qt::QueuedConnection);
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

void SendSamplePage::initializePage()
{
	if (KMessageBox::questionYesNo(this, i18n("Do you want to start sending the samples?"))) {
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
	}
}

void SendSamplePage::cleanupPage()
{
}


/**
 * Does the actual server communication. This will take LONG. This is why this 
 * function should be called from outside the GUI thread. It will communicate 
 * with the rest of the world through the shared transmitOperation object.
 */
bool SendSampleWorker::sendSamples()
{
	shouldAbort = false;
	int maxProgress = m_files.count();

	int i=0;
	while (!shouldAbort && (m_files.count() > i)) {
		QString currentFile = SSCConfig::sampleDirectory()+m_files.at(i)+".wav";
		QString currentPrompt = m_prompts.at(i);

		//m_transmitOperation->update(i18n("Sending: %1", currentFile), i, maxProgress);
		emit status(i18n("Sending: %1", currentFile), i, maxProgress);


		QFile f(currentFile);
		if (f.open(QIODevice::ReadOnly)) {
			emit sendSample(m_userId, m_sampleType, currentPrompt, f.readAll());
			kDebug() << "Emitt signal";
			if (!SSCDAccess::getInstance()->processSampleAnswer())  {
				kDebug() << "Error processing sample";
				emit error(i18n("Server couldn't process sample: %1", SSCDAccess::getInstance()->lastError()));
			}
			kDebug() << "Done processing sample";

			//if sample could not be opened we probably skipped this sample; ignore that silently
		} else kDebug() << "File not found";

		i++;
	}
	kDebug() << "Done";
	if (!shouldAbort) {
		emit finished();
	} else 
		emit aborted();

	//m_transmitOperation->update(i18n("Done"), maxProgress, maxProgress);
	return true;
}
void SendSamplePage::sendSample(qint32 userId, qint32 sampleType, const QString& currentPrompt, const QByteArray& data)
{
	if (!SSCDAccess::getInstance()->sendSample(userId, sampleType, currentPrompt, data)) 
		KMessageBox::error(this, i18n("Could not send sample"));
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
	if (m_progressWidget) m_progressWidget->deleteLater();
	worker->deleteLater();
}
