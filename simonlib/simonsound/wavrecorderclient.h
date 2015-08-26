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

#ifndef SIMON_WAVRECORDERCLIENT_H_0AC60651BE6A419EA6256220815A2AAD
#define SIMON_WAVRECORDERCLIENT_H_0AC60651BE6A419EA6256220815A2AAD

#include <QObject>
#include "soundinputclient.h"

class WAV;
class VADSoundProcessor;

class WavRecorderClient :public QObject, public SoundInputClient
{
  Q_OBJECT

  private:
    WAV *wavData;
    VADSoundProcessor *vad;

  signals:
    void currentProgress(int msecs, float level);
    void clippingOccured();
    void signalToNoiseRatioLow();

    void speaking();
    void speakingStopped();

  public:
    explicit WavRecorderClient(const SimonSound::DeviceConfiguration& deviceConfiguration, QObject *parent=0);
    bool finish();
    bool record(QString filename);

    void processPrivate(const QByteArray& data, qint64 currentTime);

    virtual ~WavRecorderClient();

};
#endif
