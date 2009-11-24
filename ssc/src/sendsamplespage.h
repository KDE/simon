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


#ifndef SENDSAMPLEPAGE_H
#define SENDSAMPLEPAGE_H

#include "trainingswizard.h"
#include <QString>
#include <QWizardPage>
#include <QFutureWatcher>
#include <QTimer>

class Operation;
class QVBoxLayout;
class ProgressWidget;

class SendSamplePage : public QWizardPage
{
	Q_OBJECT
	
	signals:
		void error(QString);

	private:
		qint32 m_userId;
		QTimer *statusWatcher;

		QVBoxLayout *layout;
		Operation *m_transmitOperation;
		ProgressWidget *m_progressWidget;
		QStringList m_files;
		QStringList m_prompts;
		TrainingsWizard::TrainingsType m_sampleType;

		QFutureWatcher<bool> *futureWatcher;

		bool sendSamples();
	
	private slots:
		void displayStatus();
		void displayError(QString error);
	
	public:
		SendSamplePage(qint32 userId, TrainingsWizard::TrainingsType sampleType, const QStringList& files, const QStringList& prompts, QWidget *parent=0);
		~SendSamplePage();
		bool isComplete() const;

		void initializePage();
		void cleanupPage();

};

#endif
