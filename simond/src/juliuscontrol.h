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
 *
 *
 *   Powered By:
 *
 *   Large Vocabulary Continuous Speech Recognition Engine Julius
 *   Copyright (c) 1997-2000 Information-technology Promotion Agency, Japan
 *   Copyright (c) 1991-2008 Kawahara Lab., Kyoto University
 *   Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 *   Copyright (c) 2005-2008 Julius project team, Nagoya Institute of Technology
 *
 */

#ifndef JULIUSCONTROL_H
#define JULIUSCONTROL_H

#include <QList>
#include <QPointer>
#include <QMutex>
#include <KDebug>
#include "recognitioncontrol.h"

#ifdef bzero
#undef bzero
#endif

#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif

extern "C" {
	#include <julius/julius.h>
}

class JuliusControl : public RecognitionControl
{
	Q_OBJECT
		
	public:
		enum Request {
			None=0,
			Stop=2,
			Pause=3
		};
		
		JuliusControl(const QString& username, QObject *parent=0);

		bool initializeRecognition(bool isLocal);
		void stop();
		void pause();
		void resume();
		bool isInitialized();
		
		bool isStopping() { return stopping; }
		
		void waitForResumed();
		void recognized(const QList<RecognitionResult>& recognitionResults);
		JuliusControl::Request popNextRequest();
		

		Jconf* setupJconf();
      
		~JuliusControl();
		qint32 getPort();

	protected:
		void run();
		void pushRequest(JuliusControl::Request);
		void uninitialize();

	private:
		qint32 reservedPort;
		Recog *recog;
		Jconf *jconf;
		bool isLocal;
		bool stopping;
		QMutex pauseMutex;
		bool m_initialized;
		bool shouldBeRunning;
		
		QList<JuliusControl::Request> nextRequests;
		
};

#endif
