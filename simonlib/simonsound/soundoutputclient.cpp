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

#include "soundoutputclient.h"

/**
 * \brief Constructor
 */
SoundOutputClient::SoundOutputClient() :
	SoundClient(SoundClient::Exclusive)
{
}


/**
 * \brief Destructor
 */
SoundOutputClient::~SoundOutputClient()
{
}



/*
SoundOutputClientWrapper::SoundOutputClientWrapper(QObject *parent, SoundOutputClient *client) :
	QIODevice(parent),
	m_client(client),
	m_currentStreamTime(0)
{
}

qint64 SoundOutputClientWrapper::readData(char *data, qint64 maxlen)
{
	return m_client->readData(data, maxlen, 0);
}

qint64 SoundOutputClientWrapper::writeData(const char *data, qint64 len)
{
	Q_UNUSED(data);
	Q_UNUSED(len);
	return -1;
}
*/



