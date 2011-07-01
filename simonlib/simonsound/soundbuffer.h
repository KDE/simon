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

#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H

#include <QThread>
#include <QByteArray>
#include <QMutex>
#include "qsemaphore2.h"

#define BUFFER_MAX_LENGTH 4*8192

class SoundBuffer : public QThread
{

public:
    explicit SoundBuffer(QObject* parent = 0);

protected:
    QMutex killLock;
    bool m_shouldBeRunning;
    QByteArray m_buffer;
    QSemaphore2 m_bufferLock;
};

#endif // SOUNDBUFFER_H
