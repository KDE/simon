//
// C++ Implementation: soundcontrol
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "soundcontrol.h"
#include <string.h>
#include "simoninfo.h"
#include "logger.h"
/**
 *	@brief Constructor
 *
 *	Creates a new Soundbackend
 *
 *	@author Peter Grasch
 */
SoundControl::SoundControl()
{
	try
	{
		audio = new RtAudio();
	}
	catch ( RtError &error )
	{
		error.printMessage();
		exit ( EXIT_FAILURE );
	}
}



/**
 *	@brief Returns the current volume in percent
 *
 *	@author Peter Grasch
 *	@return int
 *	volume in percent
*/
int SoundControl::getVolume()
{
	return 0;
}

/**
 *	@brief Sets the volume in percent
 *
 *	@author Peter Grasch
 *	@param int percent
 *	volume in percent
*/


void SoundControl::setVolume ( int percent )
{
}

/**
 *	@brief Sets the volume in percent
 *
 *	@author Gigerl Martin
 *	@param int percent
 *	returns the aviable sound devices
*/

SoundDeviceList* SoundControl::getOutputDevices()
{
	Logger::log("Getting list of available output devices");
	SoundDeviceList *sdl= new SoundDeviceList();

	// Determine the number of devices available
	int devices = audio->getDeviceCount();

	// Scan through devices for various capabilities
	RtAudioDeviceInfo info;
	for ( int i=1; i<=devices; i++ )
	{

		try
		{
			info = audio->getDeviceInfo ( i );
			if ( info.outputChannels>0 )
			{
				sdl->append ( SoundDevice ( QString::number ( i ),QString ( info.name.c_str() ) ) );
			}
		}
		catch ( RtError &error )
		{
			error.printMessage();
			break;
		}
	}

	return sdl;
}

QList<int>* SoundControl::getSamplerate ( QString id )
{
	Logger::log("Determining samplerate of device \""+id+"\"");
	// Determine the number of devices available
	int devices = audio->getDeviceCount();
	// Scan through devices for various capabilities
	RtAudioDeviceInfo info;
	for ( int i=1; i<=devices; i++ )
	{
		try
		{
			info = audio->getDeviceInfo ( i );
			if ( QString ( info.name.c_str() ) ==id )
			{
				QList<int>* temp= new QList<int>();

				for ( int i=0; i<info.sampleRates.size();i++ )
				{

					temp->append ( info.sampleRates.at ( i ) );

				}
				return temp;


			}
		}
		catch ( RtError &error )
		{
			error.printMessage();
			break;
		}
	}
	return NULL;
}


int SoundControl::getChannel ( QString id )
{
	Logger::log("Returning available channels of device \""+id+"\"");
	// Determine the number of devices available
	int devices = audio->getDeviceCount();
	// Scan through devices for various capabilities
	RtAudioDeviceInfo info;
	for ( int i=1; i<=devices; i++ )
	{
		try
		{
			info = audio->getDeviceInfo ( i );
			if ( QString ( info.name.c_str() ) ==id )
			{
				return info.inputChannels;

			}
		}
		catch ( RtError &error )
		{
			error.printMessage();
			break;
		}
	}
	return 0;

}


SoundDeviceList* SoundControl::getInputDevices()
{
	Logger::log("Getting list of available input devices");
	SoundDeviceList *sdl= new SoundDeviceList();

	// Determine the number of devices available
	int devices = audio->getDeviceCount();

	// Scan through devices for various capabilities
	RtAudioDeviceInfo info;
	for ( int i=1; i<=devices; i++ )
	{
		try
		{
			info = audio->getDeviceInfo ( i );
			if ( info.inputChannels>0 )
			{
				sdl->append ( SoundDevice ( QString::number ( i ),QString ( info.name.c_str() ) ) );
			}
		}
		catch ( RtError &error )
		{
			error.printMessage();
			break;
		}
	}


	return sdl;

}

/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
*/
SoundControl::~SoundControl()
{
// 	delete this->soundbackend;
// 	delete in_audio;
// 	delete out_audio;
}
