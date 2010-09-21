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

#include "simontts.h"
#include <QString>
#include <QRegExp>
#include <QDBusInterface>
#include <QDBusConnection>
#include <KDebug>

QDBusInterface* SimonTTS::interface = 0;


/**
 * \brief Will force the sytem to performe the initialization
 *
 * Normally the tts subsystem will be initialized during the first call
 * to \sa say() but this can be done earlier by calling this method
 *
 * Once initialized this will do nothing (and return true)
 *
 * \return Success
 */
bool SimonTTS::initialize()
{
  kDebug() << "Initializing tts";
  if (interface && interface->isValid()) return true;

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
 * \brief Says the given text using the text to speech engine
 *
 * Will call \sa initialize() if the system has not yet been initialized
 *
 * \param text The text to say
 * \return True if successful
 */
bool SimonTTS::say(const QString& text, SimonTTS::TTSFlags flags)
{
  if (!initialize()) return false;

  QString spokenText = text;
  kDebug() << "Text requested to be spoken: " << spokenText;

  if (flags & SimonTTS::StripHTML)
  {
    spokenText = spokenText.replace(QRegExp("<br */?>"), "\n");
    spokenText = spokenText.replace("</p>", "\n");
    spokenText = spokenText.remove(QRegExp("<[^>]*>"));
    spokenText = spokenText.trimmed();
  }

  kDebug() << "Saying: " << spokenText;
  interface->call("say", spokenText, 0);
  return true;
}


/**
 * \brief Uninitializes the TTS system. 
 * \note It's safe to call this anytime because the system will be re-initialized automatically if needed 
 */
bool SimonTTS::uninitialize()
{
  interface->deleteLater();
  interface = 0;
  return true;
}

