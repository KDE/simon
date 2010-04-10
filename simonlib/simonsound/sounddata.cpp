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

#include "sounddata.h"

#include <KDebug>
#include <string.h>


/**
 * \brief Constructor
 */
SoundData::SoundData(QObject *parent) :
	QIODevice(parent)
{
	availableChunks.clear();
}


qint64 SoundData::readData(char *toRead, qint64 maxLen)
{
	Q_UNUSED(toRead);
	Q_UNUSED(maxLen);

	kWarning() << "Use getChunk() instead";
	return -1;
}

QByteArray SoundData::getChunk(bool& success)
{
	Q_ASSERT (availableChunks.count() > 0);
	success = true;
	return availableChunks.takeAt(0);
}



qint64 SoundData::writeData(const char *toWrite, qint64 len)
{
	QByteArray chunk;
	chunk.append(toWrite, len);
	availableChunks << chunk;

	emit bytesWritten(len);
	return len;
}


/**
 * \brief Destructor
 */
SoundData::~SoundData()
{
}


