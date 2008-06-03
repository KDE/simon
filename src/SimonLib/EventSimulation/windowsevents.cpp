//
// C++ Implementation: WindowsEvents
//
// Description: 
//
//
// Author: Phillip Goriup , (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "../Logging/logger.h"
#include <QCoreApplication>
#include "windowsevents.h"
#include <QMessageBox>
#include <ctype.h>


/**
 * @brief Constructor
 * saves the keys and the corresponding hex-values into a hash
 * saves the special character and there parent keys into a hash
 * @author Phillip Goriup
 */
WindowsEvents::WindowsEvents()
{
	this->keycodes = new QHash <char, int>();
	keycodes->insert('0',0x30);
	keycodes->insert('1',0x31);
	keycodes->insert('2',0x32);
	keycodes->insert('3',0x33);
	keycodes->insert('4',0x34);
	keycodes->insert('5',0x35);
	keycodes->insert('6',0x36);
	keycodes->insert('7',0x37);
	keycodes->insert('8',0x38);
	keycodes->insert('9',0x39);
	keycodes->insert('a',0x41);
	keycodes->insert('b',0x42);
	keycodes->insert('c',0x43);
	keycodes->insert('d',0x44);
	keycodes->insert('e',0x45);
	keycodes->insert('f',0x46);
	keycodes->insert('g',0x47);
	keycodes->insert('h',0x48);
	keycodes->insert('i',0x49);
	keycodes->insert('j',0x4A);
	keycodes->insert('k',0x4B);
	keycodes->insert('l',0x4C);
	keycodes->insert('m',0x4D);
	keycodes->insert('n',0x4E);
	keycodes->insert('o',0x4F),
	keycodes->insert('p',0x50);
	keycodes->insert('q',0x51);
	keycodes->insert('r',0x52);
	keycodes->insert('s',0x53);
	keycodes->insert('t',0x54);
	keycodes->insert('u',0x55);
	keycodes->insert('v',0x56);
	keycodes->insert('w',0x57);
	keycodes->insert('x',0x58);
	keycodes->insert('y',0x59);
	keycodes->insert('z',0x5A);
	
	keycodes->insert(' ',0x20);
	keycodes->insert('+',0xBB);
	keycodes->insert(',',0xBC);
	keycodes->insert('-',0xBD);
	keycodes->insert('.',0xBE);
	keycodes->insert('#',0xBF);
	keycodes->insert('ü',0xBA);
	keycodes->insert('ö',0xC0);
	keycodes->insert('ä',0xDE);
	keycodes->insert('´',0xDD);
	keycodes->insert('^',0xDC);
	keycodes->insert('ß',0xDB);
	keycodes->insert('<',0xE2);
	keycodes->insert('\n',0x0D);
	keycodes->insert('\r',0x0D);
	keycodes->insert('\r\n',0x0D);
	keycodes->insert('\t',0x09);


	this->shiftcodes = new QHash <char, char>();
	shiftcodes->insert('!','1');
	shiftcodes->insert('\"','2');
	shiftcodes->insert('§','3');
	shiftcodes->insert('$','4');
	shiftcodes->insert('%','5');
	shiftcodes->insert('&','6');
	shiftcodes->insert('/','7');
	shiftcodes->insert('(','8');
	shiftcodes->insert(')','9');
	shiftcodes->insert('=','0');
	shiftcodes->insert('?','ß');
	shiftcodes->insert('?','/');
	shiftcodes->insert('\'','#');
	shiftcodes->insert(';',',');
	shiftcodes->insert(':','.');
	shiftcodes->insert('_','-');
	shiftcodes->insert('*','+');
	shiftcodes->insert('Ü','ü');
	shiftcodes->insert('Ö','ö');
	shiftcodes->insert('Ä','ä');
	shiftcodes->insert('`','´');
	shiftcodes->insert('°','^');
	shiftcodes->insert('>','<');
	
	
	
	
	this->altgrcodes = new QHash <char, char>();
	altgrcodes->insert('²','2');
	altgrcodes->insert('³','3');
	altgrcodes->insert('{','7');
	altgrcodes->insert('[','8');
	altgrcodes->insert(']','9');
	altgrcodes->insert('}','0');
	altgrcodes->insert('\\','ß');
	altgrcodes->insert('@','q');
	altgrcodes->insert('€','e');
	altgrcodes->insert('~','+');
	altgrcodes->insert('|','<');
	altgrcodes->insert('µ','m');
	
	this->specialcodes = new QHash <int,char>();
	specialcodes->insert(8364,'e');
	
	this->modloger = 0;
}

/**
 * @brief press the key at the keyboard
 *
 * @param 
 * int key: saves the int value of the requested key
 * 
 * @author Phillip Goriup
 */
void WindowsEvents::sendKey(int key)
{	
	Sleep(50);
	keybd_event(key,0,0,0);	
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
	
	//windows API devides the screen into 65535*65535
	int clickx = x * 65535 / xsolution;
	int clicky = y * 65535 / ysolution;
	
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,clickx,clicky,0,0);

	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}

/**
 * @brief if it is a special character from an german keyboard, the parent key will be safed, and the modifier will be pressed
 *
 * @param unsigned shord key
 * unsigned shord key: the requested key as a unsigned short
 * 
 * @author Phillip Goriup
 */
void WindowsEvents::sendKey(unsigned short key /*unicode representation*/)
{
	int keyint = key;
	if (specialcodes->contains(keyint))
	{
		setModifierKey(VK_RMENU,false);
		key = specialcodes->value(keyint);
	}
	sendChar(key);
}

/**
 * @brief 
 *
 * @param Shortcut shortcut
 * 
 * 
 * @author Phillip Goriup
 */
void WindowsEvents::sendShortcut(Shortcut shortcut)
{
	
	int modifier = shortcut.getModifiers();
	if (modifier & KeyShift)
	{	
		setModifierKey(VK_SHIFT, true);
	}
	if (modifier & KeyAlt)
 		setModifierKey(VK_MENU, true);
 	if (modifier & KeyStrg)
	{
 		setModifierKey(VK_CONTROL, true);
 	}
 	if (modifier & KeySuper)
 		setModifierKey(VK_LWIN, true);
 	if (modifier & KeyCapsLock)
 		setModifierKey(VK_CAPITAL, true);
 	if (modifier & KeyAltGr)
 		setModifierKey(VK_RMENU, true);
 
 	
 	int action = shortcut.getActionKeys();
 	if (action & KeyBackspace)
 		sendKey(VK_BACK);
 	if (action & KeyEscape)
 		sendKey(VK_ESCAPE);
 	if (action & KeyClear)
 		sendKey(VK_OEM_CLEAR);
 	if (action & KeyPrintScr)
 		sendKey(VK_PRINT);
 	if (action & KeyPause)
 		sendKey(VK_PAUSE);
 	if (action & KeyEnter)
 		sendKey(VK_RETURN);
 
 	int movement = shortcut.getMovementKeys();
 	if (movement & KeyArrowLeft) 
 		sendKey(VK_LEFT);
 	if (movement & KeyArrowRight)
 		sendKey(VK_RIGHT);
 	if (movement & KeyArrowDown)
 		sendKey(VK_DOWN);
 	if (movement & KeyArrowUp)
 		sendKey(VK_UP);
 	if (movement & KeyPageUp)
 		sendKey(VK_PRIOR);
 	if (movement & KeyPageDown)
 		sendKey(VK_NEXT);
 	if (movement & KeyEnd)
 		sendKey(VK_END);
 	if (movement & KeyBegin)
 		sendKey(VK_HOME);
 	
 	int fkeys = shortcut.getFunctionKeys();
 	if (fkeys & KeyF1) sendKey(VK_F1);
 	if (fkeys & KeyF2) sendKey(VK_F2);
 	if (fkeys & KeyF3) sendKey(VK_F3);
 	if (fkeys & KeyF4) sendKey(VK_F4);
 	if (fkeys & KeyF5) sendKey(VK_F5);
 	if (fkeys & KeyF6) sendKey(VK_F6);
 	if (fkeys & KeyF7) sendKey(VK_F7);
 	if (fkeys & KeyF8) sendKey(VK_F8);
 	if (fkeys & KeyF9) sendKey(VK_F9);
 	if (fkeys & KeyF10) sendKey(VK_F10);
 	if (fkeys & KeyF11) sendKey(VK_F11);
 	if (fkeys & KeyF12) sendKey(VK_F12);
 	
 	char key = shortcut.getCharKey();
 	key = key + 32;
 	this->sendKey(this->keycodes->value(key));
 
 	unsetUnneededModifiers();
}

/**
 * @brief 
 *
 * @param int virtualKey
 * 
 * 
 * @author Phillip Goriup
 */
void WindowsEvents::sendChar(char key)
{	
	if (shiftcodes->contains(key))
	{ 	
		key = shiftcodes->value(key);
		setModifierKey(VK_LSHIFT,false);
	}
	if (altgrcodes->contains(key))
	{
		key = altgrcodes->value(key);
		setModifierKey(VK_RMENU,false);
	}
	this->sendKey(this->keycodes->value(key));
	unsetModifier(VK_LSHIFT);
	unsetModifier(VK_RMENU);
}

/**
 * @brief
 *
 * @param int virtualKey

 * @author Phillip Goriup
 */
void WindowsEvents::setModifierKey(int virtualKey, bool once)
{
	if (once)
	{
		keybd_event(virtualKey,0,0,0);
	}
	else
		keybd_event(virtualKey,0,0,0);
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
	keybd_event(virtualKey,0,KEYEVENTF_KEYUP,0);
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
		unsetModifier(VK_SHIFT);
		unsetModifier(VK_MENU);
		unsetModifier(VK_CONTROL);
		unsetModifier(VK_LWIN);
		unsetModifier(VK_CAPITAL);
		unsetModifier(VK_RMENU);
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



