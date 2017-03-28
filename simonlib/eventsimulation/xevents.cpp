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

#include "xevents.h"
#include "xeventsprivate.h"

/**
 * @brief Constructor
 *
 * Opens the display (member)
 *
 * @param char* displayName
 * The name of the display to open
 *
 * @author Peter Grasch
 */
XEvents::XEvents (const char* displayName) : PCEvents(),
d (new XEventsPrivate (displayName))
{
}


void XEvents::click (int x, int y, EventSimulation::ClickMode clickMode)
{
  d->click (x, y, clickMode);
}


void XEvents::dragAndDrop (int xStart, int yStart, int x, int y)
{
  d->dragAndDrop (xStart, yStart, x, y);
}


void XEvents::sendKeyPrivate (unsigned int key, EventSimulation::PressMode mode)
{
  d->sendKeyPrivate (key, mode);
  unsetUnneededModifiers();
}

void XEvents::setModifierKeyPrivate (int virtualKey)
{
  switch (virtualKey) {
  case Qt::SHIFT:
    d->setModifierKey(Qt::SHIFT);
    break;
  case Qt::Key_AltGr:
    d->setModifierKey(Qt::Key_AltGr);
    break;
  case Qt::CTRL:
    d->setModifierKey(Qt::CTRL);
    break;
  case Qt::ALT:
    d->setModifierKey(Qt::ALT);
    break;
  case Qt::META:
    d->setModifierKey(Qt::META);
    break;
  }
}


void XEvents::unsetModifierKeyPrivate (int virtualKey)
{
  switch (virtualKey) {
  case Qt::SHIFT:
    d->unsetModifier(Qt::SHIFT);
    break;
  case Qt::Key_AltGr:
    d->unsetModifier(Qt::Key_AltGr);
    break;
  case Qt::CTRL:
    d->unsetModifier(Qt::CTRL);
    break;
  case Qt::ALT:
    d->unsetModifier(Qt::ALT);
    break;
  case Qt::META:
    d->unsetModifier(Qt::META);
    break;
  }
}


/**
 * @brief Destructor
 *
 * @author Peter Grasch
 */
XEvents::~XEvents()
{
  delete d;
}
