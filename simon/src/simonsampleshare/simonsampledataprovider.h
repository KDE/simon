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


#ifndef SIMONSAMPLEDATAPROVIDER_H
#define SIMONSAMPLEDATAPROVIDER_H

#include <sscdaccess/abstractsampledataprovider.h>
#include <QStringList>

class Microphone;
class SoundCard;

class SimonSampleDataProvider : public AbstractSampleDataProvider
{
private:
  Microphone *m_microphone;
  SoundCard *m_soundCard;
  QStringList m_sampleBlackList;
  
protected:
    QHash<QString, Microphone*> buildMicrophoneMappings(bool &ok);
    QHash<QString, SoundCard*> buildSoundCardMappings(bool &ok);
    QList<TrainingSamplesDescriptor*> buildSampleDescriptors(bool &ok);
public:
    SimonSampleDataProvider(qint32 userId, Microphone *microphone, 
			    SoundCard *soundCard, 
			    Sample::SampleType sampleType, const QString& name,
          const QStringList& sampleBlackList);
    virtual ~SimonSampleDataProvider();
    bool store();
};

#endif // SIMONSAMPLEDATAPROVIDER_H
