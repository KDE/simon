#include "wavrecorder.h"
#include "settings.h"
#include <QVariant>
#include "RtError.h"
#include "logger.h"
#include <QObject>
#include "wav.h"

/**
 * \brief Constructor
 */
WavRecorder::WavRecorder(QObject *parent) : QObject(parent)
{
	audio = 0;
	wavData=0;
}

int processData( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *userData )
{
// 	if ( status )
// 		qDebug() << QObject::tr("Bufferüberlauf!");

	WavRecorder *rec = (WavRecorder*) userData;
	if (!rec) return 1;

	WAV *wav = rec->getWav();
	wav->addData((char*) inputBuffer, sizeof(signed short)*nBufferFrames*rec->getChannels());
	rec->publishTime(streamTime);

	return 0;
}

/**
 * \brief Records a WAV file to the given filename
 * \author Peter Grasch
 * \param QString filename
 * Filename to write to
 * \param short channels
 * Channels
 * \param int sampleRate
 * The samplerate of the file
 */
bool WavRecorder::record(QString filename)
{
	if (audio) {
		delete audio;
		audio =0;
	}
	audio = new RtAudio();
	RtAudio::StreamParameters parameters;

	parameters.deviceId = Settings::get("Sound/InputDevice").toInt();
	parameters.nChannels = this->chans = Settings::get("Sound/ChannelsIn").toInt();
	parameters.firstChannel = 0;
	unsigned int sampleRate = Settings::get("Sound/SamplerateIn").toInt();
	unsigned int bufferFrames = 256; // 256 sample frames

	wavData = new WAV(filename, sampleRate);

	try {
		audio->openStream( NULL, &parameters, RTAUDIO_SINT16,
				   sampleRate, &bufferFrames, &processData, (void*) this, NULL );
		audio->startStream();
	}
	catch ( RtError& e ) {
		e.printMessage();
		return false;
	}
	return true;
}


void WavRecorder::publishTime(double time)
{
	emit currentProgress(time*1000);
}

/**
 * \brief This will stop the current recording
 * 
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
bool WavRecorder::finish()
{
	try {
		audio->stopStream();
		audio->closeStream();
	}
	catch (RtError &error) {
		error.printMessage();
	}
	delete audio;
	audio = 0;
	
	if (! wavData->writeFile()) return false;
	
	delete wavData;
	wavData = 0;
	return true;
}

/**
 * \brief Callback function that sends the gathered data to the WAV class
 * @param buffer
 * The buffer to process
 * @param bufferSize 
 * Buffersize
 * @param rec
 * The object who gathered the data - this is used to get the WAV object and the channels 
 * @return int
 * always 0
 */
// int WavRecorder::processWrapper(void *output, char *buffer, int bufferSize, double streamTime, RtAudioStreamStatus status, void* rec)
// {
// 	qDebug() << streamTime;
// 	qDebug() << status;
// 	WAV *wavFile = ((WavRecorder*) rec)->getWav();
// 	wavFile->addData((char*) buffer, sizeof(signed short)*
// 			((bufferSize*((WavRecorder*) rec)->getChannels())));
// 	return 0;
// }


/**
 * \brief Destructor
 */
WavRecorder::~WavRecorder()
{
    if (wavData) delete wavData;
    if (audio) delete audio;
}


