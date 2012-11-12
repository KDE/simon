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

#ifndef SIMON_SOUNDSERVER_H_BAC60651BE6A419EA6156220815A2AAD
#define SIMON_SOUNDSERVER_H_BAC60651BE6A419EA6156220815A2AAD

#include <simonsound/simonsound.h>
#include <QIODevice>
#include <QHash>
#include <QList>
#include <QMutex>
#include "simonsound_export.h"

class SoundInputClient;
class SoundOutputClient;
class SimonSoundInput;
class SimonSoundOutput;
class SoundBackend;

class SIMONSOUND_EXPORT SoundServer : public QObject
{
  Q_OBJECT

  signals:
    void error(const QString& str);
    void devicesChanged();

  private:
    static SoundServer* instance;

    SoundBackend *backend;
    QMutex inputRegistrationLock;
    QMutex outputRegistrationLock;

    QHash<SimonSound::DeviceConfiguration, SimonSoundInput*> inputs;

    QHash<SimonSound::DeviceConfiguration, SimonSoundOutput*> outputs;
    static QList<SimonSound::DeviceConfiguration> getInputDevices(SimonSound::SoundDeviceUses uses);
    static QList<SimonSound::DeviceConfiguration> getOutputDevices(SimonSound::SoundDeviceUses uses);

    void applyInputPriorities();
    void applyOutputPriorities();

    void initializeDevices();
    SoundServer(QObject *parent=0);

    qint64 getDeviceLengthFactor(SimonSound::DeviceConfiguration device);
    QStringList getDevicesPrivate(SimonSound::SoundDeviceType type);
    QString defaultInputDevicePrivate();
    QString defaultOutputDevicePrivate();

  public:
    static SoundServer* getInstance();

    bool registerInputClient(SoundInputClient* client);
    bool deRegisterInputClient(SoundInputClient* client);

    bool registerOutputClient(SoundOutputClient* client);
    bool deRegisterOutputClient(SoundOutputClient* client);

    qint64 byteSizeToLength(qint64 bytes, SimonSound::DeviceConfiguration device);
    qint64 lengthToByteSize(qint64 length, SimonSound::DeviceConfiguration device);

    bool reinitializeDevices();

    int getInputDeviceCount();
    int getOutputDeviceCount();

    bool check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate);

    static bool getDefaultToPowerTraining();
    static bool getCalibrateVolume();

    static int getLevelThreshold();
    static int getHeadMargin();
    static int getTailMargin();
    static int getShortSampleCutoff();

    static QString defaultInputDevice();
    static QString defaultSampleGroup();
    static QString defaultOutputDevice();

    static QList<SimonSound::DeviceConfiguration> getTrainingInputDevices();
    static QList<SimonSound::DeviceConfiguration> getRecognitionInputDevices();

    static QList<SimonSound::DeviceConfiguration> getTrainingOutputDevices();

    static QStringList getDevices(SimonSound::SoundDeviceType type);
    static QStringList getInputDeviceDefaultSampleGroups();
    
    void closeOutput(SimonSoundOutput* client);
    
    void uninitializeSoundSystem();

    virtual ~SoundServer();

};
#endif
