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
#include <KLocale>
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
	SoundDeviceList *sdl= new SoundDeviceList();
	
	if (Pa_Initialize() < 0) return sdl;

	int numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
		KMessageBox::error(0, i18n("Error requesting the sound output devices:\n\nPa_CountDevices returned %1", numDevices));
		return sdl;
	}

	const PaDeviceInfo *deviceInfo;
	for(int i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		
		if (deviceInfo->maxOutputChannels > 0)
		{ 	//yay it's an output-device!
			sdl->append(SoundDevice ( i, QString(deviceInfo->name), deviceInfo->maxOutputChannels, (int) deviceInfo->defaultSampleRate ));
		}
	}

	Pa_Terminate();
	
	return sdl;
}

#ifdef Q_OS_UNIX
QString SoundControl::idToALSAName(int deviceId)
{
	if (Pa_Initialize() < 0) return QString();

	const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo( deviceId );
	if (!deviceInfo) {
		Pa_Terminate();
		return QString();
	}

	QString devName = QString::fromUtf8(deviceInfo->name);

	if (devName.contains("("))
	{
		devName = devName.remove(0, devName.indexOf("(")+1);
		devName = devName.left(devName.indexOf(")"));
	}

	Pa_Terminate();
	return devName;
}
#endif

int SoundControl::getDefaultInputDevice()
{
	if (Pa_Initialize() != paNoError) return 0;
	int device = Pa_GetDefaultInputDevice();
	Pa_Terminate();
	return device;
}

int SoundControl::getDefaultOutputDevice()
{
	if (Pa_Initialize() != paNoError) return 0;
	int device = Pa_GetDefaultOutputDevice();
	Pa_Terminate();
	return device;
}

/**
 * \brief Returns the available input devices
 * \author Peter Grasch
 * @return The list of found Sounddevices
 */
SoundDeviceList* SoundControl::getInputDevices()
{
	SoundDeviceList *sdl= new SoundDeviceList();

	if (Pa_Initialize() < 0) return sdl;

	
	int numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
		KMessageBox::error(0, i18n("Error requesting the sound input devices:\n\nPa_CountDevices returned %1", numDevices));
		return sdl;
	}

	const PaDeviceInfo *deviceInfo;
	for(int i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		
		if (deviceInfo->maxInputChannels > 0)
		{ 	//yay it's an input-device!
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
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputDeviceId)->defaultLowInputLatency ;
	inputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field
	
	
	bzero( &outputParameters, sizeof( outputParameters ) );
	outputParameters.channelCount = channels;
	outputParameters.device = outputDeviceId;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputDeviceId)->defaultLowOutputLatency ;
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
