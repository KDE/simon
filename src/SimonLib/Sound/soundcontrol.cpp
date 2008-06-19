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
#include <QObject>
#include <QMessageBox>
#include "../SimonInfo/simoninfo.h"
#include "../Logging/logger.h"
#include "portaudio.h"

#include <QObject>
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
 *	returns the aviable sound devices
*/

SoundDeviceList* SoundControl::getOutputDevices()
{
	Logger::log(QObject::tr("[INF] Bekommen einer Liste mit den verfügbaren devices"));
	SoundDeviceList *sdl= new SoundDeviceList();
	
	if (Pa_Initialize() < 0) return sdl;

	int numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
		QMessageBox::critical(0, QObject::tr("Konnte Ausgabegeräte nicht einlesen"), QObject::tr("Fehler beim Einlesen der Audio-Ausgabegeräte:\n\nPa_CountDevices returned %1").arg(numDevices));
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
	Logger::log(QObject::tr("[INF] Bekommen einer Liste mit den verfügbaren Input-devices"));
	SoundDeviceList *sdl= new SoundDeviceList();

	if (Pa_Initialize() < 0) return sdl;

	
	int numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
		QMessageBox::critical(0, QObject::tr("Konnte Eingabegeräte nicht einlesen"), QObject::tr("Fehler beim Einlesen der Audio-Eingabegeräte:\n\nPa_CountDevices returned %1").arg(numDevices));
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
