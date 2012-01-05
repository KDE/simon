/*   Copyright (C) 2010 Grasch Peter <grasch@simon-listens.org>
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

#include "soundoutputbuffer.h"
#include "simonsoundoutput.h"
#include <QMutexLocker>
#include <KDebug>

SoundOutputBuffer::SoundOutputBuffer(SimonSoundOutput* output): SoundBuffer(output),
  m_output(output)
{
}

qint64 SoundOutputBuffer::read(char* data, qint64 maxLen)
{
  QMutexLocker l(&bufferLock);
  if (m_buffer.isEmpty()) {
    //return zeros to keep stream going while we sort out the next client or
    //stop the stream
    kDebug() << "Buffer is empty, popping client!";
    m_output->popClient();
    memset(data, 0, maxLen);
    return maxLen;
  }

  int realCpy = qMin((int) maxLen, m_buffer.size());
  memcpy(data, m_buffer.constData(), realCpy);
  m_buffer.remove(0,realCpy);
  
  return realCpy;
}

void SoundOutputBuffer::run()
{
  bufferLock.lock();
  m_buffer.fill(0, qMin(2*8192, BUFFER_MAX_LENGTH));
  bufferLock.unlock();
  while (m_shouldBeRunning)
  {
    killLock.lock();

    int bufferSize = m_output->bufferSize();
    int bufferLength = m_output->bufferTime();
    if (m_buffer.size() < BUFFER_MAX_LENGTH) {
      //kDebug() << "Buffer not full";
      //fill buffer
      QByteArray currentData = m_output->requestData(bufferSize);
      //kDebug() << "Got " << currentData.size() << " bytes of data from clients";

      //if currentData is empty, just keep going as read() will pop the
      //output client as soon as the buffer is empty to make sure that
      //the current active output is really the current active output
      bufferLock.lock();
      m_buffer += currentData;
      bufferLock.unlock();
    }
    killLock.unlock();

    //either half of the buffered time or 50 milliseconds, whichever is shorter
    int sleepDuration = qMin((bufferLength*100)/2, 50000);
    //kDebug() << "Sleeping: " << sleepDuration << " microseconds; Buffersize: " << bufferSize << bufferLength;
    usleep(sleepDuration);
  }
  kWarning() << "Left run loop";
}

void SoundOutputBuffer::stop()
{
  kWarning() << "Locking killlock.";
  killLock.lock();

  m_shouldBeRunning = false;
  kWarning() << "Set should be running, calling quit.";
  quit();
  kWarning() << "Unlocking killLock..";

  killLock.unlock();
  wait();
}

SoundOutputBuffer::~SoundOutputBuffer()
{
}
