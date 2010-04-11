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

#include "soundinputclient.h"
#include "soundprocessor.h"
#include <QByteArray>

/**
 * \brief Constructor
 */
SoundInputClient::SoundInputClient(SoundClient::SoundClientFlags options) :
	SoundClient(options)
{
}

void SoundInputClient::registerSoundProcessor(SoundProcessor *p)
{
	processors << p;
}

void SoundInputClient::process(const QByteArray& data, qint64 currentTime)
{
	QByteArray processedData = data;
	foreach (SoundProcessor* p, processors)
	{
		p->process(processedData);
		if (processedData.isEmpty()) return;
	}

	processPrivate(processedData, currentTime);
}

/**
 * \brief Destructor
 */
SoundInputClient::~SoundInputClient()
{
	qDeleteAll(processors);
}





