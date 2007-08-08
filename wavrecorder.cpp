#include "wavrecorder.h"
#include "logger.h"

/**
 * \brief Constructor
 */
WavRecorder::WavRecorder(QWidget *parent) : QObject(parent)
{
	
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
bool WavRecorder::record(QString filename, short channels, int sampleRate)
{
	Logger::log(tr("[INF] Aufnehmen: %1 Kanäle, samplerate: %2 Hz) zu %3").arg(channels).arg(sampleRate).arg(filename));
	
	wavData = new WAV(filename, sampleRate);
	
	progress=0;
	progressTimer = new QTimer();
	connect(progressTimer, SIGNAL(timeout()), this, SLOT(increaseProgress()));
	progressTimer->start(100);
	
	int fs=44100, buffer_size=512, device = 0;
	chans=1;
	//long frames, counter = 0;
	signed short *buffer;
	audio = 0;

	try {
		audio = new RtAudio(0, 0, device, chans,
				    RTAUDIO_SINT16, fs, &buffer_size, 8);
	}
	catch (RtError &error) {
		error.printMessage();
		exit(EXIT_FAILURE);
	}

	try {
		buffer = (signed short *) audio->getStreamBuffer();
		audio->startStream();
	}
	catch (RtError &error) {
		error.printMessage();
		audio->closeStream();
		delete audio;
		return false;
	}
	
	audio->setStreamCallback(&processWrapper, (void*) this);
	audio->startStream();
	
	return true;
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
	
	progressTimer->stop();
	if (! wavData->writeFile()) return false;
	
	delete wavData;
	return true;
}

/**
 * \brief Increases the progress by 100 msecs
 * 
 * emits the currentProgress signal
 * 
 * \author Peter Grasch
 */
void WavRecorder::increaseProgress()
{
	progress+=100;
	emit currentProgress(progress);
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
int WavRecorder::processWrapper(char* buffer, int bufferSize, void *rec)
{
	WAV *wavFile = ((WavRecorder*) rec)->getWav();
	wavFile->addData((char*) buffer, sizeof(signed short)*
			((bufferSize*((WavRecorder*) rec)->getChannels())));
	return 0;
}


/**
 * \brief Destructor
 */
WavRecorder::~WavRecorder()
{
}


