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
/**
 *	@brief Constructor
 *	
 *	Creates a new Soundbackend depending on the arch.
 *	
 *	@author Peter Grasch
 */
SoundControl::SoundControl()
{
	in_audio= new RtAudio();
	out_audio = new RtAudio();
// 	try {
// 		in_audio = new RtAudio();
// 	}
// 	catch (RtError &error) {
// 		error.printMessage();
// 	}
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
	int read=0;
	//char* tmp = new char(255);
	
	while (read+out_buffersize < count) {
		
		memcpy((void*) this->out_data, 
				     ((void*) (out_data+read)), 
				      (size_t) out_buffersize);
	
		try {
			out_audio->tickStream();
		}
		catch (RtError &error) {
			error.printMessage();
			this->closeSpeaker();
		}
 		
		read += out_buffersize;
	}
	try {
		out_audio->stopStream();
	}
	catch (RtError &error) {
		
		SimonInfo::showMessage(error.getMessageString(), 6000);
		error.printMessage();
	}
	
	return true;
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
	int device=0;
	in_buffersize = 512;
	in_samplerate = samplerate;
	in_channels=channels;
	try {
		in_audio = new RtAudio(0, 0, device, channels,
				    FORMAT, samplerate, &in_buffersize, 8);
	}
	catch (RtError &error) {
		SimonInfo::showMessage(error.getMessageString(), 6000);
		error.printMessage();
		return false;
	}


	try {
		in_data = (char *) in_audio->getStreamBuffer();
		in_audio->startStream();
	}
	catch (RtError &error) {
		SimonInfo::showMessage(error.getMessageString(), 6000);
		error.printMessage();
		in_audio->closeStream();
		return false;
	}

	
	return true;
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
	out_buffersize = 512;
	int device = 0;
	out_samplerate=samplerate;
	in_channels=channels;
	
	try {
		out_audio = new RtAudio(device, channels, 0, 0,
				    FORMAT, samplerate, &out_buffersize, 2);
	}
	catch (RtError &error) {
		SimonInfo::showMessage(error.getMessageString(), 6000);
		error.printMessage();
		return false;
	}

	try {
		out_data = (char *) out_audio->getStreamBuffer();
		out_audio->startStream();
	}
	catch (RtError &error) {
		SimonInfo::showMessage(error.getMessageString(), 6000);
		out_audio->closeStream();
		return false;
	}
	
	return true;
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
	FILE *fd = fopen("test.raw","wb");
	long counter=0;
	long frames = (long) (((double)in_samplerate) * (((double) msecs)/((double)1000)));
	size=frames;
// 	std::cout << "Channels: " << in_channels << "; Samplerate: " << in_samplerate << "; Frames: " << frames << ";" << std::endl;
	char* data = new char(frames*in_channels);
	
	while (counter < frames) {

		try {
			in_audio->tickStream();
		}
		catch (RtError &error) {
			error.printMessage();
			in_audio->closeStream();
			break;
		}
		
		//saving the wavedata temp.
		char* tmp = (char*) malloc (counter*in_channels);
		for (int i = 0; i < counter*in_channels; i++)
			tmp[i] = data[i];
// 	
// 		//reallocate the member and extends its size of the size of the data to add
		data = (char*) malloc(counter*in_channels + in_buffersize*in_channels);
// 	
// 		//copy back the original data
		for (int i = 0; i < counter*in_channels; i++)
			data[i] = tmp[i];
// 	
// 		//add the new data
		for (int i = 0; i < in_buffersize*in_channels; i++)
			data[i+(counter*in_channels)] = in_data[i];
		
		fwrite(in_data, sizeof(char), in_channels * in_buffersize, fd);
		counter += in_buffersize;
	}
	fclose(fd);

	try {
		in_audio->stopStream();
	}
	catch (RtError &error) {
		error.printMessage();
	}
	size=counter*in_channels;
	return data;
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
	if (in_audio) {
		 
		try {
			in_audio->closeStream();
		}
		catch (RtError &error) {
			error.printMessage();
			return false;
		}
	}
	
	return true;
}


/**
 *	@brief Closes the "speakers"
 *	
 *	Closes the handles and opens the lock
 *	
 *	@author Peter Grasch
 *	@return bool
 *	Returns wether the speakers were successfully closed
 */
bool SoundControl::closeSpeaker()
{
	if (out_audio) {
		 
		try {
			out_audio->closeStream();
		}
		catch (RtError &error) {
			error.printMessage();
			return false;
		}
	}
	
	return true;
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

SoundDeviceList* SoundControl::getOutputDevices()
{
SoundDeviceList *sdl= new SoundDeviceList(); 
  RtAudio *audio = 0;
  try 
  {
    audio = new RtAudio();
  }
  catch (RtError &error) 
  {
    error.printMessage();
    exit(EXIT_FAILURE);
  }
  
  // Determine the number of devices available
  int devices = audio->getDeviceCount();

  // Scan through devices for various capabilities
  RtAudioDeviceInfo info;
  for (int i=1; i<=devices; i++) 
  {

    try 
    {        
      info = audio->getDeviceInfo(i);
      if(info.outputChannels>0)
      {
       sdl->append(SoundDevice(QString::number(i),QString(info.name.c_str())));
      }
    }
    catch (RtError &error) 
    {
      error.printMessage();
      break;
    }
  }
  
 return sdl;
}

SoundDeviceList* SoundControl::getInputDevices()
{
SoundDeviceList *sdl= new SoundDeviceList(); 
  RtAudio *audio = 0;
  try 
  {
    audio = new RtAudio();
  }
  catch (RtError &error) 
  {
    error.printMessage();
    exit(EXIT_FAILURE);
  }
  
  // Determine the number of devices available
  int devices = audio->getDeviceCount();

  // Scan through devices for various capabilities
  RtAudioDeviceInfo info;
  for (int i=1; i<=devices; i++) 
  {

    try 
    {        
      info = audio->getDeviceInfo(i);
      if(info.inputChannels>0)
      {
       sdl->append(SoundDevice(QString::number(i),QString(info.name.c_str())));
      }
    }
    catch (RtError &error) 
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
