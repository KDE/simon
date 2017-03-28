/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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

#include "soundbackend.h"

#ifdef Q_OS_LINUX
#include "alsa/alsabackend.h"
#endif
#ifdef Q_OS_MAC
#include "coreaudio/coreaudiobackend.h"
#endif
#ifdef Q_OS_WIN32
#include "directsound/directsoundbackend.h"
#endif

SoundBackend::SoundBackend() : m_client(0), m_error(SimonSound::NoError), m_state(SimonSound::IdleState)
{
  connect(this, SIGNAL(errorOccured(SimonSound::Error)), this, SLOT(errorSlot(SimonSound::Error)));
  connect(this, SIGNAL(stateChanged(SimonSound::State)), this, SLOT(stateSlot(SimonSound::State)));
}

SoundBackend* SoundBackend::createObject()
{
#ifdef Q_OS_LINUX
  return new ALSABackend();
#endif
#ifdef Q_OS_MAC
  return new CoreAudioBackend();
#endif
#ifdef Q_OS_WIN32
  return new DirectSoundBackend();
#endif
}

void SoundBackend::errorSlot(SimonSound::Error err)
{
  m_error = err;
}

void SoundBackend::stateSlot(SimonSound::State state)
{
  m_state = state;
}

SimonSound::Error SoundBackend::error()
{
  return m_error;
}

SimonSound::State SoundBackend::state()
{
  return m_state;
}

