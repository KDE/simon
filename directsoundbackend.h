//
// C++ Interface: directsoundbackend
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DIRECTSOUNDBACKEND_H
#define DIRECTSOUNDBACKEND_H

/**
 *	@class DirectSoundBackend
 *	@brief The DirectSound Backend - implements the SoundInterface
 *
 *	The DirectSound Backend provides another Layer, used from the plattform-indepentant
 *	MicControl to provide an interface to the low level system calls
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */
class DirectSoundBackend{
public:
    DirectSoundBackend();

    ~DirectSoundBackend();

};

#endif
