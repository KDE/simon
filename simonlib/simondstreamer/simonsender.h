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

#ifndef SIMON_SIMONSENDER_H_0AC05651BE6A419EA6256220815A2AAD
#define SIMON_SIMONSENDER_H_0AC05651BE6A419EA6256220815A2AAD

#include <QtGlobal>
#include <QObject>
#include "simondstreamer_export.h"
#include <QTimer>

class QByteArray;

class SIMONDSTREAMER_EXPORT SimonSender : public QObject
{
  Q_OBJECT
  Q_CLASSINFO("simons audio streaming system with voice activity detection",
	      "org.simon-listens.SimonSender")

  public:
    enum SimonSenderState {
      Idle=0,
      Listening=1,
      Processing=2
    };
    SimonSenderState m_state;
  private:
    float m_currentPeak;
    QTimer loudnessTimer;
    
  private slots:
    void slotListening();
    void slotProcessing();
    void slotReceivedResults();
    
    void relayLoudness();
  
  signals:
    void recordingLevel(double peak);
    void listening();
    void processing();
    void receivedResults();
    
  protected:
    virtual void startSampleToRecognizePrivate(qint8 id, qint8 channels, qint32 sampleRate)=0;
    virtual void sendSampleToRecognizePrivate(qint8 id, const QByteArray& data)=0;
    virtual void recognizeSamplePrivate(qint8 id)=0;

  public:
    SimonSender();
    virtual ~SimonSender() {}
    void startSampleToRecognize(qint8 id, qint8 channels, qint32 sampleRate);
    void sendSampleToRecognize(qint8 id, const QByteArray& data);
    void recognizeSample(qint8 id);
    
    void clientLoudness(qint8 id, float currentPeak);
};
#endif
