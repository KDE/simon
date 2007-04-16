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


/**
 *	@brief Constructor
 *	
 *	Creates a new Soundbackend depending on the arch.
 *	
 *	@author Peter Grasch
 */
SoundControl::SoundControl()
{
#ifdef linux
	soundbackend = new ALSABackend();
#endif
#ifdef __WIN32
	soundbackend= new DirectSoundBackend();
#endif
}

/**
 * \brief Plays back the given data
 * 
 * \author Peter Grasch
 * \param char* data
 * The data to play
 * \param int count
 * length of the array
 */

bool SoundControl::playback (char* data, int count)
{
#ifdef linux
	ALSABackend *abackend = dynamic_cast<ALSABackend*>(soundbackend);
	if (!(abackend)) return false;
	abackend->writeData(data, count);
#endif
}

/**
 *	@brief Prepares the mic for recording
 *	
 *	Calls - depending on the arch. all the needed calls to get
 *	the sound card ready for recording
 *	It also catches all possible errors and tells the user about it
 *	
 *	@author Peter Grasch
 *	@param short channels
 *	Number of channels (1=mono, 2=stereo,...)
 *	@param int samplerate
 *	Samplerate in Hz (usually 44100)
 *	@return bool
 *	Returns wether the mic was successfully prepared
 */
bool SoundControl::initializeMic(short channels, int samplerate)
{
#ifdef linux
	ALSABackend *abackend = dynamic_cast<ALSABackend*>(soundbackend);
	if (!(abackend)) return false;
	
	if (!(abackend->openDevice("default", READ)))
	{
		SimonInfo::showMessage("Konnte Mikrofon nicht �ffnen", 6000);
		return false;
	}
	if (!(abackend->setInterleaved(true)))
	{
		SimonInfo::showMessage("Konnte Soundkarte nicht auf interleaved setzen", 6000);
		return false;
	}
	if (!(abackend->setSampleRate(samplerate)))
	{
		SimonInfo::showMessage("Konnte Samplerate nicht setzen", 6000);
		return false;
	}
	
	if (!(abackend->setChannels(channels)))
	{
		SimonInfo::showMessage("Setzen der Kan�le fehlgeschlagen", 6000);
		return false;
	}
	
	if (!(abackend->prepareDevice()))
	{
		SimonInfo::showMessage("Konnte das Ger�t nicht vorbereiten", 6000);
		return false;
	}
	
#endif //linux
}


/**
 *	@brief Prepares the speakers for playing music
 *	
 *	Calls - depending on the arch. all the needed calls to get
 *	the sound card ready for writing
 *	It also catches all possible errors and tells the user about it
 *	
 *	@author Peter Grasch
 *	@param short channels
 *	Number of channels (1=mono, 2=stereo,...)
 *	@param int samplerate
 *	Samplerate in Hz (usually 44100)
 *	@return bool
 *	Returns wether speakers were successfully prepared
 */
bool SoundControl::initializeSpeaker(short channels, int samplerate)
{
#ifdef linux
	ALSABackend *abackend = dynamic_cast<ALSABackend*>(soundbackend);
	if (!(abackend)) return false;
	
	if (!(abackend->openDevice("default", WRITE)))
	{
		SimonInfo::showMessage("Konnte Lautsprecher nicht �ffnen", 6000);
		return false;
	}
	if (!(abackend->setInterleaved(true)))
	{
		SimonInfo::showMessage("Konnte Soundkarte nicht auf interleaved setzen", 6000);
		return false;
	}
	if (!(abackend->setSampleRate(samplerate)))
	{
		SimonInfo::showMessage("Konnte Samplerate nicht setzen", 6000);
		return false;
	}
	
	if (!(abackend->setChannels(channels)))
	{
		SimonInfo::showMessage("Setzen der Kan�le fehlgeschlagen", 6000);
		return false;
	}
	
	if (!(abackend->prepareDevice()))
	{
		SimonInfo::showMessage("Konnte das Ger�t nicht vorbereiten", 6000);
		return false;
	}
	
#endif //linux
}


/**
 *	@brief Captures data from the device
 *	
 *	Records data from a prepared the device
 *	
 *	@author Peter Grasch
 *	@param int msecs
 *	Msecs of time to capture
 *	@param long unsigned int *size
 *	Number of frames read
 *	@return char
 *	The read data
 *	@see initializeMic()
 */
char* SoundControl::capture (int msecs, long unsigned int& size)
{
	
#ifdef linux
	ALSABackend *abackend = dynamic_cast<ALSABackend*>(soundbackend);
	if (!(abackend)) return false;
	
	char * buffer;
	buffer = abackend->readData(msecs,size);
	
	if (!(buffer))
	{
		SimonInfo::showMessage("Aufnehmen fehlgeschlagen", 6000);
	}
	
	return buffer;
#endif //linux
}

/**
 *	@brief Captures data from the device
 *	
 *	Records data from a prepared the device
 *	
 *	@author Peter Grasch
 *	@param int count
 *	How often should we read the buffersize?
 *	@param int buffersize
 *	How long should the buffer be?
 *	@param long unsigned int& size
 *	Reference parameter to return the actual number of frames read
 *	@return short*
 *	Array of the freq. data
 *	@see initializeMic()
 */
short* SoundControl::capture (int count, int buffersize, long unsigned int& size)
{
	
#ifdef linux
	ALSABackend *abackend = dynamic_cast<ALSABackend*>(soundbackend);
	if (!(abackend)) return false;
	
	short * buffer;
	buffer = abackend->readData(count,buffersize,size);
	
	if (!(buffer))
	{
		SimonInfo::showMessage("Aufnehmen fehlgeschlagen", 6000);
	}
	return buffer;
#endif //linux
}

/**
 *	@brief Closes the mic
 *	
 *	Closes the handles and opens the lock
 *	
 *	@author Peter Grasch
 *	@return bool
 *	Returns wether the mic was successfully closed
 */
bool SoundControl::close()
{
#ifdef linux
	ALSABackend *abackend = dynamic_cast<ALSABackend*>(soundbackend);
	if (!(abackend)) return false;
	
	if (!(abackend->closeDevice()))
	{
		SimonInfo::showMessage("Konnte Soundkarte nicht schlie�en", 6000);
		return false;
	}
#endif //linux
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
	
}

/**
 *	@brief Sets the volume in percent
 *	
 *	@author Peter Grasch
 *	@param int percent
 *	volume in percent
*/
void SoundControl::setVolume(int percent)
{
	
}

/**
 *	@brief Sets the volume in percent
 *	
 *	@author Gigerl Martin
 *	@param int percent
 *	returns the aviable sound devices
*/

SoundDeviceList* SoundControl::getDevices()
{
SoundDeviceList *sdl= new SoundDeviceList(); 
 sdl->append(SoundDevice("145","fakedevice1"));
 sdl->append(SoundDevice("147","fakedevice2"));        
#ifdef linux
 return sdl;
#endif
#ifdef __WIN32
 return sdl;
#endif
}

/**
 *	@brief Destructor
 *	
 *	@author Peter Grasch
*/
SoundControl::~SoundControl()
{
	delete this->soundbackend;
}
