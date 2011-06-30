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

#include "wavplayersubclient.h"
#include "soundserver.h"
#include <KDebug>

/**
 * \brief Constructor
 * \author Peter Grasch
 */
WavPlayerSubClient::WavPlayerSubClient(SimonSound::DeviceConfiguration device, QObject* parent) : QIODevice(parent),
SoundOutputClient(device),
wav(0)
{
}


qint64 WavPlayerSubClient::readData(char *data, qint64 maxlen)
{
  qint64 read = wav->read(data, maxlen);
  emit currentProgress(SoundServer::getInstance()->byteSizeToLength(wav->pos(), m_deviceConfiguration));

  return read;

}


qint64 WavPlayerSubClient::writeData(const char *data, qint64 len)
{
  Q_UNUSED(data);
  Q_UNUSED(len);
  return -1;
}


int WavPlayerSubClient::getChannelCount()
{
  return m_deviceConfiguration.channels();
}


bool WavPlayerSubClient::open (OpenMode mode)
{
  if (wav) {
    if (wav->isOpen())
	  wav->close();
    if (!wav->open(QIODevice::ReadOnly))
      return false;
  }
  return QIODevice::open(mode);
}


void WavPlayerSubClient::close()
{
  if (wav && wav->isOpen())
      wav->close();
  QIODevice::close();
}

/**
 * \brief Plays directly from the given device to allow for streaming
 */
bool WavPlayerSubClient::play(QIODevice* device)
{
  if (wav) wav->deleteLater();
  wav = device;

  open(QIODevice::ReadOnly);

  if (!SoundServer::getInstance()->registerOutputClient(this)) {
    return false;
  }
  return true;
}

/**
 * \brief Stops the current playback
 *
 * \author Peter Grasch
 */
void WavPlayerSubClient::stop()
{
  SoundServer::getInstance()->deRegisterOutputClient(this);
}


void WavPlayerSubClient::finish()
{
  close();
  if (wav)
    wav->deleteLater();
  wav = 0;
  emit finished();
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
WavPlayerSubClient::~WavPlayerSubClient()
{
  if (wav) wav->deleteLater();
}
