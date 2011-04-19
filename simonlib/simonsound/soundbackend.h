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

#ifndef SIMON_SOUNDBACKEND_H_BAC60651BE6A419EA6156220815A2AAD
#define SIMON_SOUNDBACKEND_H_BAC60651BE6A419EA6156220815A2AAD

#include <simonsound/simonsound.h>
#include <QStringList>
#include <QObject>

class SoundBackendClient;

class SoundBackend : public QObject
{
  Q_OBJECT

  signals:
    void errorOccured(SimonSound::Error);
    void stateChanged(SimonSound::State);

  protected:
    SoundBackendClient *m_client;

    SoundBackend();

  private:
    SimonSound::Error m_error;
    SimonSound::State m_state;

  private slots:
    void errorSlot(SimonSound::Error);
    void stateSlot(SimonSound::State);

  public:
    //Factory method
    static SoundBackend* createObject();

    //virtual destructor
    virtual ~SoundBackend() {}

    //device listing / checking
    virtual QStringList getAvailableInputDevices()=0;
    virtual QStringList getAvailableOutputDevices()=0;
    virtual bool check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate)=0;

    virtual QString getDefaultInputDevice()=0;
    virtual QString getDefaultOutputDevice()=0;

    //general information
    virtual int bufferSize()=0;

    SimonSound::Error error();
    SimonSound::State state();

    //recording
    virtual bool prepareRecording(const QString& device, int& channels, int& samplerate)=0;
    virtual bool startRecording(SoundBackendClient *client)=0;
    virtual bool stopRecording()=0;

    //playback
    virtual bool preparePlayback(const QString& device, int& channels, int& samplerate)=0;
    virtual bool startPlayback(SoundBackendClient *client)=0;
    virtual bool stopPlayback()=0;
};

#endif

