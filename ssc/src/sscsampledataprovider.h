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

#ifndef SIMON_SSCSAMPLEDATAPROVIDER_H_58DB5F6A2C9049A79FFCD02D32604B02
#define SIMON_SSCSAMPLEDATAPROVIDER_H_58DB5F6A2C9049A79FFCD02D32604B02

#include <QList>
#include <sscdaccess/abstractsampledataprovider.h>
#include <sscobjects/sample.h>

class DeviceInformationPage;
class TrainSamplePage;
class Sample;
class Microphone;
class SoundCard;

/**
 *	@class SSCSampleDataProvider
 *	@brief Combines all data gathered from one training session
 *
 *	@date 22.05.2010
 *	@author Peter Grasch
 */
class SSCSampleDataProvider : public AbstractSampleDataProvider
{

  private:
    DeviceInformationPage* m_infoPage;
    QList<TrainSamplePage*> m_trainSamplePages;

    QHash<QString, Microphone*> buildMicrophoneMappings(bool &ok);
    QHash<QString, SoundCard*> buildSoundCardMappings(bool &ok);
    QList<TrainingSamplesDescriptor*> buildSampleDescriptors(bool &ok);

  public:
    SSCSampleDataProvider(qint32 userId, Sample::SampleType sampleType, const QString& name);

    void registerMicrophoneInfo(DeviceInformationPage* infoPage);
    void registerDataProvider(TrainSamplePage* trainSamplePage);

    bool store();

    ~SSCSampleDataProvider();

};
#endif
