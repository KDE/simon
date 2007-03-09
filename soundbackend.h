//
// C++ Interface: soundbackend
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SOUNDBACKEND_H
#define SOUNDBACKEND_H


/**
 *	@interface SoundBackend
 *	@brief Interface to define the set of commands to implement 
 *
 *	The interface defines a fixed set of commands needed to get
 *	a fully functional soundbackend
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */
class SoundBackend{
public:
    virtual bool openDevice( const char* deviceID ) = 0;
    virtual bool setSampleRate( int sampleRate ) = 0;
    virtual bool setChannels( short channels ) = 0;
    virtual bool setInterleaved( bool interleaved ) = 0;
    virtual bool closeDevice() = 0;
    virtual bool prepareDevice() = 0;
    virtual short **readData( short count, short buffersize ) = 0;
    virtual int getVolume() = 0;
    virtual void setVolume( int percent ) = 0;

};

#endif
