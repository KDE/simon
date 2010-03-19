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


#ifndef SIMON_SENDSAMPLEPAGE_H_7C93ADEB2453450C88A2A4C90F5D82F1
#define SIMON_SENDSAMPLEPAGE_H_7C93ADEB2453450C88A2A4C90F5D82F1

#include "trainingswizard.h"
#include <QString>
#include <QWizardPage>
#include <QFutureWatcher>
#include <QTimer>

class Operation;
class QVBoxLayout;
class ProgressWidget;

class SendSampleWorker : public QObject
{
	Q_OBJECT

	signals:
		void status(QString, int now, int max);
		void error(QString);
		void aborted();
		void finished();
		void sendSample(qint32 userId, qint32 sampleType, const QString& currentPrompt, const QByteArray& data);

	private:
		bool shouldAbort;
		qint32 m_userId;
		QStringList m_files;
		QStringList m_prompts;
		TrainingsWizard::TrainingsType m_sampleType;

	private slots:
		void abort() { shouldAbort = true; }

	public:
		SendSampleWorker(qint32 userId, TrainingsWizard::TrainingsType sampleType, const QStringList& files, const QStringList& prompts, QObject *parent) : QObject(parent),
			shouldAbort(false),
			m_userId(userId),
			m_files(files),
			m_prompts(prompts),
			m_sampleType(sampleType)
		{}
		
		bool sendSamples();
};

class SendSamplePage : public QWizardPage
{
	Q_OBJECT
	
	signals:
		void error(QString);

	private:
		qint32 m_userId;

		SendSampleWorker *worker;

		QVBoxLayout *layout;
		Operation *m_transmitOperation;
		ProgressWidget *m_progressWidget;

		QFutureWatcher<bool> *futureWatcher;

	
	private slots:
		void displayStatus(QString message, int now, int max);
		void displayError(QString error);
		void sendSample(qint32 userId, qint32 sampleType, const QString& currentPrompt, const QByteArray& data);
		void updateStatusDisplay();
	
	public:
		SendSamplePage(qint32 userId, TrainingsWizard::TrainingsType sampleType, const QStringList& files, const QStringList& prompts, QWidget *parent=0);
		~SendSamplePage();
		bool isComplete() const;

		void initializePage();
		void cleanupPage();

};

#endif
