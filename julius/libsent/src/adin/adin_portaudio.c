/**
 * @file   adin_portaudio.c
 * 
 * <JA>
 * @brief  �ޥ������� (portaudio�饤�֥���)
 *
 * portaudioo�饤�֥��������Ѥ����ޥ������ϤΤ����������٥��ؿ��Ǥ���
 * ���Ѥ����ˤ� configure ���� "--with-mictype=portaudio" �����ꤷ�Ʋ�������
 * Linux ������ Win32 �ǻ��Ѳ�ǽ�Ǥ���Win32�⡼�ɤǤϤ��줬
 * �ǥե����ȤȤʤ��ޤ���
 *
 * Julius�ϥߥ������ǥХ��������������ڹԤ��ޤ�����Ͽ���ǥХ�����
 * ����ʥޥ���/�饤���ˤ�Ͽ���ܥ��塼����Ĵ����Windows��
 * �֥ܥ��塼�ॳ���ȥ����� �� Linux �� xmixer �ʤɡ�¾�Υġ�����
 * �ԤʤäƲ�������
 *
 * portaudio �ϥե꡼�ǥ����ץ��åȥۡ����Υ����ǥ��������ϥ饤�֥���d
 * �Ǥ����������� libsent/src/adin/pa/ �˴ޤޤ��Ƥ��ޤ������Υץ������Ǥ�
 * �����åɤ����Ѥ���callback �����Ѥ������ϲ�������󥰥Хåե��˼���������
 * ���ޤ���
 *
 * @sa http://www.portaudio.com/
 * </JA>
 * <EN>
 * @brief  Microphone input using portaudio library
 *
 * Low level I/O functions for microphone input using portaudio library.
 * To use, please specify "--with-mictype=portaudio" options
 * to configure script.  This function is currently available for Linux and
 * Win32.  On Windows, this is default.
 *
 * Julius does not alter any mixer device setting at all.  You should
 * configure the mixer for recording source (mic/line) and recording volume
 * correctly using other audio tool such as xmixer on Linux, or
 * 'Volume Control' on Windows.
 *
 * Portaudio is a free, cross platform, open-source audio I/O library.
 * The sources are included at libsent/src/adin/pa/.  This program uses
 * ring buffer to store captured samples in callback functions with threading.
 *
 * @sa http://www.portaudio.com/
 * </EN>
 *
 * @author Akinobu LEE
 * @date   Mon Feb 14 12:03:48 2005
 *
 * $Revision: 1.2 $
 * 
 */
/*
 * Copyright (c) 2004-2005 Shikano Lab., Nara Institute of Science and Technology
 * Copyright (c) 2005-2007 Julius project team, Nagoya Institute of Technology
 * All rights reserved
 */

#include <sent/stddefs.h>
#include <sent/speech.h>
#include <sent/adin.h>

/* sound header */
/*#include "pa/portaudio.h"*/
#include <portaudio.h>
#ifdef linux
#include <pthread.h>
#endif

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)


#undef DDEBUG


/**
 * Maximum Data fragment Length in msec.  Input can be delayed to this time.
 * You can override this value by specifying environment valuable
 * "LATENCY_MSEC".
 * 
 */
#define MAX_FRAGMENT_MSEC 128

/* temporal buffer */
static SP16 *speech;		///< cycle buffer for incoming speech data
static int current;		///< writing point
static int processed;		///< reading point
static boolean buffer_overflowed = FALSE; ///< TRUE if buffer overflowed
static int cycle_buffer_len;	///< length of cycle buffer based on INPUT_DELAY_SEC
static boolean adin_is_stopped = TRUE;
static int adin_stream_sfreq = -1;

/*static pthread_mutex_t adin_mutex;*/

boolean adin_stream_should_be_running_pa = FALSE;

/** 
 * PortAudio callback to store the incoming speech data into the cycle
 * buffer.
 * 
 * @param inbuf [in] portaudio input buffer 
 * @param outbuf [in] portaudio output buffer (not used)
 * @param len [in] length of above
 * @param outTime [in] output time (not used)
 * @param userdata [in] user defined data (not used)
 * 
 * @return 0 when no error, or 1 to terminate recording.
 */
static int
Callback(const void *inbuf, void *outbuf, unsigned long len, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
  if (adin_stream_should_be_running_pa == FALSE)
	  return paAbort;

  const SP16 *now;
  int avail;
  int processed_local;
  int written;

  now = inbuf;

  processed_local = processed;

#ifdef DDEBUG
  printf("callback-1: processed=%d, current=%d: recordlen=%d\n", processed_local, current, len);
#endif

  /* check overflow */
  if (processed_local > current) {
    avail = processed_local - current;
  } else {
    avail = cycle_buffer_len + processed_local - current;
  }
  if (len > avail) {
#ifdef DDEBUG
    printf("callback-*: buffer overflow!\n");
#endif
    buffer_overflowed = TRUE;
    len = avail;
  }

  /*pthread_mutex_lock(&adin_mutex);*/
  /* store to buffer */
  if (current + len <= cycle_buffer_len) {
    memcpy(&(speech[current]), now, len * sizeof(SP16));
#ifdef DDEBUG
    printf("callback-2: [%d..%d] %d samples written\n", current, current+len, len);
#endif
  } else {
    written = cycle_buffer_len - current;
    memcpy(&(speech[current]), now, written * sizeof(SP16));
#ifdef DDEBUG
    printf("callback-2-1: [%d..%d] %d samples written\n", current, current+written, written);
#endif
    memcpy(&(speech[0]), &(now[written]), (len - written) * sizeof(SP16));
#ifdef DDEBUG
    printf("callback-2-2: ->[%d..%d] %d samples written (total %d samples)\n", 0, len-written, len-written, len);
#endif
  }
  current += len;
  if (current >= cycle_buffer_len) current -= cycle_buffer_len;
#ifdef DDEBUG
  printf("callback-3: new current: %d\n", current);
#endif


  /*pthread_mutex_unlock(&adin_mutex);*/

  if (adin_stream_should_be_running_pa==TRUE)
	return paContinue;
  else return paAbort;
}

static PaStream *stream=NULL;		///< Stream information

/** 
 * Device initialization: check device capability and open for recording.
 * 
 * @param sfreq [in] required sampling frequency.
 * @param dummy [in] a dummy data
 * 
 * @return TRUE on success, FALSE on failure.
 */
boolean
adin_mic_standby(int sfreq, void *dummy)
{
  adin_stream_sfreq = sfreq;

  return(TRUE);
}

/** 
 * Start recording.
 * 
 * @return TRUE on success, FALSE on failure.
 */
boolean
adin_mic_begin(char *pathname)
{
  if (adin_stream_sfreq == -1) {
	  return(FALSE);
  }


  /* initialize mutex */
  /*pthread_mutex_init (&adin_mutex, NULL);*/

  PaError err;
  int frames_per_buffer, num_buffer;
  char *p;

  /* set cycle buffer length */
  cycle_buffer_len = INPUT_DELAY_SEC * adin_stream_sfreq;
  jlog("Stat: adin_portaudio: INPUT_DELAY_SEC = %d\n", INPUT_DELAY_SEC);
  jlog("Stat: adin_portaudio: audio cycle buffer length = %d bytes\n", cycle_buffer_len * sizeof(SP16));

  /* allocate and init */
  current = processed = 0;
  speech = (SP16 *)mymalloc(sizeof(SP16) * cycle_buffer_len);
  buffer_overflowed = FALSE;

  frames_per_buffer = 256;
  jlog("Stat: adin_portaudio: framesPerBuffer=%d\n", frames_per_buffer);

  /* initialize device and open stream */
  err = Pa_Initialize();
  if (err != paNoError) {
    jlog("Error: adin_portaudio: failed to initialize: %s\n", Pa_GetErrorText(err));
    return(FALSE);
  }

  adin_is_stopped = FALSE;

  PaStreamParameters  inputParameters;
  bzero( &inputParameters, sizeof( inputParameters ) );
  int device;
  char* inputDevice = getenv("SIMONDEV");
  if (!inputDevice) device = 0;
  else
	  device = atoi(inputDevice);
  inputParameters.device = device; 
  inputParameters.channelCount = 1;
  inputParameters.sampleFormat = paInt16;
  const PaDeviceInfo *info = Pa_GetDeviceInfo( inputParameters.device );
  if (!info) {
    return (FALSE);
  }

  inputParameters.suggestedLatency = info->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;


  err = Pa_OpenStream(&stream, &inputParameters, NULL, adin_stream_sfreq, 
			     frames_per_buffer, /*num_buffer, */ paClipOff,
			     Callback, NULL);
  if (err != paNoError) {
    jlog("Error: adin_portaudio: error in opening stream: %s\n", Pa_GetErrorText(err));
    return(FALSE);
  }

  adin_stream_should_be_running_pa=TRUE;
  /* start stream */
  err = Pa_StartStream(stream);
  if (err != paNoError) {
    jlog("Error: adin_portaudio: failed to begin stream: %s\n", Pa_GetErrorText(err));
    adin_stream_should_be_running_pa=FALSE;
    return(FALSE);
  }
  
  return(TRUE);
}

/** 
 * Stop recording.
 * 
 * @return TRUE on success, FALSE on failure.
 */
boolean
adin_mic_end()
{
  fprintf(stderr, "\nadin_mic_end()\n");
  if (adin_is_stopped || (stream==NULL)) {
	return(TRUE); // we have already stopped
  }

  /*pthread_mutex_lock(&adin_mutex);*/

  PaStream *real_stream = stream;
  stream=NULL;
  
  adin_stream_should_be_running_pa=FALSE;
  PaError err;

  /* stop stream */
  err = Pa_StopStream(real_stream);
  if (err != paNoError) {
    jlog("Error: adin_portaudio: failed to stop stream: %s\n", Pa_GetErrorText(err));
    return(FALSE);
  }
  err = Pa_CloseStream(real_stream);
  if (err != paNoError) {
    jlog("Error: adin_portaudio: failed to close stream: %s\n", Pa_GetErrorText(err));
    return(FALSE);
  }
  err = Pa_Terminate();
  if (err != paNoError) {
    jlog("Error: adin_portaudio: failed to terminate portaudio: %s\n", Pa_GetErrorText(err));
    return(FALSE);
  }
  current = processed = 0;
  adin_is_stopped = TRUE;

  /*pthread_mutex_unlock(&adin_mutex);*/

  /* destroy the mutex and unallocate any memory associated to it */
  /*pthread_mutex_destroy (&adin_mutex);*/

  return TRUE;
}

/**
 * @brief  Read samples from device
 * 
 * Try to read @a sampnum samples and returns actual number of recorded
 * samples currently available.  This function will block until
 * at least some samples are obtained.
 * 
 * @param buf [out] samples obtained in this function
 * @param sampnum [in] wanted number of samples to be read
 * 
 * @return actural number of read samples, -2 if an error occured.
 */
int
adin_mic_read(SP16 *buf, int sampnum)
{
  if (adin_stream_should_be_running_pa==FALSE)
	return -1;

  int current_local;
  int avail;
  int len;

  if (buffer_overflowed) {
    jlog("Error: adin_portaudio: input buffer OVERFLOW, increase INPUT_DELAY_SEC in sent/speech.h\n");
    buffer_overflowed = FALSE;
  }

  while ((current == processed) && adin_stream_should_be_running_pa) {
#ifdef DDEBUG
    printf("process  : current == processed: %d: wait\n", current);
#endif
    Pa_Sleep(30); /* wait till some input comes */
  }

  /*pthread_mutex_lock(&adin_mutex);*/ /* lock the mutex */

  if ((current == processed)
	|| (adin_stream_should_be_running_pa == FALSE)
        || (speech==NULL)) {
    /* any of these conditions mean that we have stopped recording */
    /*pthread_mutex_unlock(&adin_mutex);*/
    return -1;
  }

  current_local = current;

#ifdef DDEBUG
  printf("process-1: processed=%d, current=%d\n", processed, current_local);
#endif

  if (processed < current_local) {
    avail = current_local - processed;
    if (avail > sampnum) avail = sampnum;

    /*
    if (adin_stream_should_be_running_pa == TRUE)
	    fprintf(stderr, "Adin should be running: TRUE\n");
    else
	    fprintf(stderr, "Adin should be running: TRUE\n");
    fprintf(stderr, "Available: %d\n", avail);
    fprintf(stderr, "Processed: %d\n", processed);
    */
    //if the following line crashes you forgot to call j_close_stream()
    memcpy(buf, &(speech[processed]), avail * sizeof(SP16));
#ifdef DDEBUG
    printf("process-2: [%d..%d] %d samples read\n", processed, processed+avail, avail);
#endif
    len = avail;
    processed += avail;
  } else {
    avail = cycle_buffer_len - processed;
    if (avail > sampnum) avail = sampnum;
    memcpy(buf, &(speech[processed]), avail * sizeof(SP16));
#ifdef DDEBUG
    printf("process-2-1: [%d..%d] %d samples read\n", processed, processed+avail, avail);
#endif
    len = avail;
    processed += avail;
    if (processed >= cycle_buffer_len) processed -= cycle_buffer_len;
    if (sampnum - avail > 0) {
      if (sampnum - avail < current_local) {
	avail = sampnum - avail;
      } else {
	avail = current_local;
      }
      if (avail > 0) {
	memcpy(&(buf[len]), &(speech[0]), avail * sizeof(SP16));
#ifdef DDEBUG
	printf("process-2-2: [%d..%d] %d samples read (total %d)\n", 0, avail, avail, len + avail);
#endif
	len += avail;
	processed += avail;
	if (processed >= cycle_buffer_len) processed -= cycle_buffer_len;
      }
    }
  }
#ifdef DDEBUG
  printf("process-3: new processed: %d\n", processed);
#endif
  /*pthread_mutex_unlock(&adin_mutex);*/ /* unlock mutex */
  if (len == 0) return -1;
  return len;
}

/** 
 * 
 * Function to return current input source device name
 * 
 * @return string of current input device name.
 * 
 */
char *
adin_mic_input_name()
{
  return("Portaudio device set by simon");
}

