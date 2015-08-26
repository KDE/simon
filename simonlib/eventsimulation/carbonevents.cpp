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

#include "carbonevents.h"
#include <unistd.h>
#include <Carbon/Carbon.h>
#include <QDebug>

CarbonEvents::CarbonEvents() : CoreEvents()
{
}


void CarbonEvents::moveMouse(int x, int y)
{
  CGEventRef move = CGEventCreateMouseEvent(
        NULL, kCGEventMouseMoved, CGPointMake(x, y),
        kCGMouseButtonLeft);
  CGEventPost(kCGHIDEventTap, move);
  CFRelease(move);
}

void CarbonEvents::postClick(const CGMouseButton& button, const CGEventType& type, const CGPoint& point)
{
  CGEventRef event = CGEventCreateMouseEvent(
        NULL, type, point, button);
  CGEventPost(kCGHIDEventTap, event);
  CFRelease(event);
}

void CarbonEvents::click(int x, int y, EventSimulation::ClickMode clickMode)
{
  CGPoint point = CGPointMake(x, y);

  switch (clickMode) {
    case EventSimulation::LMB:
      postClick(kCGMouseButtonLeft, kCGEventLeftMouseDown, point);
      postClick(kCGMouseButtonLeft, kCGEventLeftMouseUp, point);
      break;
    case EventSimulation::LMBDouble:
      postClick(kCGMouseButtonLeft, kCGEventLeftMouseDown, point);
      postClick(kCGMouseButtonLeft, kCGEventLeftMouseUp, point);
      postClick(kCGMouseButtonLeft, kCGEventLeftMouseDown, point);
      postClick(kCGMouseButtonLeft, kCGEventLeftMouseUp, point);
      break;
    case EventSimulation::LMBDown:
      postClick(kCGMouseButtonLeft, kCGEventLeftMouseDown, point);
      break;
    case EventSimulation::LMBUp:
      postClick(kCGMouseButtonLeft, kCGEventLeftMouseUp, point);
      break;
    case EventSimulation::RMB:
      postClick(kCGMouseButtonRight, kCGEventRightMouseDown, point);
      postClick(kCGMouseButtonRight, kCGEventRightMouseUp, point);
      break;
    case EventSimulation::MMB:
      postClick(kCGMouseButtonCenter, kCGEventOtherMouseDown, point);
      postClick(kCGMouseButtonCenter, kCGEventOtherMouseUp, point);
      break;
  }
}

void CarbonEvents::dragAndDrop(int xStart, int yStart, int x, int y)
{
  click(xStart, yStart, EventSimulation::LMBDown);
  usleep(20000);
  moveMouse(x,y);
  usleep(20000);
  click(x, y, EventSimulation::LMBUp);
}

void CarbonEvents::sendKeyPrivate(unsigned int key /*unicode representation*/, EventSimulation::PressMode mode)
{
  int modifiers=0;
  CGKeyCode virtualKey=0;
  switch (key) {
    case Qt::Key_Backspace:
      virtualKey = kVK_Delete;
      break;
    case Qt::Key_Tab:
      virtualKey = kVK_Tab;
      break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case '\n':
      virtualKey = kVK_Return;
      break;
    case Qt::Key_PageUp:
      virtualKey = kVK_PageUp;
      break;
    case Qt::Key_PageDown:
      virtualKey = kVK_PageDown;
      break;

    case Qt::Key_End:
      virtualKey = kVK_End;
      break;

    case Qt::Key_Home:
      virtualKey = kVK_Home;
      break;

    case Qt::Key_Left:
      virtualKey = kVK_LeftArrow;
      break;
    case Qt::Key_Up:
      virtualKey = kVK_UpArrow;
      break;
    case Qt::Key_Right:
      virtualKey = kVK_RightArrow;
      break;
    case Qt::Key_Down:
      virtualKey = kVK_DownArrow;
      break;

    case Qt::Key_Escape:
      virtualKey = kVK_Escape;
      break;

    case Qt::Key_Delete:
      virtualKey = kVK_ForwardDelete;
      break;
    case Qt::Key_Help:
      virtualKey = kVK_Help;
      break;
    case Qt::Key_F1:
      virtualKey = kVK_F1;
      break;
    case Qt::Key_F2:
      virtualKey = kVK_F2;
      break;
    case Qt::Key_F3:
      virtualKey = kVK_F3;
      break;
    case Qt::Key_F4:
      virtualKey = kVK_F4;
      break;
    case Qt::Key_F5:
      virtualKey = kVK_F5;
      break;
    case Qt::Key_F6:
      virtualKey = kVK_F6;
      break;
    case Qt::Key_F7:
      virtualKey = kVK_F7;
      break;
    case Qt::Key_F8:
      virtualKey = kVK_F8;
      break;
    case Qt::Key_F9:
      virtualKey = kVK_F9;
      break;
    case Qt::Key_F10:
      virtualKey = kVK_F10;
      break;
    case Qt::Key_F11:
      virtualKey = kVK_F11;
      break;
    case Qt::Key_F12:
      virtualKey = kVK_F12;
      break;
    case Qt::Key_F13:
      virtualKey = kVK_F13;
      break;
    case Qt::Key_F14:
      virtualKey = kVK_F14;
      break;
    case Qt::Key_F15:
      virtualKey = kVK_F15;
      break;
    case Qt::Key_F16:
      virtualKey = kVK_F16;
      break;
    case Qt::Key_F17:
      virtualKey = kVK_F17;
      break;
    case Qt::Key_F18:
      virtualKey = kVK_F18;
      break;
    case Qt::Key_F19:
      virtualKey = kVK_F19;
      break;
    case Qt::Key_F20:
      virtualKey = kVK_F20;
      break;

    case Qt::Key_VolumeMute:
      virtualKey = kVK_Mute;
      break;
    case Qt::Key_VolumeDown:
      virtualKey = kVK_VolumeDown;
      break;
    case Qt::Key_VolumeUp:
      virtualKey = kVK_VolumeUp;
      break;
    default:
      QPair<unsigned int, unsigned int> keysToPress = keyCodeForChar(key);
      modifiers = keysToPress.first;
      virtualKey = keysToPress.second;
  }

  setModifierKey(modifiers, mode & EventSimulation::Release);
  pressCode((CGKeyCode) virtualKey, mode, (unsigned short) key);
  unsetUnneededModifiers();
}

void CarbonEvents::pressCode(const CGKeyCode code, EventSimulation::PressMode mode, unsigned short string)
{
  CGEventRef event1;
  UniChar inputString[1];
  inputString[0] = (unsigned short) string;
  
  if (mode & EventSimulation::Press) {
    event1 = CGEventCreateKeyboardEvent (NULL, code, true);
    if (code == 0)
      CGEventKeyboardSetUnicodeString(event1, 1, inputString);
    CGEventPost(kCGHIDEventTap, event1);
    CFRelease(event1);
  }
  if (mode & EventSimulation::Release) {
    event1 = CGEventCreateKeyboardEvent (NULL, code, false);
    if (code == 0)
      CGEventKeyboardSetUnicodeString(event1, 1, inputString);
    CGEventPost(kCGHIDEventTap, event1);
    CFRelease(event1);
  }

  usleep(20000);
}

/* Returns key code for given character via the above function, or a pair of (unsigned) -1's
 * on error. 
 * Based on: http://stackoverflow.com/questions/1918841/how-to-convert-ascii-character-to-cgkeycode
 */
QPair<unsigned int /*modifiers*/, unsigned int /*keycode*/> CarbonEvents::keyCodeForChar(unsigned short c)
{
  if (m_keyTable.isEmpty())
    buildKeyLookupTable();

  return m_keyTable.value(c, qMakePair((unsigned int) -1,(unsigned int) -1));
}

void CarbonEvents::buildKeyLookupTable()
{
  TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardInputSource();
  CFDataRef layoutData = (CFDataRef) TISGetInputSourceProperty(currentKeyboard,
                                              kTISPropertyUnicodeKeyLayoutData);
  const UCKeyboardLayout *keyboardLayout =
        (const UCKeyboardLayout *)CFDataGetBytePtr(layoutData);

  unsigned int modifiers = 0;
  unsigned char kbdType = LMGetKbdType();

  
  // Loop through every keycode to find its current mapping.
  // todo: do the same with modifiers shift and alt:
  // tood: build dead key table with compose key

  /* Event record modifiers:
	cmdKey          = 0x0100  Command key down?
	shiftKey        = 0x0200  Shift key down?
	alphaLock       = 0x0400  Caps Lock key down?
	optionKey       = 0x0800  Option key down?
	controlKey      = 0x1000  Control key down?
	rightShiftKey   = 0x2000  Right Shift Key down?
	rightOptionKey  = 0x4000  Right Option Key down?
	rightControlKey = 0x8000  Right Control Key down?
  */
  modifiers = (0x0200 >> 8) & 0xFF;

  scanKeyCodes(0, 0, keyboardLayout, kbdType); //stock
  scanKeyCodes(Qt::SHIFT, (0x0200 >> 8) & 0xFF, keyboardLayout, kbdType); //shift
  scanKeyCodes(Qt::Key_Option, (0x0800 >> 8) & 0xFF, keyboardLayout, kbdType); //option
  scanKeyCodes(Qt::Key_Option|Qt::SHIFT, ((0x0800 >> 8)|(0x0800 >> 8)) & 0xFF, keyboardLayout, kbdType); //option+shift
  CFRelease(currentKeyboard);    
}


void CarbonEvents::scanKeyCodes(unsigned int modifiers, unsigned int modifierCodes, const UCKeyboardLayout * keyboardLayout, unsigned char kbdType)
{
  UInt32 keysDown = 0;
  UniChar chars[4];
  UniCharCount realLength;

  for (unsigned int i = 0; i < 128u; ++i) {
    UCKeyTranslate(keyboardLayout,
                   i,
                   kUCKeyActionDisplay,
                   modifierCodes,
                   kbdType,
                   kUCKeyTranslateNoDeadKeysBit,
                   &keysDown,
                   sizeof(chars) / sizeof(chars[0]),
                   &realLength,
                   chars);
    unsigned int unicodeValue = chars[0];
    if (realLength == 2) {
      unicodeValue += (chars[1] << 16);
    } else if (realLength > 2) {
      qDebug() << "Skipping unrepresentable unicode character" << modifiers << i;
      continue;
    }
    if (!m_keyTable.contains(unicodeValue))
      m_keyTable.insert(unicodeValue, qMakePair(modifiers, i));
  }
}

void CarbonEvents::setModifierKeyPrivate(int virtualKey)
{
  switch (virtualKey) {
  case Qt::SHIFT:
    pressCode(kVK_Shift, EventSimulation::Press);
    break;
  case Qt::CTRL:
    pressCode(kVK_Command, EventSimulation::Press);
    break;
  case Qt::ALT:
    pressCode(kVK_Option, EventSimulation::Press);
    break;
  case Qt::META:
    pressCode(kVK_Control, EventSimulation::Press);
    break;
  }
}

void CarbonEvents::unsetModifierKeyPrivate(int virtualKey)
{
  switch (virtualKey) {
  case Qt::SHIFT:
    pressCode(kVK_Shift, EventSimulation::Release);
    break;
  case Qt::CTRL:
    pressCode(kVK_Command, EventSimulation::Release);
    break;
  case Qt::ALT:
    pressCode(kVK_Option, EventSimulation::Release);
    break;
  case Qt::META:
    pressCode(kVK_Control, EventSimulation::Release);
    break;
  }
}

CarbonEvents::~CarbonEvents()
{
}
