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

#ifndef JULIUSCONTROL_H
#define JULIUSCONTROL_H

#include <QList>
#include <QPointer>
#include <QMutex>
#include <KDebug>
#include "recognitioncontrol.h"

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
		
		void stopped();
		
		void waitForResumed();
		void recognized(const QString& sequence, const QString& sampa, const QString& samparaw);
		JuliusControl::Request popNextRequest();
		

		Jconf* setupJconf();
      
		~JuliusControl();
		
	protected:
		void run();
		void pushRequest(JuliusControl::Request);

	private:
		Recog *recog;
		Jconf *jconf;
		bool isLocal;
		QMutex pauseMutex;
		
		QList<JuliusControl::Request> nextRequests;
		
};

#endif
