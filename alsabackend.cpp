//
// C++ Implementation: alsabackend
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "alsabackend.h"


/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 */
ALSABackend::ALSABackend()
{
}


/**
 * @brief Opens the given DeviceID
 *
 * This initializes the capture_handle (member) by opening a system alsa device
 * the ID is a char* and can also take values like "default"
 * 
 * After opening the device the function tries to allocate memory to hold the
 * hardware parameters (which define stuff like channel count, interleaving mode,...)
 * 
 * The function aborts on the first error
 * 
 *	@author Peter Grasch
 *	@return bool
 *	Returns if something went wrong. true= all went well, false= error occured
 */
bool ALSABackend::openDevice( const char* deviceID )
{
	//Open the device for recording
	if (snd_pcm_open (&capture_handle, deviceID, SND_PCM_STREAM_CAPTURE, SND_PCM_ASYNC) < 0) 
		return false;
	
	//Allocate the size for the paramter
	if (snd_pcm_hw_params_malloc (&hw_params) < 0) 
		return false;
	
	//Fills the parameter with default values
	if (snd_pcm_hw_params_any (capture_handle, hw_params) < 0) 
		return false;

	return true; // All done
}


/**
 * @brief Sets the interleaving mode
 * 
 * The function aborts on the first error
 * 
 *	@author Peter Grasch
 *	@return bool interleaved
 *	Wether we should set the device interleaved or not
 *	@return bool
 *	Returns if something went wrong. true= all went well, false= error occured
 */
bool ALSABackend::setInterleaved( bool interleaved )
{
	if (interleaved)
	{
		return (!(snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0));
	} else 
		return (!(snd_pcm_hw_params_set_access (capture_handle, hw_params,  SND_PCM_ACCESS_RW_NONINTERLEAVED) < 0));
	return true;
}


/**
 * @brief Sets the Sample Rate
 * 
 * Sets the sample rate to the given value
 * 
 *	@author Peter Grasch
 *	@return int sampleRate
 *	Sample rate in Hz
 *	@return bool
 *	Returns if something went wrong. true= all went well, false= error occured
 */
bool ALSABackend::setSampleRate( int sampleRate )
{
	if (snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE) < 0) 
		return false;

	if (snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, (unsigned int*) &sampleRate, 0) < 0) 
		return false;
	
	return true;
}

/**
 * @brief Sets the number of channels used
 * 
 *	@author Peter Grasch
 *	@param short channels
 *	Number of channels (2 = stereo)
 *	@return bool
 *	Returns if something went wrong. true= all went well, false= error occured
 */
bool ALSABackend::setChannels( short channels )
{
	if (snd_pcm_hw_params_set_channels (capture_handle, hw_params, channels) < 0) 
		return false;
	return true;
}

/**
 * @brief Closes the handle
 * 
 *	@author Peter Grasch
 *	@return bool
 *	Returns if something went wrong. true= all went well, false= error occured
 */
bool ALSABackend::closeDevice()
{
	snd_pcm_close (capture_handle);
	return true;
}


/**
 * @brief Prepare the device for use
 * 
 *	Sets the parameters for the handle (by using the member in which we already
 *	stored the parameters we want by utilizing the other functions);
 *	Deletes the parameters afterwards
 * 
 *	@author Peter Grasch
 *	@return bool
 *	Returns if something went wrong. true= all went well, false= error occured
 */
bool ALSABackend::prepareDevice()
{
	if (snd_pcm_hw_params (capture_handle, hw_params) < 0) 
		return false;

	snd_pcm_hw_params_free (hw_params);
	
	return true;
}

/**
 * @brief Reads data from the device
 * 
 *	Prepares the device (this is an ALSA function)
 *	Reads some data from the device and returns it in a short array
 * 
 *	@author Peter Grasch
 *	@param short count
 *	How many times the buffersize we should read
 *	@return short**
 *	Returns the read data
 */
short **ALSABackend::readData( short count, short buffersize )
{	
	short **buf = new short*[count];
	for (int i = 0; i < count; i++)
		buf[i] = new short[buffersize];
	short *buffer[buffersize];
	
	if (snd_pcm_prepare (capture_handle) < 0) return buf;
	
	for (int i = 0; i < count; i++) 
	{
		if ((snd_pcm_readi (capture_handle, buffer, buffersize)) != buffersize) 
			return buf;
		//write (1, buffer, buffersize*4);
		
		buf[i] = (short*) buffer;
	}
	return buf;
}


/**
 * @brief Gets the currently set volume
 * 
 *	@author Peter Grasch
 *	@return int
 *	Returns the current volume in percent
 */
int ALSABackend::getVolume()
{
	
}


/**
 * @brief Sets the volume
 * 
 *	@author Peter Grasch
 *	@param int percent
 *	The percent of maximal volume
 */
void ALSABackend::setVolume( int percent )
{
	
}



/**
 * @brief Destructor
 * 
 */
ALSABackend::~ALSABackend()
{
}
