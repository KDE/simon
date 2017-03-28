/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_ALSABACKEND_H_BAC60651BE6A419EA6156220815A2AAD
#define SIMON_ALSABACKEND_H_BAC60651BE6A419EA6156220815A2AAD

#include <simonsound/simonsound.h>
#include <simonsound/soundbackend.h>
#include <alsa/asoundlib.h>
#include <QStringList>
#include <QMutex>

class ALSALoop;
class ALSACaptureLoop;
class ALSAPlaybackLoop;

class ALSABackend : public SoundBackend
{
  friend class ALSAPlaybackLoop;
  friend class ALSACaptureLoop;

  private:
    QMutex m_deviceListLock;

    snd_pcm_t *m_handle;
    ALSALoop *m_loop;

    int m_bufferSize;
    int m_periodSize;
    unsigned int m_chunks;
    QStringList getDevices(SimonSound::SoundDeviceType type);

    snd_pcm_t* openDevice(SimonSound::SoundDeviceType type, 
        const QString& device, int channels, int samplerate);
    QString defaultDevice(const QStringList& list);

  protected:
    void errorRecoveryFailed();
    bool stop();
    bool closeSoundSystem();

  public:
    ALSABackend();
    ~ALSABackend();

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
};

#endif


