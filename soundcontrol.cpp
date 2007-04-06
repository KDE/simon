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
	
	if (!(abackend->openDevice("default")))
	{
			SimonInfo::showMessage("Konnte Soundkarte nicht ˆffnen", 6000);
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
		SimonInfo::showMessage("Setzen der Kan‰le fehlgeschlagen", 6000);
		return false;
	}
	
	if (!(abackend->prepareDevice()))
	{
		SimonInfo::showMessage("Konnte das Ger‰t nicht vorbereiten", 6000);
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
bool SoundControl::closeMic()
{
#ifdef linux
	ALSABackend *abackend = dynamic_cast<ALSABackend*>(soundbackend);
	if (!(abackend)) return false;
	
	if (!(abackend->closeDevice()))
	{
		SimonInfo::showMessage("Konnte Soundkarte nicht schlieﬂen", 6000);
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
 *	@brief Destructor
 *	
 *	@author Peter Grasch
*/
SoundControl::~SoundControl()
{
}
