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


#include "simonsoundinput.h"
#include <simonsound/soundinputclient.h>

#include <QAudioInput>
#include <KDebug>

SimonSoundInput::SimonSoundInput(QAudioInput *input, QObject *parent) : QObject(parent),
	m_input(input)
{
}

void SimonSoundInput::suspend(SoundInputClient* client)
{
	client->suspend();
	m_suspendedInputClients.insert(client, m_activeInputClients.value(client));
	m_activeInputClients.remove(client);
}

void SimonSoundInput::addActive(SoundInputClient* client)
{
	m_activeInputClients.insert(client, 0);
}


bool SimonSoundInput::deRegisterInputClient(SoundInputClient* client)
{
	kDebug() << "Deregistering input client";

	if (m_activeInputClients.remove(client) == 0)
	{
		//wasn't active anyways
		m_suspendedInputClients.remove(client);
		return true;
	}
	if (client->isExclusive())
	{
		QHashIterator<SoundInputClient*, qint64> j(m_suspendedInputClients);

		/// if we have one exclusive input in the suspended list move it to the active
		/// list and ignore the rest
		///
		/// @note The order in which the input clients are resumed is undefined
		///
		///otherwise move everything back
		
		bool hasExclusive = false;
		while (j.hasNext())
		{
			j.next();
			if (j.key()->isExclusive())
			{
				m_activeInputClients.insert(j.key(), j.value());
				m_suspendedInputClients.remove(j.key());
				hasExclusive = true;
				break;
			}
		}
		if (!hasExclusive)
		{
			j.toFront();
			while (j.hasNext())
			{
				j.next();
				j.key()->resume();
				m_activeInputClients.insert(j.key(), j.value());
				m_suspendedInputClients.remove(j.key());
			}
		}
	}

	bool success = true;

	if (m_activeInputClients.isEmpty()) //don't need to record any longer
	{
		//then stop recording
		kDebug() << "No active clients available... Stopping recording";
		success = stopRecording();
		if (success)
			deleteLater(); // destroy this sound input
	}

	return success;
}


bool SimonSoundInput::stopRecording()
{
	kDebug() << "Stopping recording";
	if (!m_input) return true;

	m_input->disconnect(this);
	m_input->stop();
	delete m_input;
	m_input = NULL;

	return true;
}

void SimonSoundInput::suspendInput()
{
	if (!m_input) return;
	m_input->suspend();
}

void SimonSoundInput::resumeInput()
{
	if (!m_input) return;
	m_input->resume();
}
	
SimonSoundInput::~SimonSoundInput()
{
	if (m_input)
		m_input->deleteLater();
}

