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

#define VK_BROWSER_BACK	0xA6
#define VK_BROWSER_FAVORITES	0xAB
#define VK_BROWSER_FORWARD	0xA7
#define VK_BROWSER_HOME	0xAC
#define VK_BROWSER_REFRESH	0xA8
#define VK_BROWSER_SEARCH	0xAA
#define VK_BROWSER_STOP	0xA9

#define VK_LAUNCH_APP1	0xB6	
#define VK_LAUNCH_APP2	0xB7	
#define VK_LAUNCH_MAIL	0xB4	
#define VK_LAUNCH_MEDIA_SELECT	0xB5

#define VK_MEDIA_NEXT_TRACK	0xB0	 
#define VK_MEDIA_PLAY_PAUSE	0xB3	
#define VK_MEDIA_PREV_TRACK	0xB1	
#define VK_MEDIA_STOP	0xB2	

#define VK_VOLUME_DOWN	0xAE	 
#define VK_VOLUME_MUTE	0xAD	
#define VK_VOLUME_UP	0xAF	

/**
 * @brief Constructor
 * saves the keys and the corresponding hex-values into a hash
 * saves the special character and there parent keys into a hash
 * @author Phillip Goriup
 */
WindowsEvents::WindowsEvents() : CoreEvents()
{
/*	
	altgrcodes.insert('²','2');
	altgrcodes.insert('³','3');
	altgrcodes.insert('{','7');
	altgrcodes.insert('[','8');
	altgrcodes.insert(']','9');
	altgrcodes.insert('}','0');
	altgrcodes.insert('\\','ß');
	altgrcodes.insert('@','q');
	altgrcodes.insert('€','e');
	altgrcodes.insert('~','+');
	altgrcodes.insert('|','<');
	altgrcodes.insert('µ','m');
	
	altgrcodes.insert(8364,'e');*/
}

/**
 * @brief simulates a mouseclick at the requested coordinates
 *
 * @param int x, int y
 * int x,y: coordinates on the display, where the mouseclick should be simulated
 * 
 * 
 * 
 * @author Phillip Goriup
 */
void WindowsEvents::click(int x, int y)
{	
	const long nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	const long nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int xsolution = nScreenWidth;
	int ysolution = nScreenHeight;
	
	//windows API divides the screen into 65535*65535
	int clickx = x * 65535 / xsolution;
	int clicky = y * 65535 / ysolution;



	// Create our array of INPUT structure.
	INPUT iClick[3];
	memset(iClick, 0, sizeof(INPUT*3));
	iClick[0].type = iClick[1].type = iClick[2].type = INPUT_MOUSE;

	iClick[0].mi.dx = clickx;
	iClick[0].mi.dy = clicky;
	iClick[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE;

	iClick[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	iClick[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(3, iClick, sizeof(INPUT)*3);
	
//	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,clickx,clicky,0,0);

//	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
//	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}



/**
 * @brief
 *
 * @param int virtualKey

 * @author Phillip Goriup
 */
void WindowsEvents::setModifierKey(int virtualKey, bool once)
{
	if ((!shiftSet) && (virtualKey & Qt::SHIFT))
	{
		//keybd_event(VK_SHIFT,0,0,0);
		pressVk(VK_SHIFT, Down);
		shiftSet=true;
		shiftOnce=once;
	}
	if ((!altgrSet) && (virtualKey & Qt::Key_AltGr))
	{
		//keybd_event(VK_RMENU,0,0,0);
		pressVk(VK_RMENU, Down);
		altgrSet=true;
		altgrOnce=once;
	}
	if ((!strgSet) && (virtualKey & Qt::CTRL))
	{
		//keybd_event(VK_CONTROL,0,0,0);
		pressVk(VK_CONTROL, Down);
		strgSet=true;
		strgOnce=once;
	}
	if ((!altSet) && (virtualKey & Qt::ALT))
	{
		//keybd_event(VK_MENU,0,0,0);
		pressVk(VK_MENU, Down);
		altSet=true;
		altOnce=once;
	}
	if ((!superSet) && (virtualKey & Qt::META))
	{
		//keybd_event(VK_LWIN,0,0,0);
		pressVk(VK_LWIN, Down);
		superSet=true;
		superOnce=once;
	}
	Sleep(50);
}

/**
 * @brief if it is a special character from an german keyboard, the parent key will be safed, and the modifier will be pressed
 *
 * @param unsigned shord key
 * unsigned shord key: the requested key as a unsigned int
 * 
 * @author Phillip Goriup
 */
void WindowsEvents::sendKey(unsigned int key /*unicode representation*/)
{
	int modifiers=0;
	BYTE virtualKey=0;
	switch (key)
	{
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
		case Qt::Key_Return:
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
			key = VK_ESCAPE;
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
			key = VK_SLEEP;
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
	
	if (virtualKey == 0)
	{
		SHORT result = VkKeyScan(key);
		virtualKey = result % 256;	//virtual key code
		BYTE high = result / 256; 	//shift state
		
		if (high & 1)
			modifiers = modifiers|Qt::SHIFT;
		if (high & 2)
			modifiers = modifiers|Qt::CTRL;
		if (high & 4)
			modifiers = modifiers|Qt::ALT;
	}
	
	Sleep(30);
	setModifierKey(modifiers, true);
//	//keybd_event(virtualKey,0,0,0);
	pressVk(virtualKey, Down|Up);
	
	unsetUnneededModifiers();
}

void WindowsEvents::pressVk(BYTE vK, KeyPressMode mode)
{
	INPUT *key = new INPUT;
	key->type = INPUT_KEYBOARD;
	key->ki.wVk = 41;
	key->ki.dwFlags = 0;
	key->ki.time = 0;
	key->ki.wScan = 0;

	if (mode & Down)
	{
		key->ki.dwExtraInfo = 0;
		SendInput(1,key,sizeof(INPUT));
	}
	if (mode & Up)
	{
		key->ki.dwExtraInfo = KEYEVENTF_KEYUP;
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
 * @author Phillip Goriup
 */
void WindowsEvents::unsetModifier(int virtualKey)
{
	//keybd_event(virtualKey,0,KEYEVENTF_KEYUP,0);
	pressVk(virtualKey, Up);
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
	if (shiftSet && shiftOnce)
	{
		unsetModifier(VK_SHIFT);
		shiftSet=false;
		shiftOnce=false;
	}
	if (altSet && altOnce)
	{
		unsetModifier(VK_MENU);
		altSet=false;
		altOnce=false;
	}
	if (strgSet && strgOnce)
	{
		unsetModifier(VK_CONTROL);
		strgSet=false;
		strgOnce=false;
	}
	if (superSet && superOnce)
	{
		unsetModifier(VK_LWIN);
		superSet=false;
		superOnce=false;
	}
	if (altgrSet && altgrOnce)
	{
		unsetModifier(VK_RMENU);
		altgrSet=false;
		altgrOnce=false;
	}
	Sleep(50);
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



