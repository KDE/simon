/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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
#include "simonttsadaptor.h"
#include "ttsconfiguration.h"
#include "joviettsprovider.h"
#include "recordedttsprovider.h"
#include "webservicettsprovider.h"
#include <QString>
#include <QDBusConnection>
#include <QRegExp>
#include <KDebug>

/**
 * \brief Constructor
 *
 * \todo Will set up the backends to the configured values
 */
SimonTTSPrivate::SimonTTSPrivate() : forceReinitialization(true)
{
  new SimonTTSAdaptor(this);
  QDBusConnection dbus = QDBusConnection::sessionBus();
  dbus.registerObject("/SimonTTS", this);
  dbus.registerService("org.simon-listens.SimonTTS");
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
bool SimonTTSPrivate::initialize()
{
  if (providers.isEmpty())
  {
    TTSConfiguration::self()->readConfig();
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
        {
          providers << new JovieTTSProvider();
        }
        else if (back == "Webservice")
        {
          providers << new WebserviceTTSProvider();
        } else
          kDebug() << "Unknown provider: " << back;
      }
    }

  }

  bool succ = true;
  foreach (SimonTTSProvider *p, providers)
    succ = p->initialize() && succ;

  if (succ)
    forceReinitialization = false;
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
 * \brief Calls the other say method with a default parameter
 *
 * Just a wrapper function for the dbus interface
 * \param text The text to say
 * \return True if successful
 */
bool SimonTTSPrivate::say(const QString& text)
{
  return say(text, SimonTTS::StripHTML);
}

/**
 * \brief Says the given text using the text to speech engine
 *
 * \param text The text to say
 * \param flags Te flags to apply
 * \return True if successful
 */
bool SimonTTSPrivate::say(const QString& text, SimonTTS::TTSFlags flags)
{
  if (forceReinitialization && !initialize()) return false;
  QString spokenText = processString(text, flags);
  if (spokenText.isEmpty()) return true;


  recentlyRequestedTexts.removeAll(spokenText);
  recentlyRequestedTexts.insert(0, spokenText);
  if (recentlyRequestedTexts.count() >= 30)
    recentlyRequestedTexts.removeAt(30);

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
  if (forceReinitialization && !initialize()) return false;
  bool succ = true;
  foreach (SimonTTSProvider *p, providers)
    succ = p->interrupt() && succ;
  return succ;
}


/**
 * \brief Returns the last texts (up to 30) that were requested from the tts system
 * \return List of texts
 */
QStringList SimonTTSPrivate::recentlyUsed()
{
  return recentlyRequestedTexts;
}

/**
 * \brief Uninitializes the TTS system. 
 * \note It's safe to call this anytime because the system will be re-initialized automatically if needed 
 *
 * Call \sa interrupt() if you want to stop the TTS immediately
 */
bool SimonTTSPrivate::uninitialize()
{
  bool succ = true;
  foreach (SimonTTSProvider *p, providers)
    succ = p->uninitialize() && succ;
  qDeleteAll(providers);
  kDebug() << "Destroying all providers";
  providers.clear();
  forceReinitialization = true;
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

