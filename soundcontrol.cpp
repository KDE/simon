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
#include <QObject>
#include <QDebug>
/**
 *	@brief Constructor
 *
 *	Creates a new Soundbackend
 *
 *	@author Peter Grasch
 */
SoundControl::SoundControl()
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
	Logger::log(QObject::tr("[INF] Bekommen einer Liste mit den verfügbaren devices"));
	SoundDeviceList *sdl= new SoundDeviceList();
	RtAudio audio;
	
	// Determine the number of devices available
	unsigned int devices = audio.getDeviceCount();
	
	
	// Scan through devices for various capabilities
	RtAudio::DeviceInfo info;
	for ( unsigned int i=0; i<devices+1; i++ )
	{
		try {
			info = audio.getDeviceInfo ( i );
		} catch (RtError &e)
		{
// 			e.printMessage();
		}
	
		if ( info.probed == true )
		{
			if (info.outputChannels > 0)
			{
				QList<int> supportedChannels;
				QList<int> sampleRates;
				for (unsigned int j=1; j <= info.outputChannels; j++)
				{
					supportedChannels << j;
				}
				for (unsigned int j=0; j < info.sampleRates.size(); j++)
					sampleRates << info.sampleRates[j];
				sdl->append(SoundDevice(i, info.name.c_str(), supportedChannels, sampleRates));
			}
		}
	}

	return sdl;
}


/**
 * \author Gigerl Martin
 * \brief Returns the available input devices
 * @return The list of found Sounddevices
 */
SoundDeviceList* SoundControl::getInputDevices()
{
	Logger::log(QObject::tr("[INF] Bekommen einer Liste mit den verfügbaren Input-devices"));
	SoundDeviceList *sdl= new SoundDeviceList();
	RtAudio audio;
	
	// Determine the number of devices available
	unsigned int devices = audio.getDeviceCount();
	
	// Scan through devices for various capabilities
	RtAudio::DeviceInfo info;
	for ( unsigned int i=0; i<devices+1; i++ )
	{
		try {
			info = audio.getDeviceInfo ( i );
		} catch (RtError &e)
		{
			e.printMessage();
		}
	
		if ( info.probed == true )
		{
			qDebug() << "Name: " << info.name.c_str();
			qDebug() << "Input: " << info.inputChannels;
			qDebug() << "Output: " << info.outputChannels;
			if (info.inputChannels > 0)
			{
			// Print, for example, the maximum number of output channels for each device
				QList<int> supportedChannels;
				QList<int> sampleRates;
				for (unsigned int j=1; j <= info.inputChannels; j++)
				{
					supportedChannels << j;
				}
				for (unsigned int j=0; j < info.sampleRates.size(); j++)
					sampleRates << info.sampleRates[j];
				sdl->append(SoundDevice(i, info.name.c_str(), supportedChannels, sampleRates));
			}
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
	delete audio;
}
