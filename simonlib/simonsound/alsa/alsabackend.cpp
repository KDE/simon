/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#include "alsabackend.h"
#include <unistd.h>
#include <simonsound/soundbackendclient.h>
#include <QThread>
#include <KLocalizedString>
#include <KDebug>
#include <KLocalizedString>

//Basic functions to set up alsa
static int set_hwparams(snd_pcm_t *handle,
                             snd_pcm_hw_params_t *params,
                             snd_pcm_access_t access,
                             int* bufferSize, int* periodSize, unsigned int* chunks,
                             int channels, unsigned int& samplerate);
static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams, int period_size);
static int xrun_recovery(snd_pcm_t *handle, int err);


class ALSALoop : public QThread {
  protected:
    ALSABackend *m_parent;
    bool shouldRun;
  public:
    ALSALoop(ALSABackend *parent) : m_parent(parent),
      shouldRun(true) 
    {}

    void stop() {
      shouldRun = false;
    }
};

//Capture loop
class ALSACaptureLoop : public ALSALoop
{
  public:
    ALSACaptureLoop(ALSABackend *parent) : ALSALoop(parent)
    {}

    void run()
    {
      shouldRun = true;

      int err = 0;
      snd_pcm_state_t state;
      short* buffer = (short*) malloc(sizeof(short)*m_parent->m_bufferSize);

      while ((err >= 0) && shouldRun) {
        err = 0;
        state = snd_pcm_state(m_parent->m_handle);

        if (state == SND_PCM_STATE_XRUN)
          err = xrun_recovery(m_parent->m_handle, -EPIPE);
        else if (state == SND_PCM_STATE_SUSPENDED)
          err = xrun_recovery(m_parent->m_handle, -ESTRPIPE);

        if (err < 0) {
          kWarning() << "XRUN / SUSPEND recovery failed: " << snd_strerror(err);
          break;
        }
        snd_pcm_sframes_t readCount = snd_pcm_readi(m_parent->m_handle, buffer, m_parent->m_bufferSize);
        if (readCount < 0) {
          kWarning() << "Read failed";
          break;
        }
        //kDebug() << "Recorded " << readCount << " samples of " << m_parent->m_periodSize;

        m_parent->m_client->writeData((char*) buffer, readCount*sizeof(short));
      }
      if (err < 0)
        m_parent->errorRecoveryFailed();

      m_parent->closeSoundSystem();
      free(buffer);
      shouldRun = false;
    }
};


//Playback loop
class ALSAPlaybackLoop : public ALSALoop
{
  public:
    ALSAPlaybackLoop(ALSABackend *parent) : ALSALoop(parent)
    {}

    void run()
    {
      shouldRun = true;

      int err = 0;

      snd_pcm_state_t state;
      int bufferSize = sizeof(short)*m_parent->m_bufferSize;
      short* buffer = (short*) malloc(bufferSize);

      while ((err >= 0) && shouldRun) {
        err = 0;
        state = snd_pcm_state(m_parent->m_handle);

        if (state == SND_PCM_STATE_XRUN)
          err = xrun_recovery(m_parent->m_handle, -EPIPE);
        else if (state == SND_PCM_STATE_SUSPENDED)
          err = xrun_recovery(m_parent->m_handle, -ESTRPIPE);

        if (err < 0) {
          kWarning() << "XRUN / SUSPEND recovery failed: " << snd_strerror(err);
          break;
        }

        qint64 written = m_parent->m_client->readData((char*) buffer, bufferSize);
        if (written == -1)
          break;

        snd_pcm_sframes_t writtenCount = snd_pcm_writei(m_parent->m_handle, buffer, written/sizeof(short));
        if (writtenCount < 0) {
          kWarning() << "Write failed";
          break;
        }
      }
      if (err < 0)
        m_parent->errorRecoveryFailed();

      free(buffer);
      m_parent->closeSoundSystem();

      shouldRun = false;
    }
};





ALSABackend::ALSABackend() : 
  m_handle(0),
  m_loop(0),
  m_bufferSize(1024)
{
}

int ALSABackend::bufferSize()
{
  return m_bufferSize;
}

QStringList ALSABackend::getAvailableInputDevices()
{
  return getDevices(SimonSound::Input);
}

QStringList ALSABackend::getAvailableOutputDevices()
{
  return getDevices(SimonSound::Output);
}

QStringList ALSABackend::getDevices(SimonSound::SoundDeviceType type)
{
  QStringList devices;
  devices << i18n("Default audio device (%1)", "default");

  char **hints, **hints_;

  if (snd_device_name_hint(-1, "pcm", (void***) &hints) < 0) {
    kWarning() << "List of devices is empty";
    return devices;
  }

  hints_ = hints;

  while (*hints) {
    char* device = *hints;

    char* name = snd_device_name_get_hint(device, "NAME");
    char* description = snd_device_name_get_hint(device, "DESC");
    char* ioid = snd_device_name_get_hint(device, "IOID");

    //is this device of the right type
    if ((ioid == 0) || (strcmp(ioid, (type == SimonSound::Input) ? "Input" : "Output") == 0)) {
      //add it to the list
      QString userDeviceName = QString("%1 (%2)").arg(description).arg(name).replace("\n", " ");
      if (userDeviceName.contains("Default Audio Device"))
        devices.insert(0, userDeviceName);
      else
        devices << userDeviceName;
    }

    free(name);
    free(description);
    free(ioid);

    hints++;
  }

  snd_device_name_free_hint((void**) hints_);

  return devices;
}

snd_pcm_t* ALSABackend::openDevice(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate)
{
  // remove everything up to (
  QByteArray internalDeviceName = device.mid(device.lastIndexOf("(")+1).toAscii();
  // remove )
  internalDeviceName = internalDeviceName.left(internalDeviceName.length()-1);

  kDebug() << "Opening device: " << internalDeviceName;

  snd_pcm_t *handle;
  snd_pcm_hw_params_t *hwparams;
  //snd_pcm_sw_params_t *swparams;

  snd_pcm_hw_params_alloca(&hwparams);
  //snd_pcm_sw_params_alloca(&swparams);

  int err = 0;

  if (snd_pcm_open(&handle, internalDeviceName.constData(), (type == SimonSound::Input) ? 
                      SND_PCM_STREAM_CAPTURE : SND_PCM_STREAM_PLAYBACK,
                      /*SND_PCM_NONBLOCK*/ 0) < 0) {
    kWarning() << "Couldn't open audio device: " << internalDeviceName << " - " 
               << snd_strerror(err);
    handle = 0;
  }

  unsigned int srate = static_cast<unsigned int>(samplerate);
  if (handle && (err = set_hwparams(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED,
          &m_bufferSize, &m_periodSize, &m_chunks,
          channels, srate)) < 0) {
    kWarning() << "Setting of hwparams failed: " << snd_strerror(err);
    snd_pcm_close(handle);
    handle = 0;
  }

  //if ((err >= 0) && ((err = set_swparams(handle, swparams, m_periodSize)) < 0)) {
    //kWarning() << "Setting of swparams failed: " << snd_strerror(err);
    //snd_pcm_close(handle);
    //handle = 0;
  //}

  //snd_pcm_hw_params_free(hwparams);
  //snd_pcm_sw_params_free(swparams);

  if (!handle)
    emit errorOccured(SimonSound::OpenError);

  return handle;
}

bool ALSABackend::check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate)
{
  snd_pcm_t *handle = openDevice(type, device, channels, samplerate);
  
  if (handle) {
    snd_pcm_close(handle);
    return true;
  }

  return false;
}

QString ALSABackend::getDefaultInputDevice()
{
  QString systemDefault = "default";
  if (getAvailableInputDevices().contains("pulse"))
    systemDefault = "pulse";
  return systemDefault;
}

QString ALSABackend::getDefaultOutputDevice()
{
  QString systemDefault = "default";
  if (getAvailableOutputDevices().contains("pulse"))
    systemDefault = "pulse";
  return systemDefault;
}

void ALSABackend::errorRecoveryFailed()
{
  emit errorOccured(SimonSound::FatalError);
  stopRecording();
}

// stop playback / recording
bool ALSABackend::stop()
{
  kDebug() << "Called stop()";
  if (state() != SimonSound::ActiveState)
    return true;

  Q_ASSERT(m_loop); //should be here if we are active
  Q_ASSERT(m_handle); 

  m_loop->stop();
  m_loop->wait();
  m_loop->deleteLater();
  m_loop = 0;

  return true;
}

bool ALSABackend::closeSoundSystem()
{
  snd_pcm_close(m_handle);
  emit stateChanged(SimonSound::IdleState);
  m_handle = 0;
  return true;
}

///////////////////////////////////////
// Recording  /////////////////////////
///////////////////////////////////////

bool ALSABackend::prepareRecording(const QString& device, int& channels, int& samplerate)
{
  if (m_handle || (m_loop && m_loop->isRunning())) {
    emit errorOccured(SimonSound::BackendBusy);
    return false;
  }

  m_handle = openDevice(SimonSound::Input, device, channels, samplerate);
  m_loop = new ALSACaptureLoop(this);
  emit stateChanged(SimonSound::PreparedState);
  kDebug() << "Prepared recording: " << m_handle;
  
  return m_handle;
}

bool ALSABackend::startRecording(SoundBackendClient *client)
{
  m_client = client;

  if (!m_handle || !m_loop) return false;

  //starting recording
  m_loop->start();
  emit stateChanged(SimonSound::ActiveState);
  kDebug() << "Started recording: " << m_handle;
  return true;
}

bool ALSABackend::stopRecording()
{
  kDebug() << "Stop recording";
  return stop();
}

///////////////////////////////////////
// Playback  //////////////////////////
///////////////////////////////////////

bool ALSABackend::preparePlayback(const QString& device, int& channels, int& samplerate)
{
  if (m_handle || (m_loop && m_loop->isRunning())) {
    emit errorOccured(SimonSound::BackendBusy);
    return false;
  }

  m_handle = openDevice(SimonSound::Output, device, channels, samplerate);
  m_loop = new ALSAPlaybackLoop(this);
  emit stateChanged(SimonSound::PreparedState);
  kDebug() << "Prepared playback: " << m_handle;
  
  return m_handle;
}

bool ALSABackend::startPlayback(SoundBackendClient *client)
{
  m_client = client;

  if (!m_handle || !m_loop) return false;

  //starting playback
  m_loop->start();
  emit stateChanged(SimonSound::ActiveState);
  kDebug() << "Started playback: " << m_handle;
  return true;
}

bool ALSABackend::stopPlayback()
{
  return stop();
}

ALSABackend::~ALSABackend()
{
  if (state() != SimonSound::IdleState)
    stop();
}











static int set_hwparams(snd_pcm_t *handle,
                             snd_pcm_hw_params_t *params,
                             snd_pcm_access_t access,
                             int* bufferSize, int* periodSize, unsigned int* chunks,
                             int channels, unsigned int& samplerate)
{
  unsigned int rrate;
  snd_pcm_uframes_t size;
  int err, dir;

  /* choose all parameters */
  err = snd_pcm_hw_params_any(handle, params);
  if (err < 0) {
    kWarning() << "Broken configuration for playback: no configurations available:" << snd_strerror(err);
    return err;
  }
  /* set hardware resampling */
  err = snd_pcm_hw_params_set_rate_resample(handle, params, true); //TODO: yeah?!
  if (err < 0) {
    kWarning() << "Resampling setup failed for playback:" << snd_strerror(err);
    return err;
  }
  /* set the interleaved read/write format */
  err = snd_pcm_hw_params_set_access(handle, params, access);
  if (err < 0) {
    kWarning() << "Access type not available for playback:" << snd_strerror(err);
    return err;
  }
  /* set the sample format */
  err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16);
  if (err < 0) {
    kWarning() << "Sample format not available for playback:" << snd_strerror(err);
    return err;
  }
  /* set the count of channels */
  err = snd_pcm_hw_params_set_channels(handle, params, channels);
  if (err < 0) {
    kWarning() << "Channels count (%i) not available for playbacks:" << channels << snd_strerror(err);
    return err;
  }
  /* set the stream rate */
  rrate = samplerate;
  err = snd_pcm_hw_params_set_rate_near(handle, params, &samplerate, 0);
  if (err < 0) {
    kWarning() << "Rate %iHz not available for playback:" << samplerate << snd_strerror(err);
    return err;
  }
  if (rrate != samplerate) {
    kWarning() << "Rate doesn't match (requested %iHz, get %iHz)\n" << samplerate << err;
    return -EINVAL;
  }
  /* set the buffer time */
  unsigned int buffer_time = 100000;
  err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, &dir);
  if (err < 0) {
    kWarning() << "Unable to set buffer time %i for playback:" << buffer_time << snd_strerror(err);
    return err;
  }

  err = snd_pcm_hw_params_get_buffer_size(params, &size);
  if (err < 0) {
    kWarning() << "Unable to get buffer size for playback:" << snd_strerror(err);
    return err;
  }
  kDebug() << "Selected buffer size: " << size;
  *bufferSize = size;
  
  unsigned int period_time = 20000;
  /* set the period time */
  err = snd_pcm_hw_params_set_period_time_near(handle, params, &period_time, &dir);
  if (err < 0) {
    kWarning() << "Unable to set period time %i for playback:" << period_time << snd_strerror(err);
    return err;
  }

  unsigned int chunks_ = 8;
  err = snd_pcm_hw_params_set_periods_near(handle, params, &chunks_, &dir);
  if ( err < 0 ) {
    kWarning() << "Unable to set periods: " << snd_strerror(err);
    return err;
  }
  *chunks = chunks_;

  err = snd_pcm_hw_params_get_period_size(params, &size, &dir);
  kDebug() << "Selected period size: " << size;
  if (err < 0) {
    kWarning() << "Unable to get period size for playback:" << snd_strerror(err);
    return err;
  }
  *periodSize = size;
  /* write the parameters to device */
  err = snd_pcm_hw_params(handle, params);
  if (err < 0) {
    kWarning() << "Unable to set hw params for playback:" << snd_strerror(err);
    return err;
  }
  return 0;
}

static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams, int period_size)
{
  int err;

  /* get the current swparams */
  err = snd_pcm_sw_params_current(handle, swparams);
  if (err < 0) {
    kDebug() << "Unable to determine current swparams for playback:" << snd_strerror(err);
    return err;
  }
  if ((err = snd_pcm_sw_params_set_avail_min (handle, swparams, period_size)) < 0) {
    kDebug() << "cannot set minimum available count " << snd_strerror(err);
    return err;
  }
  if ((err = snd_pcm_sw_params_set_start_threshold (handle, swparams, 0U)) < 0) {
    kDebug() << "cannot set start mode " << snd_strerror(err);
    return err;
  }
  /* write the parameters to the playback device */
  err = snd_pcm_sw_params(handle, swparams);
  if (err < 0) {
    kDebug() << "Unable to set sw params for playback:" << snd_strerror(err);
    return err;
  }
  return 0;
}

static int xrun_recovery(snd_pcm_t *handle, int err)
{
  if (err == -EPIPE) {    /* under-run */
    err = snd_pcm_prepare(handle);
    if (err < 0)
      kWarning() << "Can't recovery from underrun, prepare failed: %s\n" << snd_strerror(err);
    return 0;
  } else if (err == -ESTRPIPE) {
    while ((err = snd_pcm_resume(handle)) == -EAGAIN)
      sleep(1);       /* wait until the suspend flag is released */
    if (err < 0) {
      err = snd_pcm_prepare(handle);
      if (err < 0)
        kWarning() << "Can't recovery from suspend, prepare failed: %s\n" << snd_strerror(err);
    }
    return 0;
  }
  return err;
}

