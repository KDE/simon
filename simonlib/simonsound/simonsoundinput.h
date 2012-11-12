/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SIMONSOUNDINPUT_H_BAC62651BE6A419EA6156220815A2AAD
#define SIMON_SIMONSOUNDINPUT_H_BAC62651BE6A419EA6156220815A2AAD

class SoundInputBuffer;
class QAudioInput;

#include <simonsound/simonsound.h>
#include <simonsound/soundclient.h>
#include <simonsound/soundbackendclient.h>
#include <QHash>
#include <QObject>
#include <QMutex>

class SoundInputClient;
class SoundBackend;

class SimonSoundInput : public QObject, public SoundBackendClient
{
  Q_OBJECT

  signals:
    void recordingFinished();
    void error(const QString& str);
    void inputStateChanged(SimonSound::State state);

  private:
    QMutex m_lock;
    SimonSound::DeviceConfiguration m_device;
    SoundBackend *m_input;
    QHash<SoundInputClient*, qint64> m_activeInputClients;
    QHash<SoundInputClient*, qint64> m_suspendedInputClients;
    SoundInputBuffer *m_buffer;

    void killBuffer();

  protected:
    qint64 writeData(const char *toWrite, qint64 len);

  private slots:
    void slotInputStateChanged(SimonSound::State state);

  public:
    SimonSoundInput(QObject *parent=0);

    void registerInputClient(SoundInputClient* client);

    bool deRegisterInputClient(SoundInputClient* client);

    bool prepareRecording(SimonSound::DeviceConfiguration& device);
    bool startRecording();
    bool stopRecording();

    SoundClient::SoundClientPriority getHighestPriority();
    bool activate(SoundClient::SoundClientPriority priority);

    bool isActive() { return (m_activeInputClients.count() > 0); }

    int bufferSize();
    void processData(const QByteArray& data);

    void suspend(SoundInputClient*);
    void resume(SoundInputClient*);
    void suspendInputClients();
    ~SimonSoundInput();
};
#endif
