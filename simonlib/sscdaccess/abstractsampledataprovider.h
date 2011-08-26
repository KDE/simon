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

#ifndef SIMON_ABSTRACTSAMPLEDATAPROVIDER_H_58DB5F6A2C9049A79FFCD02D32604B02
#define SIMON_ABSTRACTSAMPLEDATAPROVIDER_H_58DB5F6A2C9049A79FFCD02D32604B02

#include "sscdaccess_export.h"
#include <QList>
#include <QHash>
#include <sscobjects/sample.h>

class Sample;
class Microphone;
class SoundCard;
class TrainingSamplesDescriptor;

class SSCDACCESS_EXPORT AbstractSampleDataProvider
{
  protected:
    qint32 m_userId;
    Sample::SampleType m_sampleType;
    QString m_name;
    QList<Sample*> m_samplesToTransmit;
    bool m_keepSamples;

    virtual QHash<QString, Microphone*> buildMicrophoneMappings(bool &ok)=0;
    virtual QHash<QString, SoundCard*> buildSoundCardMappings(bool &ok)=0;
    virtual QList<TrainingSamplesDescriptor*> buildSampleDescriptors(bool &ok)=0;

  public:
    AbstractSampleDataProvider(qint32 userId, Sample::SampleType sampleType, const QString& name, bool keepSamples);
    virtual ~AbstractSampleDataProvider();

    bool startTransmission();
    virtual bool store()=0;

    bool hasSamplesToTransmit()
      { return !m_samplesToTransmit.isEmpty(); }

    int sampleToTransmitCount()
      { return m_samplesToTransmit.count(); }

    Sample* getSample();
    void sampleTransmitted();

    void skipSample();
    void stopTransmission();


};
#endif
