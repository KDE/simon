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
#include <QBuffer>
#include <simonwav/wav.h>

WebserviceTTSProvider::WebserviceTTSProvider() : QObject(),
  currentConnection(0),
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
    net = new QNetworkAccessManager(this);

  return true;
}

void WebserviceTTSProvider::replyReceived()
{
  QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
  if (!reply) return;
  
  if (reply->error() != QNetworkReply::NoError)
  {
    kWarning() << "Webservice reported error: " << reply->errorString();
    return;
  }
  
/*
  QString path = KStandardDirs::locateLocal("tmp", "simontts/webservice/return.wav");
  QFile f(path);
  if (!f.open(QIODevice::WriteOnly|QIODevice::Truncate))
    return;
  f.write(filesToPlay.first()->buffer());
  f.close();
  //play wav file
  kDebug() << "Playing: " << path;
  if (!player->isPlaying())
    player->play(path);
  else {
    kDebug() << "Adding to playback queue: " << path;
    filesToPlay.enqueue(path);
  }*/
  
  if (!filesToDownload.isEmpty())
    currentConnection = net->get(QNetworkRequest(KUrl(filesToDownload.dequeue())));
  else
    currentConnection = 0;
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

  QString url = TTSConfiguration::webserviceURL().replace("%1", text);
  kDebug() << "Getting: " << url;
 
  if (currentConnection)
    filesToDownload.enqueue(url);
  else {
    currentConnection = net->get(QNetworkRequest(KUrl(url)));
    connect(currentConnection, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(currentConnection, SIGNAL(finished()), this, SLOT(replyReceived()));
  }
  QBuffer *b = new QBuffer(this);
  b->open(QIODevice::ReadWrite);
  filesToPlay.enqueue(b);
  return true;
}

void WebserviceTTSProvider::downloadProgress(qint64 now, qint64 max)
{
  kDebug() << "Download progress: " << now << max;
  filesToPlay.first()->buffer() += currentConnection->readAll();
  enquePlayback();
}

void WebserviceTTSProvider::enquePlayback()
{
  kDebug() << "Bytes available: " << filesToPlay.first()->bytesAvailable() << player->isPlaying();
  if (filesToPlay.first()->bytesAvailable() >= 28 && !player->isPlaying()) {
    kDebug() << "Received header so starting playback";
    qint16 channels;
    qint32 samplerate;
    WAV::parseHeader(filesToPlay.first(), channels, samplerate);
    kDebug() << "header: " << channels << samplerate;
    QBuffer *handle = filesToPlay.first();
    handle->buffer().remove(0, 44);
    player->play(handle, channels, samplerate);
  }
}


/**
 * \brief Plays the next file in the playing queue
 */
void WebserviceTTSProvider::playNext()
{
  if (filesToPlay.isEmpty()) return;
  
  kDebug() << "Finished playback";
  filesToPlay.dequeue();
  
  if (filesToPlay.isEmpty()) return;
  enquePlayback();
}

/**
 * \brief Interrupts the current spoken text
 * \return true if successfully interrupted text or no text was playing
 */
bool WebserviceTTSProvider::interrupt()
{
  if (!initialize()) return true;

  if (currentConnection) {
    currentConnection->abort();
    currentConnection->deleteLater();
    currentConnection = 0;
  }
  player->stop();
  qDeleteAll(filesToPlay);
  filesToPlay.clear();
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

