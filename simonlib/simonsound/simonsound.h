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

#ifndef SIMON_SIMONSOUND_H_D0C0BA2429B04F65935956A32C79BB09
#define SIMON_SIMONSOUND_H_D0C0BA2429B04F65935956A32C79BB09

#include <QString>


// #include <KI18n/klocalizedstring.h>
#include <QMetaType>

namespace SimonSound
{
  enum SoundDeviceUses
  {
    None=0,
    Training=1,
    Recognition=2
  };

  enum SoundDeviceOptions
  {
    NoOptions=0,
    Removable=1
  };

  enum SoundDeviceType
  {
    Input=1,
    Output=2
  };

  enum State
  {
    IdleState=0,
    PreparedState=1,
    ActiveState=2
  };

  enum Error
  {
      NoError=0,
      OpenError=1,
      IOError=2,
      UnderrunError=3,
      FatalError=4,
      BackendBusy=5
  };

  class DeviceConfiguration
  {
    private:
      QString m_name;
      int m_channels;
      int m_sampleRate;
      bool m_resample;
      int m_resampleRate;
      QString m_defaultSampleGroup;
      QString m_conditions;

    public:
      //QT5TODO: Add I18n to "default"
      DeviceConfiguration(const QString& name, int channels, int sampleRate,
          bool resample, int resampleRate, const QString& conditions=QString(), const QString& defaultSampleGroup="default") :
      m_name(name), m_channels(channels), m_sampleRate(sampleRate),
      m_resample(resample), m_resampleRate(resampleRate), m_defaultSampleGroup(defaultSampleGroup),
      m_conditions(conditions)
        {}

      DeviceConfiguration() :
      m_name(""), m_channels(0), m_sampleRate(0),
      m_resample(false), m_resampleRate(0), m_defaultSampleGroup("default")
        {}

      QString name() const { return m_name; }
      QString defaultSampleGroup() const { return m_defaultSampleGroup; }
      int channels() const { return m_channels; }
      int sampleRate() const { return m_sampleRate; }
      bool resample() const { return m_resample; }
      int resampleSampleRate() const { return m_resampleRate; }
      QString conditions() const { return m_conditions; }

      void setChannels(int channels) { m_channels = channels; }
      void setSampleRate(int sampleRate) { m_sampleRate = sampleRate; }
      void setDefaultSampleGroup(QString sampleGroup) { m_defaultSampleGroup = sampleGroup; }

      bool operator== (const DeviceConfiguration& b) const
      {
        return (m_name == b.name()) && (m_channels == b.channels()) && (m_sampleRate == b.sampleRate()) &&
                (m_resampleRate == b.resampleSampleRate()) && (m_resample == b.resample()) &&
                (m_defaultSampleGroup == b.defaultSampleGroup()) &&
                (m_conditions == b.conditions());
      }

      DeviceConfiguration operator=(const DeviceConfiguration& b) {
        m_name = b.name();
        m_channels = b.channels();
        m_sampleRate = b.sampleRate();
        m_resample = b.resample();
        m_resampleRate = b.resampleSampleRate();
        m_defaultSampleGroup = b.defaultSampleGroup();
        m_conditions = b.conditions();
        return *this;
      }
  };
}

Q_ENUMS(SimonSound::State);
Q_DECLARE_METATYPE(SimonSound::State);
Q_ENUMS(SimonSound::Error);
Q_DECLARE_METATYPE(SimonSound::Error);




// //forward declaration needed for QHash include
// (cascading down to first use of a Deviceconfiguration-based QHash)
// Q_DECLARE_METATYPE(SimonSound::DeviceConfiguration);
inline uint qHash(const SimonSound::DeviceConfiguration& dev);
inline uint qHash(const SimonSound::DeviceConfiguration& dev, uint seed);
#include <QHash>

inline uint qHash(const SimonSound::DeviceConfiguration& dev)
{
  return qHash(QString("%1||%2||%3||%4||%5").arg(dev.name()).arg(dev.channels()).arg(dev.sampleRate())
        .arg(dev.resample() ? "1" : "0").arg(dev.resampleSampleRate()));
}

//QT5TODO: What?
inline uint qHash(const SimonSound::DeviceConfiguration& dev, uint seed)
{
    return qHash(QString("%1||%2||%3||%4||%5").arg(dev.name()).arg(dev.channels()).arg(dev.sampleRate())
                 .arg(dev.resample() ? "1" : "0").arg(dev.resampleSampleRate()), seed);
}



#endif
