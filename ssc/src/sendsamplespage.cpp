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
	statusWatcher(new QTimer()),
	layout(new QVBoxLayout()),
	m_transmitOperation(NULL),
	m_progressWidget(NULL),
	m_files(files),
	m_prompts(prompts),
	m_sampleType(sampleType)

{

	setTitle(i18n("Transmitting %1 samples...", files.count()));

	QLabel *desc = new QLabel(i18n("The recorded samples are now sent to the server.\n\nPlease be patient..."), this);
	desc->setWordWrap(true);
	layout->addWidget(desc);
	futureWatcher = new QFutureWatcher<bool>(this);
	connect(futureWatcher, SIGNAL(finished()), this, SIGNAL(completeChanged()));
	connect(futureWatcher, SIGNAL(finished()), statusWatcher, SLOT(stop()));
	//make sure we got the last bit
	connect(futureWatcher, SIGNAL(finished()), this, SLOT(displayStatus()));
	connect(statusWatcher, SIGNAL(timeout()), this, SLOT(displayStatus()));
	setLayout(layout);

	connect(this, SIGNAL(error(QString)), this, SLOT(displayError(QString)), Qt::QueuedConnection);
}

void SendSamplePage::displayStatus()
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

		layout->addWidget(m_progressWidget);
		m_progressWidget->show();

		QFuture<bool> future = QtConcurrent::run(this, &SendSamplePage::sendSamples);
		futureWatcher->setFuture(future);
		statusWatcher->start(330);
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
bool SendSamplePage::sendSamples()
{
	int maxProgress = m_files.count();

	int i=0;
	while (m_transmitOperation->isRunning() && (m_files.count() > i)) {
		QString currentFile = SSCConfig::sampleDirectory()+m_files.at(i)+".wav";
		QString currentPrompt = m_prompts.at(i);

		m_transmitOperation->update(i18n("Sending: %1", currentFile), i, maxProgress);


		QFile f(currentFile);
		if (f.open(QIODevice::ReadOnly)) {
			if (!SSCDAccess::getInstance()->sendSample(m_userId, m_sampleType, currentPrompt, f.readAll())) 
				emit error(i18n("Server couldn't process sample: %1", SSCDAccess::getInstance()->lastError()));

			//if sample could not be opened we probably skipped this sample; ignore that silently
		} else kDebug() << "File not found";

		i++;
	}
	if (m_transmitOperation->isRunning()) {
		m_transmitOperation->finished();
		kDebug() << "Sent finished";
	} else 
		m_transmitOperation->canceled();

	m_transmitOperation->update(i18n("Done"), maxProgress, maxProgress);
	return true;
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
	statusWatcher->deleteLater();
}
