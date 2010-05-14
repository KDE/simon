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

#ifndef SIMON_SIMONSOUND_H_D0C0BA2429B04F65935956A32C79BB09
#define SIMON_SIMONSOUND_H_D0C0BA2429B04F65935956A32C79BB09

#include <QString>


namespace SimonSound
{
	enum SoundDeviceUses {
		None=0,
		Training=1,
		Recognition=2
	};
	
	enum SoundDeviceOptions {
		NoOptions=0,
		Removable=1
	};

	enum SoundDeviceType {
		Input=1,
		Output=2
	};


	class DeviceConfiguration
	{
		private:
			QString m_name;
			int m_channels;
			int m_sampleRate;
		public:
			DeviceConfiguration(const QString& name, int channels, int sampleRate) : 
				m_name(name), m_channels(channels), m_sampleRate(sampleRate)
			{}
			
			QString name() const { return m_name; }
			int channels() const { return m_channels; }
			int sampleRate() const { return m_sampleRate; }

			void setChannels(int channels) { m_channels = channels; }
			void setSampleRate(int sampleRate) { m_sampleRate = sampleRate; }

			bool operator== (const DeviceConfiguration& b) const
			{
				return (m_name == b.name()) && (m_channels == b.channels()) && (m_sampleRate == b.sampleRate());
			}

			DeviceConfiguration operator=(const DeviceConfiguration& b)
			{
				m_name = b.name();
				m_channels = b.channels();
				m_sampleRate = b.sampleRate();
				return *this;
			}
	};
}

inline uint qHash(const SimonSound::DeviceConfiguration& dev);
#include <QHash>
inline uint qHash(const SimonSound::DeviceConfiguration& dev)
{
	return qHash(QString("%1||%2||%3").arg(dev.name(), dev.channels(), dev.sampleRate()));
}


#endif


