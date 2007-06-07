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

#include <stdio.h>
#include "simoninfo.h"
#include "sounddevice.h"
#include "RtAudio.h"
#include "RtError.h"

#ifndef FORMAT
#define FORMAT RTAUDIO_SINT16
#endif

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
 *	
 *	@note AVOID USAGE! DEPRCATED!
 */
class SoundControl{
private:
	RtAudio *out_audio;
	RtAudio *in_audio;
	char *in_data;
	char *out_data;
	int in_buffersize, out_samplerate, in_samplerate, out_buffersize;
	int in_channels, out_channels;
public:
	bool initializeMic(short channels=2, int samplerate=44100);
	bool initializeSpeaker(short channels=2, int samplerate=44100);
	char* capture (int msecs, long unsigned int& size);
	SoundDeviceList* getInputDevices();
	SoundDeviceList* getOutputDevices();
	bool playback (char* data, int count);
	
	bool closeSpeaker();
	bool closeMic();
	int getVolume();
	void setVolume(int percent);
    SoundControl();

    ~SoundControl();

};

#endif
