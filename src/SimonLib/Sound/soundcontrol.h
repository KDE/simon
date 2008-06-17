//
// C++ Interface: soundcontrol
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SOUNDCONTROL_H
#define SOUNDCONTROL_H

#include "sounddevice.h"
#include <QList>


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
class SoundControl{

public:
	SoundDeviceList* getInputDevices();
	SoundDeviceList* getOutputDevices();

	bool checkDeviceSupport(int inputDeviceId, int outputDeviceId, int channels, int samplerate);

    SoundControl();

    ~SoundControl();

};

#endif
