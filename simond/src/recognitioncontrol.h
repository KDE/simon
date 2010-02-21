/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_RECOGNITIONCONTROL_H_F4CB407566D6459487054C2B03311E38
#define SIMON_RECOGNITIONCONTROL_H_F4CB407566D6459487054C2B03311E38

#include <QObject>
#include <QThread>
#include <QList>
#include <QDateTime>
#include <QMetaType>
#include <simonrecognitionresult/recognitionresult.h>

class RecognitionControl : public QThread
{
	Q_OBJECT

	private:
		QDateTime m_lastSuccessfulStart;
	signals:
		void recognitionReady();
		void recognitionError(const QString& error, const QByteArray& buildLog);
		void recognitionWarning(const QString& warning);
		void recognitionAwaitingStream(qint32 port, qint32 sampleRate);
		void recognitionStarted();
		void recognitionStopped();
		void recognitionPaused();
		void recognitionResumed();
		void recognitionResult(const RecognitionResultList& recognitionResults);

	protected:
		void touchLastSuccessfulStart();

		QString username;
		qint32 sampleRate;

		static qint32 portNum;
		static qint32 reservePortNum() { 
			if (portNum >= 10000)
				portNum = 5000;
			return portNum++;
		}

		static qint32 getPortNum() { return portNum; }

	public:
		QDateTime lastSuccessfulStart() { return m_lastSuccessfulStart; }
		RecognitionControl(const QString& username, QObject *parent=0);

		virtual bool initializeRecognition(bool isLocal)=0;
		virtual bool isInitialized()=0;

		virtual void stop()=0;
		virtual void pause()=0;
		virtual void resume()=0;
			       
		~RecognitionControl();
		
};

#endif
