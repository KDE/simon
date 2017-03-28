/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "coreevents.h"
#include "pressmode.h"
#include <QKeySequence>

CoreEvents::CoreEvents()
: shiftSet(false),
altgrSet(false),
altSet(false),
superSet(false),
strgSet(false),
optionSet(false),
shiftOnce(false),
altgrOnce(false),
altOnce(false),
superOnce(false),
strgOnce(false),
optionOnce(false)
{
}


void CoreEvents::sendKey(unsigned int key /*unicode representation*/, EventSimulation::PressMode mode)
{
  //handle dead keys
  DeadKey* d = deadKeys.value(key);

  if (d) {
    sendKeyPrivate(d->deadKey(), mode);
    key = d->baseKey();
  }
  sendKeyPrivate(key, mode);
}


void CoreEvents::unsetUnneededModifiers()
{
  if (shiftOnce) {
    shiftOnce=false;
    unsetModifier(Qt::SHIFT);
  }
  if (altgrOnce) {
    altgrOnce=false;
    unsetModifier(Qt::Key_AltGr);
  }
  if (strgOnce) {
    strgOnce=false;
    unsetModifier(Qt::CTRL);
  }
  if (altOnce) {
    altOnce=false;
    unsetModifier(Qt::ALT);
  }
  if (superOnce) {
    superOnce=false;
    unsetModifier(Qt::META);
  }
}


/**
 * \brief Sends the given Shortcut
 *
 * This will set all the given modifiers and keys and then revert to the original
 * position (assuming the once flag is correctly interpreted)
 *
 * \author Peter Grasch
 * @param shortcut The shortcut to send
 */
void CoreEvents::sendShortcut(const QKeySequence& shortcut, EventSimulation::PressMode mode)
{
  int key = 0;
  int modifiers = 0;
  for(unsigned int i=0; i < shortcut.count(); i++) {
    key |= shortcut[i] & ~(Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT);
    modifiers |= shortcut[i] & (Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT);
  }

  //make key lowercase
  if ((key >= 65) && (key <= 90))
    key += 32;

  Qt::KeyboardModifiers mods = Qt::KeyboardModifiers(modifiers);
  if (key == Qt::Key_Backtab) {
    key = Qt::Key_Tab;
    mods |= Qt::ShiftModifier;
  }
  if (mode & EventSimulation::Press)
  {
    setModifierKey(mods, (mode & EventSimulation::Release));
    sendKey(key, mode);
  }
  if (mode & EventSimulation::Release)
    unsetUnneededModifiers();
}

void CoreEvents::setModifierKey(int virtualKey, bool once)
{
  if ((!shiftSet) && (virtualKey & Qt::SHIFT)) {
    setModifierKeyPrivate(Qt::SHIFT);
    shiftSet=true;
    shiftOnce=once;
  }

  if ((!altgrSet) && (virtualKey & Qt::Key_AltGr)) {
    setModifierKeyPrivate(Qt::Key_AltGr);
    altgrSet=true;
    altgrOnce=once;
  }

  if ((!strgSet) && (virtualKey & Qt::CTRL)) {
    setModifierKeyPrivate(Qt::CTRL);
    strgSet=true;
    strgOnce=once;
  }

  if ((!altSet) && (virtualKey & Qt::ALT)) {
    setModifierKeyPrivate(Qt::ALT);
    altSet=true;
    altOnce=once;
  }

  if ((!superSet) && (virtualKey & Qt::META)) {
    setModifierKeyPrivate(Qt::META);
    superSet=true;
    superOnce=once;
  }
}

void CoreEvents::unsetModifier(int virtualKey)
{
  if (virtualKey & Qt::SHIFT) {
    unsetModifierKeyPrivate(Qt::SHIFT);
    shiftSet=false;
  }

  if (virtualKey & Qt::Key_AltGr) {
    unsetModifierKeyPrivate(Qt::Key_AltGr);
    altgrSet=false;
  }

  if (virtualKey & Qt::CTRL) {
    unsetModifierKeyPrivate(Qt::CTRL);
    strgSet=false;
  }

  if (virtualKey & Qt::ALT) {
    unsetModifierKeyPrivate(Qt::ALT);
    altSet=false;
  }

  if (virtualKey & Qt::META) {
    unsetModifierKeyPrivate(Qt::META);
    superSet=false;
  }
}

CoreEvents::~CoreEvents()
{
  qDeleteAll(deadKeys.values());
}
