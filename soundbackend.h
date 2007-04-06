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

#define READ 0
#define WRITE 1

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
    virtual bool openDevice( const char* deviceID, int mode=READ ) = 0;
    virtual bool setSampleRate( int sampleRate ) = 0;
    virtual bool setChannels( short channels ) = 0;
    virtual bool setInterleaved( bool interleaved ) = 0;
    virtual bool closeDevice() = 0;
    virtual bool prepareDevice() = 0;
    virtual char* readData( int msecs, long unsigned int& length ) = 0;
    virtual short* readData( int count, int buffersize, long unsigned int& length ) = 0;
    virtual void writeData ( char* data, long unsigned int length, int buffersize ) = 0;
    virtual int getVolume() = 0;
    virtual void setVolume( int percent ) = 0;

};

#endif
