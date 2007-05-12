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

#include "simoninfo.h"
#include "soundbackend.h"
#include "sounddevice.h"
#ifdef linux
#include "alsabackend.h"
#endif
#ifdef __WIN32
#include "directsoundbackend.h"
#endif

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
 *	@todo Implementing all Windows and all Volume related stuff
 */
class SoundControl{
private:
#ifdef linux
	ALSABackend *soundbackend;
#endif
#ifdef __WIN32
	DirectSoundBackend *soundbackend;
#endif
public:
	bool initializeMic(short channels=2, int samplerate=44100);
	bool initializeSpeaker(short channels=2, int samplerate=44100);
	char* capture (int msecs, long unsigned int& size);
	short* capture (int count, int buffersize, long unsigned int& size);
	SoundDeviceList* getDevices();
	bool playback (char* data, int count);
	
	bool close();
	int getVolume();
	void setVolume(int percent);
    SoundControl();

    ~SoundControl();

};

#endif
