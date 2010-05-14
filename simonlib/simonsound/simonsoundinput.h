/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_SIMONSOUNDINPUT_H_BAC62651BE6A419EA6156220815A2AAD
#define SIMON_SIMONSOUNDINPUT_H_BAC62651BE6A419EA6156220815A2AAD

class QAudioInput;

#include <simonsound/simonsound.h>
#include <QHash>
#include <QObject>

class SoundInputClient;

class SimonSoundInput : public QObject
{
	Q_OBJECT

	private:
		QAudioInput *m_input;
		QHash<SoundInputClient*, qint64> m_activeInputClients;
		QHash<SoundInputClient*, qint64> m_suspendedInputClients;

	public:
		SimonSoundInput(QAudioInput *input, QObject *parent=NULL);
		
		QAudioInput* input() { return m_input; }
		QHash<SoundInputClient*, qint64> activeInputClients() { return m_activeInputClients; }
		QHash<SoundInputClient*, qint64> suspendedInputClients() { return m_suspendedInputClients; }

		void addActive(SoundInputClient* client);

		bool deRegisterInputClient(SoundInputClient* client);
		bool stopRecording();

		void suspend(SoundInputClient*);
		void suspendInput();
		void resumeInput();
		~SimonSoundInput();
};

#endif

