/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_CARBONEVENTS_H_648808A9BE984CC98FF8681C9BF5D4F9
#define SIMON_CARBONEVENTS_H_648808A9BE984CC98FF8681C9BF5D4F9

#include "coreevents.h"
#include <QHash>
#include <ApplicationServices/ApplicationServices.h>

class CarbonEvents : public CoreEvents
{
  private:
    enum MouseButton
    {
      Left=1,
      Right=2,
      Middle=3
    };
    QHash<unsigned int, QPair<unsigned int /*modifiers*/, unsigned int /*keycode*/> > m_keyTable;
    inline QPair<unsigned int /*modifiers*/, unsigned int /*keycode*/> keyCodeForChar(unsigned short c);
    void buildKeyLookupTable();
    void scanKeyCodes(unsigned int modifiers, unsigned int modifierCodes, const UCKeyboardLayout * keyboardLayout, unsigned char kbdType);

    void setModifierKeyPrivate(int virtualKey);
    void unsetModifierKeyPrivate(int virtualKey);
    void pressCode(const CGKeyCode code, EventSimulation::PressMode mode, unsigned short string = 0);
    void moveMouse(int x, int y);
    void postClick(const CGMouseButton& button, const CGEventType& type, const CGPoint& point);

  public:
    CarbonEvents();
    void click(int x, int y, EventSimulation::ClickMode clickMode);
    void dragAndDrop(int xStart, int yStart, int x, int y);
    void sendKeyPrivate(unsigned int key /*unicode representation*/, EventSimulation::PressMode mode);
    ~CarbonEvents();

};
#endif
