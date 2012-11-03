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

#include "simondstreamer.h"
#include "simondstreamerclient.h"
#include <simonsound/simonsound.h>
#include <simonsound/soundserver.h>

#include <QObject>
#include <KDebug>

/**
 * \brief Constructor
 */
SimondStreamer::SimondStreamer(SimonSender *s, QObject *parent) :
QObject(parent),
m_sender(s)
{
  initializeDevices();
  connect(SoundServer::getInstance(), SIGNAL(devicesChanged()), this, SLOT(initializeDevices()));
}


void SimondStreamer::initializeDevices()
{
  bool wasRunning = isRunning();

  foreach (SimondStreamerClient *c, clients)
    c->stop();
  qDeleteAll(clients);
  clients.clear();

  QList<SimonSound::DeviceConfiguration> devices = SoundServer::getRecognitionInputDevices();
  kDebug() << "Initializing " << devices.count() << " streaming devices...";
  qint8 i=0;
  foreach (const SimonSound::DeviceConfiguration& dev, devices) {
    SimondStreamerClient *streamer = new SimondStreamerClient(i++, m_sender, dev, this);

    connect(streamer, SIGNAL(started()), this, SIGNAL(started()));
    connect(streamer, SIGNAL(stopped()), this, SIGNAL(stopped()));

    if (wasRunning)
      streamer->start();

    clients << streamer;
  }
}


bool SimondStreamer::isRunning()
{
  foreach (SimondStreamerClient *c, clients)
    if (c->isRunning())
      return true;
  return false;
}


bool SimondStreamer::start()
{
  kDebug() << "Staring Simondstreamer" << SoundServer::getInstance();
  if (clients.isEmpty())
  {
    kDebug() << "clients are empty";
    return false;
  }
  bool succ = true;
  foreach (SimondStreamerClient *c, clients)
    succ = c->start() && succ;
  return succ;
}


/**
 * \brief This will stop the current recording
 *
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
bool SimondStreamer::stop()
{
  bool succ = true;
  foreach (SimondStreamerClient *c, clients)
    succ = c->stop() && succ;
  return succ;
}


/**
 * \brief Destructor
 */
SimondStreamer::~SimondStreamer()
{
}
