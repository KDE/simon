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

#include "wavplayerclient.h"
#include "wavplayersubclient.h"
#include "soundserver.h"
#include <simonwav/wav.h>
#include <KDebug>


/**
 * \brief Constructor
 * \author Peter Grasch
 */
WavPlayerClient::WavPlayerClient(QObject* parent) : QObject(parent), m_isPlaying(false)
{
}


void WavPlayerClient::slotCurrentProgress(int progress)
{
  WavPlayerSubClient *c = dynamic_cast<WavPlayerSubClient*>(sender());
  if (!c) return;

  if (clientsWaitingToFinish.isEmpty()) return; // this is possible because the slot is connected indirectly

  if (c == clientsWaitingToFinish.at(0))
    emit currentProgress(progress);               //always emit current progress info from first
  // device in leftover queue
}


void WavPlayerClient::slotFinished()
{
  kDebug() << "Received finish!";
  WavPlayerSubClient *c = dynamic_cast<WavPlayerSubClient*>(sender());
  if (!c) {
    kDebug() << "Sender not a wavplayersubclient: " << sender();
    return;
  }

  clientsWaitingToFinish.removeAll(c);

  if (clientsWaitingToFinish.isEmpty()) {
    kDebug() << "Emitting finish";
    emit finished();
    m_isPlaying = false;
  } else kDebug() << "More clients waiting to finish";
}


/**
 * \brief Plays back the given file
 * \author Peter Grasch
 */
bool WavPlayerClient::play( QString filename )
{
  QSharedPointer<WAV> w(new WAV(filename));
  int channels = w->getChannels();
  int sampleRate = w->getSampleRate();

  kDebug() << "File has " << channels << " channels and a samplerate of " << sampleRate;

  qint64 length = w->size();
  if (length==0) {
    w->deleteLater();
    return false;
  }

  return play(w, channels, sampleRate);
}

bool WavPlayerClient::play(QSharedPointer<QIODevice> device, int channels, int samplerate)
{
  qDeleteAll(clients);
  clients.clear();

  //get devices that support this playback configuration
  QList<SimonSound::DeviceConfiguration> devices = SoundServer::getTrainingOutputDevices();
  foreach (const SimonSound::DeviceConfiguration& dev, devices) {
    if ((dev.channels() != channels) || (!dev.resample() && (dev.sampleRate() != samplerate)))
      continue;

    WavPlayerSubClient *c = new WavPlayerSubClient(dev, parent());
    connect(c, SIGNAL(currentProgress(int)), this, SLOT(slotCurrentProgress(int)));
    connect(c, SIGNAL(finished()), this, SLOT(slotFinished()));
    c->initToSampleRate(samplerate);
    clients << c;
  }
  kDebug() << "Found applicable devices: " << clients.count();

  bool succ = false;
  kDebug() << "Playing: stream" << channels << samplerate;

  foreach (WavPlayerSubClient *client, clients) {
    if (client->play(device)) {
      clientsWaitingToFinish << client;
      succ = true;
    }
  }
  if (succ) m_isPlaying = true;
  return succ;
}



/**
 * \brief Stops the current playback
 *
 * \author Peter Grasch
 */
void WavPlayerClient::stop()
{
  foreach (WavPlayerSubClient *client, clients)
    client->stop();

  m_isPlaying = false;
}


WavPlayerClient::~WavPlayerClient()
{
  qDeleteAll(clients);
}
