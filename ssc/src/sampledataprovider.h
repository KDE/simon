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

#ifndef SIMON_SAMPLEDATAPROVIDER_H_58DB5F6A2C9049A79FFCD02D32604B02
#define SIMON_SAMPLEDATAPROVIDER_H_58DB5F6A2C9049A79FFCD02D32604B02

#include <QList>
#include "trainingswizard.h"

class DeviceInformationPage;
class TrainSamplePage;
class Sample;
class Microphone;
class SoundCard;

/**
 *	@class SampleDataProvider
 *	@brief Combines all data gathered from one training session
 *
 *	@date 22.05.2010
 *	@author Peter Grasch
 */
class SampleDataProvider
{

  private:
    qint32 m_userId;
    TrainingsWizard::TrainingsType m_sampleType;
    QString m_name;
    DeviceInformationPage* m_infoPage;
    QList<TrainSamplePage*> m_trainSamplePages;

    QList<Sample*> m_samplesToTransmit;

    QHash<QString, Microphone*> buildMicrophoneMappings(bool &ok);
    QHash<QString, SoundCard*> buildSoundCardMappings(bool &ok);

  public:
    SampleDataProvider(qint32 userId, TrainingsWizard::TrainingsType sampleType, const QString& name);

    void registerMicrophoneInfo(DeviceInformationPage* infoPage);
    void registerDataProvider(TrainSamplePage* trainSamplePage);

    bool startTransmission();
    bool store();

    bool hasSamplesToTransmit()
      { return !m_samplesToTransmit.isEmpty(); }

    int sampleToTransmitCount()
      { return m_samplesToTransmit.count(); }

    Sample* getSample();
    void sampleTransmitted();

    void stopTransmission();

    ~SampleDataProvider();
    void skipSample();

};
#endif
