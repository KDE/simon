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
#include <qaudio.h>

class SoundInputClient;

class SimonSoundInput : public QIODevice
{
	Q_OBJECT

	signals:
		void recordingFinished();
		void error(const QString& str);
		void inputStateChanged(QAudio::State state);
		void outputStateChanged(QAudio::State state);

	private:
		SimonSound::DeviceConfiguration m_device;
		QAudioInput *m_input;
		QHash<SoundInputClient*, qint64> m_activeInputClients;
		QHash<SoundInputClient*, qint64> m_suspendedInputClients;

	protected:
		qint64 readData(char *toRead, qint64 maxLen);
		qint64 writeData(const char *toWrite, qint64 len);
	
	private slots:
		void slotInputStateChanged(QAudio::State state);

	public:
		SimonSoundInput(QObject *parent=NULL);
		
		QAudioInput* input() { return m_input; }
		QHash<SoundInputClient*, qint64> activeInputClients() { return m_activeInputClients; }
		QHash<SoundInputClient*, qint64> suspendedInputClients() { return m_suspendedInputClients; }

		void addActive(SoundInputClient* client);

		bool deRegisterInputClient(SoundInputClient* client);

		bool startRecording(SimonSound::DeviceConfiguration& device);
		bool stopRecording();

		void suspend(SoundInputClient*);
		void suspendInput();
		void resumeInput();
		~SimonSoundInput();
};

#endif

