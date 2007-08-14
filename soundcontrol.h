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
#include "RtAudio.h"
#include "RtError.h"
#include <QList>


/**
 *	@class SoundControl
 *	@brief [DEPRCATED] The SoundControl provides a layer to the SoundBackends
 *
 *	The SoundControl controls the sound-backends to provide a Control-Point
 *	to communicate with the individual backends without losing the benefit
 *	of plattform independence
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing all Volume related stuff
 */
class SoundControl{
private:
	RtAudio *audio;

public:
	SoundDeviceList* getInputDevices();
	SoundDeviceList* getOutputDevices();
	QList<int>* getSamplerate(QString id);
	int getChannel(QString id);
	int getVolume();
	void setVolume(int percent);
    SoundControl();

    ~SoundControl();

};

#endif
