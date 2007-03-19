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
	if (snd_pcm_open (&capture_handle, deviceID, SND_PCM_STREAM_CAPTURE, 0) < 0) 
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
	
	this->sampleRate = sampleRate;
	
	if (snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, (unsigned int*) (&(this->sampleRate)), &dir) < 0) 
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
	snd_pcm_drain(capture_handle);
	snd_pcm_close(capture_handle);
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
	frames = 32;
	snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &frames, &dir);
	
	if (snd_pcm_hw_params (capture_handle, hw_params) < 0) 
		return false;
	
	//snd_pcm_hw_params_free (hw_params);
	
	return true;
}

/**
 * @brief Reads data from the device
 * 
 *	Prepares the device (this is an ALSA function)
 *	Reads some data from the device and returns it in a character array
 * 
 *	@author Peter Grasch
 *	@param int msecs
 *	The time to read
 *	@param long unsigned int& length
 *	The number of frames actually read
 *	@return char*
 *	Returns the read data
 */
char* ALSABackend::readData( int msecs, long unsigned int& length )
{
	snd_pcm_hw_params_get_period_size(hw_params,
					  &frames, &dir);

	int size = frames * 4;
	int alreadywritten=0;
	char* tmp;
	char* all = new char();
	char *buffer = (char *) malloc(size);
	int rc; //this will holds all occuring errors
	
	snd_pcm_hw_params_get_period_time(hw_params,
					  &sampleRate, &dir);
	
	int loops = (msecs*1000) / sampleRate;
	length = size * loops;
	
	while (loops > 0) {
		loops--;
		rc = snd_pcm_readi(capture_handle, buffer, frames);
		if (rc == -EPIPE) {
			fprintf(stderr, "overrun occurred\n");
			snd_pcm_prepare(capture_handle);
		} else if (rc < 0) 
			fprintf(stderr,	"error from read: %s\n", snd_strerror(rc));
		  else if (rc != (int)frames) 
			fprintf(stderr, "short read, read %d frames\n", rc);
		
		
		//adding the buffer to the data already read
		tmp  = (char*) malloc(alreadywritten);
		for (int i=0; i < alreadywritten; i++)
			tmp[i] = all[i];
		all =  (char*) malloc(alreadywritten + size);
		for (int i=0; i < alreadywritten; i++)
			all[i] = tmp[i];
		for (int i=0; i < size; i++)
			all[alreadywritten+i] = buffer[i];
		
		alreadywritten+=size;
		
		
	}
	
	
	return all;
}



/**
 * @brief Reads data from the device
 * 
 *	Prepares the device (this is an ALSA function)
 *	Reads some data from the device and returns it in a short array
 * 
 *	@author Peter Grasch
 *	@param int count
 *	How often should we read the buffersize?
 *	@param int buffersize
 *	How long should the buffer be?
 *	@param unsigned long int& length
 *	Reference used to return the number of frames read
 *	@return short*
 *	Array of the freq. data (length: count*buffersize)
 */
short* ALSABackend::readData( int count, int buffersize, unsigned long int& length )
{	
	int error;
	length = 0;
	short *all = (short*) malloc(count*buffersize);
	short *buffer = (short*) malloc(buffersize);
	
	if (snd_pcm_prepare (capture_handle) < 0) return all;
	
	int frmCount = count*buffersize;
	
	for (int i = 0; i < count; i++) 
	{
		error = snd_pcm_readi (capture_handle, buffer, buffersize);
		length += error;
		
		//copying the new buffer into the "main" buffer
		for (int j=0; j < buffersize; j++)
			all[ (i*buffersize) + j ] = buffer[j];
		
		if (error != buffersize)
		{
			return all;
		}

	}
	
	return all;
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
