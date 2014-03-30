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

#ifndef SIMON_XEVENTS_H_2BF2636B0148404495C2D0D8A926A43A
#define SIMON_XEVENTS_H_2BF2636B0148404495C2D0D8A926A43A

#include "pressmode.h"
#include "pcevents.h"

class QString;

class XEventsPrivate;
/**
 * @class XEvents
 * @brief The X11 Event Backend
 *
 * Implements PCEvents
 *
 * @version 0.1
 * @date 4.03.2007
 * @author Peter Grasch
 */

class XEvents : public PCEvents
{
  private:
    XEventsPrivate *d;
    void setModifierKeyPrivate (int virtualKey);
    void unsetModifierKeyPrivate (int virtualKey);
    void sendKeyPrivate (unsigned int key, EventSimulation::PressMode mode);

  public:
    void click (int x, int y, EventSimulation::ClickMode clickMode);
    void dragAndDrop (int xStart, int yStart, int x, int y);
    XEvents (const char* displayName=":0.0");
    ~XEvents();

};
#endif
