/*
 *   Copyright (C) 2011 Patrick von Reth <patrick.vonreth@gmail.com>
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

#ifndef SIMON_DIRECTSOUNDBACKEND_H_BAC60651BE6A419EA6156220815A2AAD
#define SIMON_DIRECTSOUNDBACKEND_H_BAC60651BE6A419EA6156220815A2AAD

#include <simonsound/simonsound.h>
#include <simonsound/soundbackend.h>
#include <QStringList>
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

#ifdef __MINGW64_VERSION_MAJOR
    //libdxerr.a is missing on mingw so no error support :(
    #define DXGetErrorString(x) x
#else
    #include <dxerr.h>
#endif


class DirectSoundLoop;
class DirectSoundCaptureLoop;
class DirectSoundPlaybackLoop;

class DirectSoundBackend : public SoundBackend
{
  friend class DirectSoundPlaybackLoop;
  friend class DirectSoundCaptureLoop;

  private:
    DirectSoundLoop *m_loop;

    HANDLE m_bufferEvents[2];
    WAVEFORMATEX    m_waveFormat;

    LPBYTE m_audioBuffer;
    LPDIRECTSOUNDNOTIFY m_notify;

    LPDIRECTSOUND8 m_handle;
    LPDIRECTSOUNDBUFFER m_primaryBuffer;
    LPDIRECTSOUNDBUFFER m_secondaryBuffer;

    LPDIRECTSOUNDCAPTURE8 m_handleC;
    LPDIRECTSOUNDCAPTUREBUFFER m_primaryBufferC;

    QStringList m_devices;

    int m_bufferSize;
    int m_blockAlign;
    int m_sampleRate;

    QStringList getDevices(SimonSound::SoundDeviceType type);

  protected:
    void errorRecoveryFailed();
    void freeAllResources();
    bool stop();
    bool closeSoundSystem();

	bool openOutputDevice(GUID *deviceID,LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER *primaryBuffer,LPDIRECTSOUNDBUFFER *secondaryBuffer,LPDIRECTSOUNDNOTIFY *notify);

	bool openInputDevice(GUID *deviceID,LPDIRECTSOUNDCAPTURE8* ppDS8C, LPDIRECTSOUNDCAPTUREBUFFER *primaryBufferC);

    bool openDevice(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate, 
        LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER *primaryBuffer, LPDIRECTSOUNDBUFFER *secondaryBuffer, 
        LPDIRECTSOUNDCAPTURE8* ppDS8C, LPDIRECTSOUNDCAPTUREBUFFER *primaryBufferC, 
        LPDIRECTSOUNDNOTIFY *notify);

  public:
    DirectSoundBackend();
    ~DirectSoundBackend();

    QStringList getAvailableInputDevices();
    QStringList getAvailableOutputDevices();
    bool check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate);

    QString getDefaultInputDevice();
    QString getDefaultOutputDevice();

    bool prepareRecording(const QString& device, int& channels, int& samplerate);
    bool startRecording(SoundBackendClient *client);
    bool stopRecording();

    bool preparePlayback(const QString& device, int& channels, int& samplerate);
    bool startPlayback(SoundBackendClient *client);
    bool stopPlayback();

    int bufferSize();

    void deviceFound(const QString& name);
};

#endif

