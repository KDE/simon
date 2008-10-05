/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include "soundcontrol.h"
#include <KLocalizedString>
#include <string.h>
#include <QObject>
#include <KMessageBox>
#include "simoninfo/simoninfo.h"
#include "logging/logger.h"
#include "portaudio.h"
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

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
 *	@author Peter Grasch
 *	@param int percent
 *	returns the available sound devices
*/

SoundDeviceList* SoundControl::getOutputDevices()
{
	Logger::log(i18n("[INF] Bekommen einer Liste mit den verfügbaren devices"));
	SoundDeviceList *sdl= new SoundDeviceList();
	
	if (Pa_Initialize() < 0) return sdl;

	int numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
		KMessageBox::error(0, i18n("Fehler beim Einlesen der Audio-Ausgabegeräte:\n\nPa_CountDevices returned %1", numDevices));
		return sdl;
	}

	const PaDeviceInfo *deviceInfo;
	for(int i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		
		if (deviceInfo->maxOutputChannels > 0)
		{ 	//yay it's an output-device!
			//TODO: samplerate as double? wtf?
			sdl->append(SoundDevice ( i, QString(deviceInfo->name), deviceInfo->maxOutputChannels, (int) deviceInfo->defaultSampleRate ));
		}
	}

	Pa_Terminate();
	
	return sdl;
}


/**
 * \brief Returns the available input devices
 * \author Peter Grasch
 * @return The list of found Sounddevices
 */
SoundDeviceList* SoundControl::getInputDevices()
{
	Logger::log(i18n("[INF] Bekommen einer Liste mit den verfügbaren Input-devices"));
	SoundDeviceList *sdl= new SoundDeviceList();

	if (Pa_Initialize() < 0) return sdl;

	
	int numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
		KMessageBox::error(0, i18n("Fehler beim Einlesen der Audio-Eingabegeräte:\n\nPa_CountDevices returned %1", numDevices));
		return sdl;
	}

	const PaDeviceInfo *deviceInfo;
	for(int i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		
		if (deviceInfo->maxInputChannels > 0)
		{ 	//yay it's an input-device!
			//TODO: samplerate as double? wtf?
			sdl->append(SoundDevice ( i, QString(deviceInfo->name), deviceInfo->maxInputChannels, (int) deviceInfo->defaultSampleRate ));
		}
	}
	
	Pa_Terminate();

	return sdl;
}

bool SoundControl::checkDeviceSupport(int inputDeviceId, int outputDeviceId, int channels, int samplerate)
{
	if (Pa_Initialize() < 0) return false;
	
	PaStreamParameters outputParameters;
	PaStreamParameters inputParameters;
	
	bzero( &inputParameters, sizeof( inputParameters ) );
	inputParameters.channelCount = channels;
	inputParameters.device = inputDeviceId;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	inputParameters.sampleFormat = paInt16;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputDeviceId)->defaultLowInputLatency ;
	inputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field
	
	
	bzero( &outputParameters, sizeof( outputParameters ) );
	outputParameters.channelCount = channels;
	outputParameters.device = outputDeviceId;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.sampleFormat = paInt16;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputDeviceId)->defaultLowInputLatency ;
	outputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field
	
	PaError err;
	err = Pa_IsFormatSupported( &inputParameters, &outputParameters, (double) samplerate );
	
	Pa_Terminate();
	
	return ( err == paFormatIsSupported );
}

/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
*/
SoundControl::~SoundControl()
{
}
