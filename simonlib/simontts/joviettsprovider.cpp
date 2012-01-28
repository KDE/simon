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

#include "joviettsprovider.h"
#include <QStringList>
#include <QRegExp>
#include <QDBusInterface>
#include <QDBusConnection>
#include <KDebug>

JovieTTSProvider::JovieTTSProvider() : interface(0)
{
}


/**
 * \brief Will force the system to performe the initialization
 *
 * Normally the tts subsystem will be initialized during the first call
 * to \sa say() but this can be done earlier by calling this method
 *
 * Once initialized this will do nothing (and return true)
 *
 * \return Success
 */
bool JovieTTSProvider::initialize()
{
  if (interface && interface->isValid()) return true;
  kDebug() << "Initializing tts";

  interface = new QDBusInterface("org.kde.jovie",
        "/KSpeech",
        "org.kde.KSpeech",
        QDBusConnection::sessionBus()); 

  if (!interface->isValid())
  {
    kWarning() << "DBus interface for speech to text not valid: " << interface->lastError();
    uninitialize();
    return false;
  }

  return true;
}


/**
 * \brief Returns true if the given text can be synthesized
 *
 * As the Jovie TTS system takes any text this method returns true whenever
 * the connection to Jovie can be initialized i.e. when \sa initialize() 
 * worked.
 *
 * \return True if the text can be said
 */
bool JovieTTSProvider::canSay(const QString& text)
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
bool JovieTTSProvider::say(const QString& text)
{
  if (/*!interrupt() ||*/ !initialize()) return false;

  interface->call("say", text, 0);

  /*
   * Configure speech dispatcher to allow "some" punctuation marks for not needing this
   *
  //TODO: some voices have problems with sentence characters in jovie; strip them out for now
  QStringList sentences = text.split(QRegExp("(\\?|\\!|\\.|:)"), QString::SkipEmptyParts);

  while (!sentences.isEmpty())
  {
    kDebug() << "Saying: " << sentences[0].trimmed();
    interface->call("say", sentences.takeAt(0).trimmed(), 0);
  }
  */

  return true;
}


/**
 * \brief Interrupts the current spoken text
 * \return true if successfully sent interrupt request or if service seems unavailable
 */
bool JovieTTSProvider::interrupt()
{
  if (!initialize()) return true;
  interface->call("stop");
  interface->call("removeAllJobs");
  return true;
}


/**
 * \brief Uninitializes the TTS system. 
 * \note It's safe to call this anytime because the system will be re-initialized automatically if needed 
 *
 * Call \sa interrupt() if you want to stop the TTS immediately; Otherwise the current text will still be
 * finished
 * \return Success
 */
bool JovieTTSProvider::uninitialize()
{
  if (!interface) return true;
  interface->deleteLater();
  interface = 0;
  return true;
}


JovieTTSProvider::~JovieTTSProvider()
{
  if (interface) interface->deleteLater();
}
