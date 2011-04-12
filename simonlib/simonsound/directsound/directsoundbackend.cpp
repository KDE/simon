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

#include "directsoundbackend.h"
#include <unistd.h>
#include <simonsound/soundbackendclient.h>
#include <QThread>
#include <KDebug>
#include <KLocalizedString>


class DirectSoundLoop : public QThread {
  protected:
    DirectSoundBackend *m_parent;
    bool shouldRun;
  public:
    DirectSoundLoop(DirectSoundBackend *parent) : m_parent(parent),
      shouldRun(true) 
    {}

    void stop() {
      shouldRun = false;
    }
};

//Capture loop
class DirectSoundCaptureLoop : public DirectSoundLoop
{
  public:
    DirectSoundCaptureLoop(DirectSoundBackend *parent) : DirectSoundLoop(parent)
    {}

    void run()
    {
      shouldRun = true;

      shouldRun = false;
    }
};


//Playback loop
class DirectSoundPlaybackLoop : public DirectSoundLoop
{
  public:
    DirectSoundPlaybackLoop(DirectSoundBackend *parent) : DirectSoundLoop(parent)
    {}

    void run()
    {
      shouldRun = true;

      shouldRun = false;
    }
};





DirectSoundBackend::DirectSoundBackend() : 
  m_loop(0),
  m_handle(0),
  m_primaryBuffer(0),
  m_secondaryBuffer(0),
  m_bufferSize(1024)
{
}

int DirectSoundBackend::bufferSize()
{
  return m_bufferSize;
}

QStringList DirectSoundBackend::getAvailableInputDevices()
{
  return getDevices(SimonSound::Input);
}

QStringList DirectSoundBackend::getAvailableOutputDevices()
{
  return getDevices(SimonSound::Output);
}

BOOL CALLBACK DirectSoundEnumerateCallback(LPGUID pGUID, LPCWSTR pcName, LPCWSTR pcDriver, LPVOID pContext)
{
  if (!pGUID) 
  {
    //primary sound driver
    return TRUE;
  }
  OLECHAR lplpsz[40]={0};
  StringFromGUID2(*pGUID, lplpsz, 40);
  QString uid = QString::fromWCharArray(lplpsz);

  QString deviceName(QString::fromWCharArray(pcName));
  deviceName += QString(" (%1)").arg(uid);

  kDebug() << "Found device: " << deviceName;
  ((DirectSoundBackend*) pContext)->deviceFound(deviceName);
  return TRUE;
}

void DirectSoundBackend::deviceFound(const QString& name)
{
  m_devices << name;
}

QStringList DirectSoundBackend::getDevices(SimonSound::SoundDeviceType type)
{
  Q_UNUSED(type);
  m_devices.clear();

  m_devices << i18n("Default audio device");

  DirectSoundEnumerate(DirectSoundEnumerateCallback, this);

  Sleep(1000); // yes, this is the ugliest hack in the universe
  return m_devices;
}

bool openDevice(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate, LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER *primaryBuffer, LPDIRECTSOUNDBUFFER8 *secondaryBuffer)
{
  DSBUFFERDESC BufferDesc;

  //GET GUID
  // remove everything up to (
  QByteArray internalDeviceName = device.mid(device.lastIndexOf("(")+1).toAscii();
  // remove )
  internalDeviceName = internalDeviceName.left(internalDeviceName.length()-1);

  kDebug() << "Opening device: " << internalDeviceName; // contains the GUID or "" for default
  LPGUID deviceID;
  HRESULT err = CLSIDFromString(internalDeviceName.toWCharArray(), (LPCLSID) deviceID);
  if (err != NOERROR) {
    kWarning() << "Couldn't parse: " << internalDeviceName << err;
    return false;
  }

  // Generate DirectSound-Interface
  if(FAILED(DirectSoundCreate8(deviceID, ppDS8, NULL))) {
    kWarning() << "Failed to open device";
    emit errorOccured(SimonSound::OpenError);
    return false;
  }

  (*ppDS8)->SetCooperativeLevel(NULL /*WTF?*/, DSSCL_PRIORITY);

  // Fill DSBUFFERDESC-structure
  BufferDesc.dwSize     = sizeof(DSBUFFERDESC);
  BufferDesc.dwFlags      = DSBCAPS_PRIMARYBUFFER;
  BufferDesc.dwBufferBytes  = 0;
  BufferDesc.dwReserved   = 0;
  BufferDesc.lpwfxFormat    = NULL;
  BufferDesc.guid3DAlgorithm  = GUID_NULL;

  // Creating primary sound buffer
  if(FAILED((*ppDS8)->CreateSoundBuffer(&BufferDesc,
            primaryBuffer, NULL))) {
    kDebug() << "Failed to create primary sound buffer
    return false;
  }

  //init sound
  WAVEFORMATEX    WaveFormat;
  DSBUFFERDESC    BufferDesc;
  LPDIRECTSOUNDBUFFER pTemp;
  SSample*      pSamples;
  DWORD       dwNumBytes;
  float       fTime;
  float       fLeft;
  float       fRight;

  // Audioformat ausfüllen
  WaveFormat.wFormatTag   = WAVE_FORMAT_PCM;
  WaveFormat.nChannels    = channel;
  WaveFormat.nSamplesPerSec = samplerate;
  WaveFormat.wBitsPerSample = 16; //S16_LE
  WaveFormat.nBlockAlign    = WaveFormat.nChannels * (WaveFormat.wBitsPerSample >> 3);
  WaveFormat.nAvgBytesPerSec  = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
  WaveFormat.cbSize     = 0;

  // DSBUFFERDESC-Struktur ausfüllen
  BufferDesc.dwSize     = sizeof(DSBUFFERDESC);
  BufferDesc.dwFlags      = DSBCAPS_LOCDEFER |
                            DSBCAPS_CTRLVOLUME |
                            DSBCAPS_CTRLPAN |
                            DSBCAPS_CTRLFREQUENCY |
                            DSBCAPS_GLOBALFOCUS;
  BufferDesc.dwBufferBytes  = WaveFormat.nAvgBytesPerSec * 5; // 5 Sekunden Sound
  BufferDesc.dwReserved   = 0;
  BufferDesc.lpwfxFormat    = &WaveFormat;
  BufferDesc.guid3DAlgorithm  = GUID_NULL;

  // Soundpuffer erstellen
  if(FAILED((*ppDS8)->CreateSoundBuffer(&BufferDesc,
              &pTemp, NULL)))
  {
    kWarning() << "Couldn't create sound buffer.";
    emit errorOccured(SimonSound::OpenError);
    return TB_ERROR;
  }

  // 8er-Schnittstelle abfragen und die alte löschen
  pTemp->QueryInterface(IID_IDirectSoundBuffer8, (void**)(secondaryBuffer));
  TB_SAFE_RELEASE(pTemp);
  return true;
}

/*
snd_pcm_t* DirectSoundBackend::openDevice(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate)
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
                      0) < 0) {
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
*/

bool DirectSoundBackend::check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate)
{
  return true; // FIXME? or does windows resample automatically?
}

QString DirectSoundBackend::getDefaultInputDevice()
{
  return i18n("Default audio device");
}

QString DirectSoundBackend::getDefaultOutputDevice()
{
  return i18n("Default audio device");
}

void DirectSoundBackend::errorRecoveryFailed()
{
  emit errorOccured(SimonSound::FatalError);
  stopRecording();
}

// stop playback / recording
bool DirectSoundBackend::stop()
{
  if (state() != SimonSound::ActiveState)
    return true;

  Q_ASSERT(m_loop); //should be here if we are active

  m_loop->stop();
  m_loop->wait();
  m_loop->deleteLater();
  m_loop = 0;

  emit stateChanged(SimonSound::IdleState);
  return true;
}

///////////////////////////////////////
// Recording  /////////////////////////
///////////////////////////////////////

bool DirectSoundBackend::prepareRecording(const QString& device, int& channels, int& samplerate)
{
  
  return false;
}

bool DirectSoundBackend::startRecording(SoundBackendClient *client)
{
  m_client = client;

  return false;
}

bool DirectSoundBackend::stopRecording()
{
  return stop();
}

///////////////////////////////////////
// Playback  //////////////////////////
///////////////////////////////////////

bool DirectSoundBackend::preparePlayback(const QString& device, int& channels, int& samplerate)
{
  return false;
}

bool DirectSoundBackend::startPlayback(SoundBackendClient *client)
{
  return false;
}

bool DirectSoundBackend::stopPlayback()
{
  return stop();
}

DirectSoundBackend::~DirectSoundBackend()
{
  if (state() != SimonSound::IdleState)
    stop();
}
