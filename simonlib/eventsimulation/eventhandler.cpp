/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
 *   Copyright (C) 2010 Manfred Scheucher <deadalps@gmail.com>
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

#include "eventhandler.h"
#include "coreevents.h"

#include <QChar>

#ifdef Q_OS_LINUX
#include "xevents.h"
#endif
#ifdef Q_OS_MAC
#include "carbonevents.h"
#endif
#ifdef Q_OS_WIN32
#include "windowsevents.h"
#endif

#include <QString>
#include <KDebug>

EventHandler* EventHandler::instance;

/**
 * @brief Constructor
 *
 * Creates a new Event Handler
 *
 * @author Peter Grasch
 */
EventHandler::EventHandler()
#ifdef Q_OS_LINUX
: coreEvents(new XEvents())
#endif
#ifdef Q_OS_MAC
: coreEvents(new CarbonEvents())
#endif
#ifdef Q_OS_WIN32
: coreEvents( (CoreEvents*) new WindowsEvents())
#endif
{
}

EventHandler* EventHandler::getInstance()
{
  if (!instance) instance = new EventHandler();
  return instance;
}

/**
 * \brief Tells coreEvents to click the coordinates
 * \author Peter Grasch
 * @param x The x coordinate
 * @param y The y coordinate
 * @param clickMode The mode of the click (LMB, RMB, etc.)
 */
void EventHandler::click(int x, int y, EventSimulation::ClickMode clickMode)
{
  if (!coreEvents) return;

  coreEvents->click(x,y, clickMode);
}


void EventHandler::dragAndDrop(int xStart, int yStart, int x, int y)
{
  if (!coreEvents) return;

  coreEvents->dragAndDrop(xStart, yStart, x, y);
}


/**
 * @brief Sends a word to the underlying Core Eventhandlers
 *
 * Splits up the word in characters and sends every single one
 * separate by using sendKey()
 *
 * @param QString word
 * The word to send
 *
 * @author Peter Grasch
 */
void EventHandler::sendWord(const QString& word) const
{
  for (int i=0; i < word.size();i++) {
    sendKey(word.at(i));
  }
}


/**
 * \brief Sends the shortcut to the Backends
 * \author Peter Grasch
 * @param shortcut The shortcut to send
 */
void EventHandler::sendShortcut(const QKeySequence& shortcut, EventSimulation::PressMode mode) const
{
  coreEvents->sendShortcut(shortcut, mode);
}


/**
 * @brief Sends a key to the XServer / WinAPI
 *
 * Sends every key trough the underlying CoreEvent classes.
 * Distincts between Linux and Windows
 *
 * @param QChar key
 * The key to send
 *
 * @author Peter Grasch
 * \todo Shift gets unset a LOT
 */
void EventHandler::sendKey(const QChar& key) const
{
  unsigned int c;
  c = key.unicode();
  coreEvents->sendKey(c, (EventSimulation::PressMode)
      (EventSimulation::Press|EventSimulation::Release));
}


void EventHandler::setModifier(int virtualKey, bool once) const
{
  coreEvents->setModifierKey(virtualKey, once);
}


void EventHandler::unsetModifier(int virtualKey) const
{
  coreEvents->unsetModifier(virtualKey);
}


EventHandler::~EventHandler()
{
  delete coreEvents;
}
