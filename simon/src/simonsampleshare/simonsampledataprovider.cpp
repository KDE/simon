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


#include "simonsampledataprovider.h"
#include <KDebug>

SimonSampleDataProvider::SimonSampleDataProvider(qint32 userId, Sample::SampleType sampleType, const QString& name) :
  AbstractSampleDataProvider(userId, sampleType, name)
{
}

bool SimonSampleDataProvider::store()
{
  kWarning() << "Not implemented";
  return false;
}

//TODO
QHash< QString, SoundCard* > SimonSampleDataProvider::buildSoundCardMappings(bool& ok)
{
  ok = true;
  return QHash< QString, SoundCard* >();
}

QHash< QString, Microphone* > SimonSampleDataProvider::buildMicrophoneMappings(bool& ok)
{
  ok = true;
  return QHash< QString, Microphone* >();
}

QList< TrainingSamplesDescriptor* > SimonSampleDataProvider::buildSampleDescriptors(bool& ok)
{
  ok = true;
  return QList< TrainingSamplesDescriptor* >();
}
