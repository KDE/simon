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

#include "simonttsprivate.h"
#include "ttsconfiguration.h"
#include "joviettsprovider.h"
#include "recordedttsprovider.h"
#include <QString>
#include <QRegExp>
#include <KDebug>

/**
 * \brief Constructor
 *
 * \todo Will set up the backends to the configured values
 */
SimonTTSPrivate::SimonTTSPrivate()
{
}

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
bool SimonTTSPrivate::initialize()
{
  if (providers.isEmpty())
  {
    QStringList backends = TTSConfiguration::backends();
    kDebug() << "Backends: " << backends;
    foreach (const QString& back, backends)
    {
      if (back == "File")
      {
        providers << new RecordedTTSProvider();
        kDebug() << "Initializing file...";
      }
      else
      {
        kDebug() << "Initializing jovie...";
        if (back == "Jovie")
          providers << new JovieTTSProvider();
        else
          kDebug() << "Unknown provider: " << back;
      }
    }

  }


  bool succ = true;
  foreach (SimonTTSProvider *p, providers)
    succ = p->initialize() && succ;
  return succ;
}


/**
 * \brief Will apply the given flags and return the new text
 * \param text The text to process
 * \param flags The flags to apply
 * \return The new text
 */
QString SimonTTSPrivate::processString(QString text, SimonTTS::TTSFlags flags)
{
  if (flags & SimonTTS::StripHTML)
  {
    text = text.replace(QRegExp("<br */?>"), "\n");
    text = text.replace("</p>", "\n");
    text = text.remove(QRegExp("<[^>]*>"));
    text = text.trimmed();
  }
  return text;
}

/**
 * \brief Says the given text using the text to speech engine
 *
 * \param text The text to say
 * \return True if successful
 */
bool SimonTTSPrivate::say(const QString& text, SimonTTS::TTSFlags flags)
{
  QString spokenText = processString(text, flags);
  if (spokenText.isEmpty()) return true;

  foreach (SimonTTSProvider *p, providers)
    if (p->canSay(spokenText))
      return p->say(spokenText);
        
  return false;
}


/**
 * \brief Interrupts the current spoken text
 * \return true if successfully sent interrupt request or if service seems unavailable
 */
bool SimonTTSPrivate::interrupt()
{
  bool succ = true;
  foreach (SimonTTSProvider *p, providers)
    succ = p->interrupt() && succ;
  return succ;
}


/**
 * \brief Uninitializes the TTS system. 
 * \note It's safe to call this anytime because the system will be re-initialized automatically if needed 
 *
 * Call \sa interrupt() if you want to stop the TTS immediatly
 */
bool SimonTTSPrivate::uninitialize()
{
  bool succ = true;
  foreach (SimonTTSProvider *p, providers)
    succ = p->uninitialize() && succ;
  qDeleteAll(providers);
  providers.clear();
  return succ;
}

/**
 * \brief Destructor
 * Will destroy associated backends
 */
SimonTTSPrivate::~SimonTTSPrivate()
{
  qDeleteAll(providers);
}

