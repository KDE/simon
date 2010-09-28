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

#include "webservicettsprovider.h"
#include "ttsconfiguration.h"
#include <simonsound/wavplayerclient.h>
#include <simonsound/soundserver.h>
#include <QStringList>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <KDebug>
#include <KStandardDirs>

WebserviceTTSProvider::WebserviceTTSProvider() : QObject(),
  net(0),
  player(0)
{
  initializeOutput();
  connect(SoundServer::getInstance(), SIGNAL(devicesChanged()), this, SLOT(initializeOutput()));
}


/**
 * \brief Will force the sytem to performe the initialization
 *
 * \return Success
 */
bool WebserviceTTSProvider::initialize()
{
  if (!net)
  {
    net = new QNetworkAccessManager(this);
    connect(net, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyReceived(QNetworkReply*)));
    kDebug() << "Initialized";
  }

  return true;
}

void WebserviceTTSProvider::replyReceived(QNetworkReply* reply)
{
  if (reply->error() != QNetworkReply::NoError)
  {
    kWarning() << "Webservice reported error: " << reply->error();
    return;
  }

  QString path = KStandardDirs::locateLocal("tmp", "simontts/webservice/return.wav");
  QFile f(path);
  if (!f.open(QIODevice::WriteOnly|QIODevice::Truncate))
    return;
  f.write(reply->readAll());
  f.close();

  //play wav file
  kDebug() << "Playing: " << path;
  if (!player->isPlaying())
    player->play(path);
  else {
    kDebug() << "Adding to playback queue: " << path;
    filesToPlay << path;
  }
}

void WebserviceTTSProvider::initializeOutput()
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
 * \return Current implementation returns true if we are initialized
 */
bool WebserviceTTSProvider::canSay(const QString& text)
{
  Q_UNUSED(text);
  return initialize();
}

/**
 * \brief Says the given text using the text to speech engine
 *
 * Will call \sa initialize() if the system has not yet been initialized
 *
 * \param text The text to say
 * \return True if successful
 */
bool WebserviceTTSProvider::say(const QString& text)
{
  if (!initialize())
    return false;

  kDebug() << "Getting: " << TTSConfiguration::webserviceURL().replace("%1", text);
  net->get(QNetworkRequest(KUrl(TTSConfiguration::webserviceURL().replace("%1", text))));
  return true;
}


/**
 * \brief Plays the next file in the playing queue
 */
void WebserviceTTSProvider::playNext()
{
  if (filesToPlay.isEmpty()) return;
  player->play(filesToPlay.takeAt(0));
}

/**
 * \brief Interrupts the current spoken text
 * \return true if successfully interrupted text or no text was playing
 */
bool WebserviceTTSProvider::interrupt()
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
bool WebserviceTTSProvider::uninitialize()
{
  return true;
}

WebserviceTTSProvider::~WebserviceTTSProvider()
{
  delete player;
}

