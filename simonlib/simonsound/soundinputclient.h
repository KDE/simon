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


#ifndef SIMON_SOUNDINPUTCLIENT_H_4AC60251BE6A419EA1236280815A2AAD
#define SIMON_SOUNDINPUTCLIENT_H_4AC60251BE6A419EA1236280815A2AAD


#include "soundclient.h"
#include "simonsound_export.h"
#include <QList>
class SoundProcessor;


class SIMONSOUND_EXPORT SoundInputClient : public SoundClient {

private:
	QList<SoundProcessor*> processors;

public:
	SoundInputClient(SoundClientFlags options=None);
	virtual ~SoundInputClient();

	void process(const QByteArray& data, qint64 currentTime);
	virtual void processPrivate(const QByteArray& data, qint64 currentTime)=0;

	void registerSoundProcessor(SoundProcessor *p);
};

#endif



