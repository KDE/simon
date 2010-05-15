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
	QObject(parent)
{
	QList<SimonSound::DeviceConfiguration> devices = SoundServer::getRecognitionDevices();

	qint8 i=0;
	foreach (SimonSound::DeviceConfiguration dev, devices)
	{
		SimondStreamerClient *streamer = new SimondStreamerClient(i++, s, dev, this);

		connect(streamer, SIGNAL(started()), this, SIGNAL(started()));
		connect(streamer, SIGNAL(stopped()), this, SIGNAL(stopped()));
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




