/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_NULLRECORDERCLIENT_H_0AC60651BE6A419EA6256220815A2AAD
#define SIMON_NULLRECORDERCLIENT_H_0AC60651BE6A419EA6256220815A2AAD

#include <QObject>
#include <QTimer>
#include "soundinputclient.h"

class WAV;
class VADSoundProcessor;

class NullRecorderClient :public QObject, public SoundInputClient
{
  Q_OBJECT

  private:
    VADSoundProcessor *vad;

  signals:
    void sampleStarted();
    void sampleCompleted();
    void level(qint64 time, float now);
    void clippingOccured();

  public:
    explicit NullRecorderClient(const SimonSound::DeviceConfiguration& deviceConfiguration, SoundClientPriority options, QObject *parent=0);
    bool start();
    bool finish();

    void processPrivate(const QByteArray& data, qint64 currentTime);

    virtual ~NullRecorderClient();

};
#endif
