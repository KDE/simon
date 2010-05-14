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


#include "simonsoundoutput.h"

#include <simonsound/soundoutputclient.h>
#include <QAudioOutput>
#include <KDebug>

SimonSoundOutput::SimonSoundOutput(QAudioOutput *output, QObject *parent) : QObject(parent),
	m_output(output)
{
}
		
SimonSoundOutput::~SimonSoundOutput()
{
	m_output->deleteLater();
}

void SimonSoundOutput::suspendOutput()
{
	if (!m_output) return;
	m_output->suspend();
}

void SimonSoundOutput::resumeOutput()
{
	if (!m_output) return;
	m_output->suspend();
}

bool SimonSoundOutput::stopPlayback()
{
	kDebug() << "Stop playback...";
	if (!m_output) return true;

	m_output->stop();
	m_output->disconnect(this);
	m_output->deleteLater();
	m_output = NULL;

	//FIXME
//	reset();

	return true;
}

