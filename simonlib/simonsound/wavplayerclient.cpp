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
WavPlayerClient::WavPlayerClient(QObject* parent) : QObject(parent)
{
  QList<SimonSound::DeviceConfiguration> devices = SoundServer::getTrainingOutputDevices();

  kDebug() << "Found applicable devices: " << devices.count();
  foreach (const SimonSound::DeviceConfiguration& dev, devices) {
    WavPlayerSubClient *c = new WavPlayerSubClient(dev, parent);
    connect(c, SIGNAL(currentProgress(int)), this, SLOT(slotCurrentProgress(int)));
    connect(c, SIGNAL(finished()), this, SLOT(slotFinished()));
    clients << c;
  }
}


void WavPlayerClient::slotCurrentProgress(int progress)
{
  WavPlayerSubClient *c = dynamic_cast<WavPlayerSubClient*>(sender());
  if (!c) return;

  Q_ASSERT(!clientsWaitingToFinish.isEmpty());

  if (c == clientsWaitingToFinish.at(0))
    emit currentProgress(progress);               //always emit current progress info from first
  // device in leftover queue
}


void WavPlayerClient::slotFinished()
{
  WavPlayerSubClient *c = dynamic_cast<WavPlayerSubClient*>(sender());
  if (!c) return;

  clientsWaitingToFinish.removeAll(c);

  if (clientsWaitingToFinish.isEmpty()) {
    emit finished();
  }
}


/**
 * \brief Plays back the given file
 * \author Peter Grasch
 */
bool WavPlayerClient::play( QString filename, int channels )
{
  bool succ = false;
  kDebug() << "Playing: " << filename;
  foreach (WavPlayerSubClient *client, clients) {
    kDebug() << "Go Client!";
    if ((client->getChannelCount() == channels) &&
    client->play(filename)) {
      clientsWaitingToFinish << client;
      succ = true;
    }
  }
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

}


WavPlayerClient::~WavPlayerClient()
{
  qDeleteAll(clients);
}
