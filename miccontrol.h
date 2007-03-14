//
// C++ Interface: miccontrol
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MICCONTROL_H
#define MICCONTROL_H

#include "simoninfo.h"
#include "soundbackend.h"

#ifdef linux
#include "alsabackend.h"
#endif

/**
 *	@class MicControl
 *	@brief The MicControl provides a layer to the SoundBackends
 *
 *	The MicControl controls the sound-backends to provide a Control-Point
 *	to communicate with the individual backends without losing the benefit
 *	of plattform indepandance
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing all Windows and all Volume related stuff
 */
class MicControl{
#ifdef linux
	ALSABackend *soundbackend;
#endif
public:
	bool initializeMic(short channels=2, int samplerate=44100);
	short** capture (int count, short buffersize);
	bool closeMic();
	int getVolume();
	void setVolume(int percent);
    MicControl();

    ~MicControl();

};

#endif
