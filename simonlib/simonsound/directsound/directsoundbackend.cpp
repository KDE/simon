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

#include <unistd.h>
#include <simonsound/soundbackendclient.h>
#include <QThread>
#include <KDebug>
#include <KLocalizedString>
#include "directsoundbackend.h"

#define SAFE_RELEASE(x) if (x) { x->Release(); x = 0; }

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

      //init first run

      DWORD dwRetSamples = 0, dwRetBytes = 0;
      //get audio samples
      m_lpGETAUDIOSAMPLES(m_lpAudioBuf, m_WFE.nSamplesPerSec, dwRetSamples, m_lpData);
      dwRetBytes = dwRetSamples*m_WFE.nBlockAlign;

      //Write the audio data to DirectSoundBuffer
      LPVOID lpvAudio1 = NULL, lpvAudio2 = NULL;
      DWORD dwBytesAudio1 = 0, dwBytesAudio2 = 0; 

      //Lock DirectSoundBuffer
      HRESULT hr = m_lpDSB->Lock(0, m_WFE.nAvgBytesPerSec, &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0);
      if ( FAILED(hr) ) {
      m_strLastError = _T("Lock DirectSoundBuffer Failed!");
      OutputDebugString(m_strLastError);
      return;
      }

      //Init lpvAudio1
      if (NULL != lpvAudio1) {      
      memset(lpvAudio1, 0, dwBytesAudio1);      
      }

      //Init lpvAudio2
      if (NULL != lpvAudio2) {      
      memset(lpvAudio2, 0, dwBytesAudio2);      
      }

      //Copy Audio Buffer to DirectSoundBuffer
      if (NULL == lpvAudio2) {
      memcpy(lpvAudio1, m_lpAudioBuf, dwRetBytes);
      }
      else {
      memcpy(lpvAudio1, m_lpAudioBuf, dwBytesAudio1);
      memcpy(lpvAudio2, m_lpAudioBuf + dwBytesAudio1, dwBytesAudio2);
      }
          
      //Unlock DirectSoundBuffer
      m_lpDSB->Unlock(lpvAudio1, dwBytesAudio1, lpvAudio2, dwBytesAudio2);


      //Beging Play
      m_lpDSB->Play(0, 0, DSBPLAY_LOOPING);
      
      //timeSetEvent
      m_timerID = timeSetEvent(300, 100, TimerProcess, (DWORD)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);

      while (shouldRun) {

        Sleep(300);
      }
      shouldRun = false;
    }
};





DirectSoundBackend::DirectSoundBackend() : 
  m_loop(0),
  m_audioBuffer(0),
  m_handle(0),
  m_primaryBuffer(0),
  m_secondaryBuffer(0),
  m_buffer1Circles(0),
  m_buffer2Circles(0),
  m_bufferSize(1024)
{
  ZeroMemory(&m_waveFormat, sizeof(m_waveFormat));
  m_bufferEvents[0] = CreateEvent(NULL, FALSE, FALSE, _T("Direct_Sound_Buffer_Notify_0"));
  m_bufferEvents[1] = CreateEvent(NULL, FALSE, FALSE, _T("Direct_Sound_Buffer_Notify_1"));
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

bool DirectSoundBackend::openDevice(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate, LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER *primaryBuffer, LPDIRECTSOUNDBUFFER8 *secondaryBuffer)
{
  DSBUFFERDESC BufferDesc;

  //GET GUID
  // remove everything up to (
  QString internalDeviceName = device.mid(device.lastIndexOf("(")+1);
  // remove )
  internalDeviceName = internalDeviceName.left(internalDeviceName.length()-1);

  kDebug() << "Opening device: " << internalDeviceName; // contains the GUID or "" for default
  wchar_t internalDeviceNameW[internalDeviceName.length()*sizeof(wchar_t)+1];
  internalDeviceNameW[internalDeviceName.toWCharArray(internalDeviceNameW)] = '\0';

  LPGUID deviceID;
  HRESULT err = CLSIDFromString(internalDeviceNameW, (LPCLSID) deviceID);
  if (err != NOERROR) {
    kWarning() << "Couldn't parse: " << internalDeviceName << err;
    return false;
  }

  // Generate DirectSound-Interface
  if (FAILED(DirectSoundCreate8(deviceID, ppDS8, 0))) {
    kWarning() << "Failed to open device";
    *ppDS8 = 0;
    emit errorOccured(SimonSound::OpenError);
    return false;
  }


  HWND hWnd = GetForegroundWindow();
  if (hWnd == NULL) {
    hWnd = GetDesktopWindow();
  }

  (*ppDS8)->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

  // Fill DSBUFFERDESC-structure
  BufferDesc.dwSize     = sizeof(DSBUFFERDESC);
  BufferDesc.dwFlags      = DSBCAPS_PRIMARYBUFFER;
  BufferDesc.dwBufferBytes  = 0;
  BufferDesc.dwReserved   = 0;
  BufferDesc.lpwfxFormat    = 0;
  BufferDesc.guid3DAlgorithm  = GUID_NULL;

  // Creating primary sound buffer
  if(FAILED((*ppDS8)->CreateSoundBuffer(&BufferDesc,
            primaryBuffer, 0))) {
    kDebug() << "Failed to create primary sound buffer";
    (*ppDS8)->Release();
    *ppDS8 = 0;
    *primaryBuffer = 0;
    return false;
  }

  //init sound
  //DSBUFFERDESC    BufferDesc;
  LPDIRECTSOUNDBUFFER pTemp;
  //SSample*      pSamples;
  //DWORD       dwNumBytes;
  //float       fTime;
  //float       fLeft;
  //float       fRight;

  // audioformat
  m_waveFormat.wFormatTag   = WAVE_FORMAT_PCM;
  m_waveFormat.nChannels    = channels;
  m_waveFormat.nSamplesPerSec = samplerate;
  m_waveFormat.wBitsPerSample = 16; //S16_LE
  m_waveFormat.nBlockAlign    = m_waveFormat.nChannels * (m_waveFormat.wBitsPerSample >> 3);
  m_waveFormat.nAvgBytesPerSec  = m_waveFormat.nSamplesPerSec * m_waveFormat.nBlockAlign;
  m_waveFormat.cbSize     = 0;

  // DSBUFFERDESC
  BufferDesc.dwSize     = sizeof(DSBUFFERDESC);
  BufferDesc.dwFlags      = DSBCAPS_CTRLPOSITIONNOTIFY |
                            DSBCAPS_CTRLFREQUENCY |
                            DSBCAPS_GLOBALFOCUS;
  BufferDesc.dwBufferBytes  = m_waveFormat.nAvgBytesPerSec * 2; // 2 seconds of sound
  BufferDesc.dwReserved   = 0;
  BufferDesc.lpwfxFormat    = &m_waveFormat;
  BufferDesc.guid3DAlgorithm  = GUID_NULL;

  // Sound buffer
  if(FAILED((*ppDS8)->CreateSoundBuffer(&BufferDesc,
              &pTemp, 0)))
  {
    kWarning() << "Couldn't create sound buffer.";
    (*ppDS8)->Release();
    *ppDS8 = 0;
    (*primaryBuffer)->Release();
    *primaryBuffer = 0;
    emit errorOccured(SimonSound::OpenError);
    return false;
  }

  pTemp->QueryInterface(IID_IDirectSoundBuffer8, (void**)(secondaryBuffer));
  SAFE_RELEASE(pTemp);


  //calculate notify positions
  DSBPOSITIONNOTIFY pPosNotify[2];
  pPosNotify[0].dwOffset = m_waveFormat.nAvgBytesPerSec/2 - 1;
  pPosNotify[1].dwOffset = 3*m_waveFormat.nAvgBytesPerSec/2 - 1;   
  pPosNotify[0].hEventNotify = m_bufferEvents[0];
  pPosNotify[1].hEventNotify = m_bufferEvents[1];  

  if ( FAILED(lpDSBNotify->SetNotificationPositions(2, pPosNotify)) ) {
    kWarning() << "Set NotificationPosition Failed!";
    freeAllResources();
    return false;
  } 

  //New audio buffer
  //if (m_audioBuffer != 0)
  delete[] m_audioBuffer;
  m_audioBuffer = new BYTE[m_waveFormat.nAvgBytesPerSec];

  //Init Audio Buffer
  memset(m_audioBuffer, 0, m_waveFormat.nAvgBytesPerSec);
  return true;
}

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

void DirectSoundBackend::freeAllResources()
{
  //destroying DS members
  (*m_secondaryBuffer)->Release();
  *m_secondaryBuffer = 0;
  (*m_primaryBuffer)->Release();
  *m_primaryBuffer = 0;
  (*m_handle)->Release();
  *m_handle = 0;

  delete[] m_audioBuffer;
  m_audioBuffer = NULL;    
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

  freeAllResources();

  m_buffer1Circles = 0;
  m_buffer2Circles = 0;

  emit stateChanged(SimonSound::IdleState);
  return true;
}

///////////////////////////////////////
// Recording  /////////////////////////
///////////////////////////////////////

bool DirectSoundBackend::prepareRecording(const QString& device, int& channels, int& samplerate)
{
  if (m_handle || (m_loop && m_loop->isRunning())) {
    emit errorOccured(SimonSound::BackendBusy);
    return false;
  }

  if (!openDevice(SimonSound::Input, device, channels, samplerate, &m_handle, &m_primaryBuffer, &m_secondaryBuffer)) {
    return false;
  }

  m_loop = new DirectSoundCaptureLoop(this);
  emit stateChanged(SimonSound::PreparedState);
  kDebug() << "Prepared recording: " << m_handle;
  
  return true;
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
  if (m_handle || (m_loop && m_loop->isRunning())) {
    emit errorOccured(SimonSound::BackendBusy);
    return false;
  }

  if (!openDevice(SimonSound::Input, device, channels, samplerate, &m_handle, &m_primaryBuffer, &m_secondaryBuffer)) {
    return false;
  }

  m_loop = new DirectSoundPlaybackLoop(this);
  emit stateChanged(SimonSound::PreparedState);
  kDebug() << "Prepared recording: " << m_handle;
  
  return false;
}

bool DirectSoundBackend::startPlayback(SoundBackendClient *client)
{
  m_client = client;

  if (!m_handle || !m_loop) return false;

  //starting playback
  m_loop->start();
  emit stateChanged(SimonSound::ActiveState);
  kDebug() << "Started playback: " << m_handle;
  return true;
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
