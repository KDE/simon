/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SOUNDCONTROL_H
#define SOUNDCONTROL_H

#include "sounddevice.h"
#include <QList>
#include "simonsound_export.h"


/**
 *	@class SoundControl
 *	@brief The SoundControl provides a layer to the SoundBackends
 *
 *	The SoundControl controls the sound-backends to provide a Control-Point
 *	to communicate with the individual backends without losing the benefit
 *	of plattform independence
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class SIMONSOUND_EXPORT SoundControl{

public:
	SoundDeviceList* getInputDevices();
	SoundDeviceList* getOutputDevices();

	bool checkDeviceSupport(int inputDeviceId, int outputDeviceId, int channels, int samplerate);

#ifdef Q_OS_UNIX
	QString idToALSAName(int deviceId);
#endif

    SoundControl();

    ~SoundControl();

};

#endif
