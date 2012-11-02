/*   Copyright (C) 2010 Grasch Peter <peter.grasch@bedahr.org>
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

#ifndef SOUNDINPUTBUFFER_H
#define SOUNDINPUTBUFFER_H
#include "soundbuffer.h"

class SimonSoundInput;
class SoundInputBuffer : public SoundBuffer
{
private:
  SimonSoundInput *m_input;
  char *m_buffer;
  int m_bufferLength;
  QMutex m_bufferAllocLock;
  QSemaphore2 m_bufferLock;
  
public:
  SoundInputBuffer(SimonSoundInput* input);
  ~SoundInputBuffer();
  void write(const char *toWrite, qint64 len);
  void stop();
  virtual void run();
};

#endif // SOUNDINPUTBUFFER_H
