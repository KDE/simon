//
// C++ Interface: alsabackend
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ALSABACKEND_H
#define ALSABACKEND_H

#define ALSA_PCM_NEW_HW_PARAMS_API

#include "soundbackend.h"
#include <iostream>
#include <math.h>

#include <alsa/asoundlib.h>


/**
 *	@class ALSABackend
 *	@brief The ALSA Backend - implements the SoundInterface
 *
 *	The ALSA Backend provides another Layer, used from the plattform-indepentant
 *	MicControl to provide an interface to the low level system calls
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing the Volume functions
 */
class ALSABackend : public SoundBackend 
{

private:
	snd_pcm_t *handle; //!< The handle
	snd_pcm_hw_params_t *hw_params; //!< Stores all config options for the handle
	
	int dir; //!< I have NO idea
	snd_pcm_uframes_t frames; //!< holds the number of frames
	unsigned int sampleRate; //!< we seem to need this again later on
	
public:
	bool openDevice( const char* deviceID, int mode=READ );
	bool setSampleRate( int sampleRate );
	bool setChannels( short channels );
	bool setInterleaved( bool interleaved );
	bool closeDevice();
	void writeData ( char* data, long unsigned int length, int buffersize=255 );
	bool prepareDevice();
	char* readData( int msecs, long unsigned int& length );
	short* readData( int count, int buffersize, long unsigned int& length );
	int getVolume();
	void setVolume( int percent );
	
	ALSABackend();
	~ALSABackend();

};

#endif
