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

#include "recordedttsprovider.h"
#include "recordingsetcollection.h"
#include <simonsound/wavplayerclient.h>
#include <simonsound/soundserver.h>
#include <QStringList>
#include <QRegExp>
#include <QDBusInterface>
#include <QDBusConnection>
#include <KDebug>
#include <KStandardDirs>

RecordedTTSProvider::RecordedTTSProvider() : QObject(), sets(0),
  player(0)
{
  initializeOutput();
  connect(SoundServer::getInstance(), SIGNAL(devicesChanged()), this, SLOT(initializeDevices()));
}


/**
 * \brief Will force the sytem to performe the initialization
 *
 * For file based systems that means reading the set definition from the 
 * XML file and loading the individual recording sets
 *
 * \return Success
 */
bool RecordedTTSProvider::initialize()
{
  if (sets) return true;
  delete sets;
  sets = new RecordingSetCollection;
  if (!sets->init(KStandardDirs::locateLocal("appdata", "ttsrec/ttssets.xml")))
  {
    kDebug() << "Failed to init...";
    delete sets;
    sets = 0;
    return false;
  }
  kDebug() << "Initialized";

  return true;
}

void RecordedTTSProvider::initializeOutput()
{
  if (player)
  {
    player->stop();
    player->deleteLater();
  }
  player = new WavPlayerClient();
  connect(player, SIGNAL(finished()), this, SLOT(playNext()));
}



/**
 * \brief Returns true if the given text can be synthesized
 *
 * Checks if we have a recorded sound file for the given text;
 * Returns false if not.
 *
 * \return True if the text can be said
 */
bool RecordedTTSProvider::canSay(const QString& text)
{
  Q_UNUSED(text);
  if (!sets && !initialize()) return false;

  kDebug() << "Looking if we have a recording for " << text;
  return sets->canSay(text);
}

/**
 * \brief Says the given text using the text to speech engine
 *
 * Will call \sa initialize() if the system has not yet been initialized
 *
 * \param text The text to say
 * \return True if successful
 */
bool RecordedTTSProvider::say(const QString& text)
{
  if (/*!interrupt() ||*/ (!sets && !initialize()))
    return false;

  //play wav file
  QString path = sets->getPath(text);
  kDebug() << "Playing: " << path;
  if (!player->isPlaying())
    player->play(path);
  else {
    kDebug() << "Adding to playback queue: " << path;
    filesToPlay << path;
  }

  return true;
}


/**
 * \brief Plays the next file in the playing queue
 */
void RecordedTTSProvider::playNext()
{
  if (filesToPlay.isEmpty()) return;
  player->play(filesToPlay.takeAt(0));
}

/**
 * \brief Interrupts the current spoken text
 * \return true if successfully interrupted text or no text was playing
 */
bool RecordedTTSProvider::interrupt()
{
  if (!initialize()) return true;

  filesToPlay.clear();
  player->stop();
  return true;
}

/**
 * \brief Uninitializes the playback system. 
 * \note It's safe to call this anytime because the system will be re-initialized automatically if needed 
 *
 * Call \sa interrupt() if you want to stop the TTS immediatly; Otherwise the current text will still be
 * finished
 * \return Success
 */
bool RecordedTTSProvider::uninitialize()
{
  delete sets;
  sets = 0;
  return true;
}


RecordedTTSProvider::~RecordedTTSProvider()
{
  delete sets;
  delete player;
}

