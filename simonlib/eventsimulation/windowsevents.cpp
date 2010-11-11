/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include "windowsevents.h"
#include <QCoreApplication>
#include <QChar>
#include <KMessageBox>
#include <ctype.h>
#include <windows.h>
#include <winable.h>

#define VK_BROWSER_BACK 0xA6
#define VK_BROWSER_FAVORITES  0xAB
#define VK_BROWSER_FORWARD  0xA7
#define VK_BROWSER_HOME 0xAC
#define VK_BROWSER_REFRESH  0xA8
#define VK_BROWSER_SEARCH 0xAA
#define VK_BROWSER_STOP 0xA9

#define VK_LAUNCH_APP1  0xB6
#define VK_LAUNCH_APP2  0xB7
#define VK_LAUNCH_MAIL  0xB4
#define VK_LAUNCH_MEDIA_SELECT  0xB5

#define VK_MEDIA_NEXT_TRACK 0xB0
#define VK_MEDIA_PLAY_PAUSE 0xB3
#define VK_MEDIA_PREV_TRACK 0xB1
#define VK_MEDIA_STOP 0xB2

#define VK_VOLUME_DOWN  0xAE
#define VK_VOLUME_MUTE  0xAD
#define VK_VOLUME_UP  0xAF

/**
 * @brief Constructor
 * saves the keys and the corresponding hex-values into a hash
 * saves the special character and there parent keys into a hash
 * @author Phillip Goriup
 */
WindowsEvents::WindowsEvents() : CoreEvents()
{
}


void WindowsEvents::moveMouse(int x, int y)
{
  double fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1;
  double fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1;
  double fx = x*(65535.0f/fScreenWidth);
  double fy = y*(65535.0f/fScreenHeight);
  INPUT  Input={0};
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
  Input.mi.dx = fx;
  Input.mi.dy = fy;
  ::SendInput(1,&Input,sizeof(INPUT));
}


void WindowsEvents::activateMouseButton(MouseButton btn, PressMode mode)
{
  INPUT Input={0};
  if (mode & Down) {
    Input.type      = INPUT_MOUSE;
    switch (btn) {
      case Left:
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
        break;
      case Middle:
        Input.mi.dwFlags  = MOUSEEVENTF_MIDDLEDOWN;
        break;
      case Right:
        Input.mi.dwFlags  = MOUSEEVENTF_RIGHTDOWN;
        break;
    }

    ::SendInput(1,&Input,sizeof(INPUT));
  }

  if (mode & Up) {
    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.type      = INPUT_MOUSE;
    switch (btn) {
      case Left:
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
        break;
      case Middle:
        Input.mi.dwFlags  = MOUSEEVENTF_MIDDLEUP;
        break;
      case Right:
        Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP;
        break;
    }
    ::SendInput(1,&Input,sizeof(INPUT));
  }
}


/**
 * @brief simulates a mouseclick at the requested coordinates
 *
 * @author Phillip Goriup, Peter Grasch
 */
void WindowsEvents::click(int x, int y, EventSimulation::ClickMode clickMode)
{
  moveMouse(x, y);

  switch (clickMode) {
    case EventSimulation::LMB:
      activateMouseButton(Left, DownAndUp);
      break;
    case EventSimulation::LMBDouble:
      activateMouseButton(Left, DownAndUp);
      activateMouseButton(Left, DownAndUp);
      break;
    case EventSimulation::LMBDown:
      activateMouseButton(Left, Down);
      break;
    case EventSimulation::LMBUp:
      activateMouseButton(Left, Up);
      break;
    case EventSimulation::RMB:
      activateMouseButton(Right, DownAndUp);
      break;
    case EventSimulation::MMB:
      activateMouseButton(Middle, DownAndUp);
      break;
  }
}


void WindowsEvents::dragAndDrop(int xStart, int yStart, int x, int y)
{
  moveMouse(xStart, yStart);
  Sleep(200);
  activateMouseButton(Left, Down);
  Sleep(200);
  moveMouse(x, y);
  Sleep(200);
  activateMouseButton(Left, Up);
}


/**
 * @brief
 *
 * @param int virtualKey

 * @author Phillip Goriup
 */
void WindowsEvents::setModifierKey(int virtualKey, bool once)
{
  if ((!shiftSet) && (virtualKey & Qt::SHIFT)) {
    pressVk(VK_SHIFT, Down);
    shiftSet=true;
    shiftOnce=once;
  }
  if ((!altgrSet) && (virtualKey & Qt::Key_AltGr)) {
    pressVk(VK_RMENU, Down);
    altgrSet=true;
    altgrOnce=once;
  }
  if ((!strgSet) && (virtualKey & Qt::CTRL)) {
    pressVk(VK_CONTROL, Down);
    strgSet=true;
    strgOnce=once;
  }
  if ((!altSet) && (virtualKey & Qt::ALT)) {
    pressVk(VK_MENU, Down);
    altSet=true;
    altOnce=once;
  }
  if ((!superSet) && (virtualKey & Qt::META)) {
    pressVk(VK_LWIN, Down);
    superSet=true;
    superOnce=once;
  }
}


/**
 * @brief if it is a special character from an german keyboard, the parent key will be safed, and the modifier will be pressed
 *
 * @param unsigned shord key
 * unsigned shord key: the requested key as a unsigned int
 *
 * @author Phillip Goriup
 */
void WindowsEvents::sendKeyPrivate(unsigned int key /*unicode representation*/)
{
  int modifiers=0;
  BYTE virtualKey=0;
  switch (key) {
    case 8364 /* Euro */:
      modifiers = modifiers|Qt::CTRL|Qt::ALT;
      key = (unsigned int) 'e';
      break;
    case Qt::Key_Backspace:
      virtualKey = VK_BACK;
      break;
    case Qt::Key_Tab:
      virtualKey = VK_TAB;
      break;
    case Qt::Key_Clear:
      virtualKey = VK_CLEAR;
      break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case '\n':
      virtualKey = VK_RETURN;
      break;
    case Qt::Key_PageUp:
      virtualKey = VK_PRIOR;
      break;
    case Qt::Key_PageDown:
      virtualKey = VK_NEXT;
      break;

    case Qt::Key_End:
      virtualKey = VK_END;
      break;

    case Qt::Key_Home:
      virtualKey = VK_HOME;
      break;

    case Qt::Key_Left:
      virtualKey = VK_LEFT;
      break;
    case Qt::Key_Up:
      virtualKey = VK_UP;
      break;
    case Qt::Key_Right:
      virtualKey = VK_RIGHT;
      break;
    case Qt::Key_Down:
      virtualKey = VK_DOWN;
      break;

    case Qt::Key_Escape:
      virtualKey = VK_ESCAPE;
      break;

    case Qt::Key_Select:
      virtualKey = VK_SELECT;
      break;
    case Qt::Key_Print:
      virtualKey = VK_PRINT;
      break;
    case Qt::Key_Execute:
      virtualKey = VK_EXECUTE;
      break;
    case Qt::Key_Insert:
      virtualKey = VK_INSERT;
      break;
    case Qt::Key_Delete:
      virtualKey = VK_DELETE;
      break;
    case Qt::Key_Help:
      virtualKey = VK_HELP;
      break;
    case Qt::Key_Sleep:
      virtualKey = VK_SLEEP;
      break;

    case Qt::Key_F1:
      virtualKey = VK_F1;
      break;
    case Qt::Key_F2:
      virtualKey = VK_F2;
      break;
    case Qt::Key_F3:
      virtualKey = VK_F3;
      break;
    case Qt::Key_F4:
      virtualKey = VK_F4;
      break;
    case Qt::Key_F5:
      virtualKey = VK_F5;
      break;
    case Qt::Key_F6:
      virtualKey = VK_F6;
      break;
    case Qt::Key_F7:
      virtualKey = VK_F7;
      break;
    case Qt::Key_F8:
      virtualKey = VK_F8;
      break;
    case Qt::Key_F9:
      virtualKey = VK_F9;
      break;
    case Qt::Key_F10:
      virtualKey = VK_F10;
      break;
    case Qt::Key_F11:
      virtualKey = VK_F11;
      break;
    case Qt::Key_F12:
      virtualKey = VK_F12;
      break;
    case Qt::Key_F13:
      virtualKey = VK_F13;
      break;
    case Qt::Key_F14:
      virtualKey = VK_F14;
      break;
    case Qt::Key_F15:
      virtualKey = VK_F15;
      break;
    case Qt::Key_F16:
      virtualKey = VK_F16;
      break;
    case Qt::Key_F17:
      virtualKey = VK_F17;
      break;
    case Qt::Key_F18:
      virtualKey = VK_F18;
      break;
    case Qt::Key_F19:
      virtualKey = VK_F19;
      break;
    case Qt::Key_F20:
      virtualKey = VK_F20;
      break;
    case Qt::Key_F21:
      virtualKey = VK_F21;
      break;
    case Qt::Key_F22:
      virtualKey = VK_F22;
      break;
    case Qt::Key_F23:
      virtualKey = VK_F23;
      break;
    case Qt::Key_F24:
      virtualKey = VK_F24;
      break;

    case Qt::Key_Back:
      virtualKey = VK_BROWSER_BACK;
      break;
    case Qt::Key_Forward:
      virtualKey = VK_BROWSER_FORWARD;
      break;
    case Qt::Key_Refresh:
      virtualKey = VK_BROWSER_REFRESH;
      break;
    case Qt::Key_Stop:
      virtualKey = VK_BROWSER_STOP;
      break;
    case Qt::Key_Search:
      virtualKey = VK_BROWSER_SEARCH;
      break;
    case Qt::Key_Favorites:
      virtualKey = VK_BROWSER_FAVORITES;
      break;
    case Qt::Key_HomePage:
      virtualKey = VK_BROWSER_HOME;
      break;
    case Qt::Key_VolumeMute:
      virtualKey = VK_VOLUME_MUTE;
      break;
    case Qt::Key_VolumeDown:
      virtualKey = VK_VOLUME_DOWN;
      break;
    case Qt::Key_VolumeUp:
      virtualKey = VK_VOLUME_UP;
      break;
    case Qt::Key_MediaNext:
      virtualKey = VK_MEDIA_NEXT_TRACK;
      break;
    case Qt::Key_MediaPrevious:
      virtualKey = VK_MEDIA_PREV_TRACK;
      break;
    case Qt::Key_MediaStop:
      virtualKey = VK_MEDIA_STOP;
      break;
    case Qt::Key_MediaPlay:
      virtualKey = VK_MEDIA_PLAY_PAUSE;
      break;
    case Qt::Key_LaunchMail:
      virtualKey = VK_LAUNCH_MAIL;
      break;
    case Qt::Key_LaunchMedia:
      virtualKey = VK_LAUNCH_MEDIA_SELECT;
      break;
    case Qt::Key_Launch0:
      virtualKey = VK_LAUNCH_APP1;
      break;
    case Qt::Key_Launch1:
      virtualKey = VK_LAUNCH_APP2;
      break;
  }

  if (virtualKey == 0) {
    SHORT result = VkKeyScan(key);
    virtualKey = result % 256;                    //virtual key code
    BYTE high = result / 256;                     //shift state

    if (high & 1) {
      modifiers = modifiers|Qt::SHIFT;
    }
    if (high & 2) {
      modifiers = modifiers|Qt::CTRL;
    }
    if (high & 4) {
      modifiers = modifiers|Qt::ALT;
    }
  }

  setModifierKey(modifiers, true);
  pressVk(virtualKey, DownAndUp);

  unsetUnneededModifiers();
  Sleep(40);
}


void WindowsEvents::pressVk(BYTE vK, EventSimulation::ShortcutMode mode)
{
  INPUT *key = new INPUT;
  key->type = INPUT_KEYBOARD;
  key->ki.wVk = vK;
  key->ki.dwFlags = 0;
  key->ki.time = 0;
  key->ki.wScan = 0;
  key->ki.dwExtraInfo = 0;

  if (mode & Down) {
    SendInput(1,key,sizeof(INPUT));
  }
  if (mode & Up) {
    key->ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1,key,sizeof(INPUT));
  }

  delete key;
}


/**
 * @brief
 *
 * @param int virtualKey
 *
 *
 * @author Peter Grasch
 */
void WindowsEvents::unsetModifier(int virtualKey)
{
  int msVirtualKey = 0;
  
  if (virtualKey & Qt::SHIFT) {
	pressVk(VK_SHIFT, Up);
    shiftSet=false;
  }

  if (virtualKey & Qt::Key_AltGr) {
	pressVk(VK_RMENU, Up);
    altgrSet=false;
  }

  if (virtualKey & Qt::CTRL) {
	pressVk(VK_CONTROL, Up);
    strgSet=false;
  }

  if (virtualKey & Qt::ALT) {
	pressVk(VK_MENU, Up);
    altSet=false;
  }

  if (virtualKey & Qt::META) {
	pressVk(VK_LWIN, Up);
    superSet=false;
  }
}


/**
 * @brief
 *
 * @param int virtualKey
 *
 *
 * @author Phillip Goriup
 */
void WindowsEvents::unsetUnneededModifiers()
{
  if (shiftSet && shiftOnce) {
    unsetModifier(Qt::SHIFT);
    //unsetModifier(VK_SHIFT);
    shiftSet=false;
    shiftOnce=false;
  }
  if (altSet && altOnce) {
    unsetModifier(Qt::ALT);
    //unsetModifier(VK_MENU);
    altSet=false;
    altOnce=false;
  }
  if (strgSet && strgOnce) {
    unsetModifier(Qt::CTRL);
    //unsetModifier(VK_CONTROL);
    strgSet=false;
    strgOnce=false;
  }
  if (superSet && superOnce) {
    unsetModifier(Qt::META);
    //unsetModifier(VK_LWIN);
    superSet=false;
    superOnce=false;
  }
  if (altgrSet && altgrOnce) {
    unsetModifier(Qt::Key_AltGr);
    //unsetModifier(VK_RMENU);
    altgrSet=false;
    altgrOnce=false;
  }
}


/**
 * @brief Destructor
 *
 *
 *
 * @author Phillip Goriup
 */
WindowsEvents::~WindowsEvents()
{
}
